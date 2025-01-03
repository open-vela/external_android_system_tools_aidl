/*
 * Copyright (C) 2018, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * limitations under the License.
 */

#include "aidl_to_ndk.h"
#include "aidl_language.h"
#include "aidl_to_cpp_common.h"
#include "logging.h"
#include "os.h"

#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include <functional>

using ::android::base::Join;
using ::android::base::Split;

namespace android {
namespace aidl {
namespace ndk {

static const AidlTypeSpecifier kIntType{AIDL_LOCATION_HERE, "int", /*array=*/std::nullopt, nullptr,
                                        Comments{}};

std::string NdkHeaderFile(const AidlDefinedType& defined_type, cpp::ClassNames name,
                          bool use_os_sep) {
  char seperator = (use_os_sep) ? OS_PATH_SEPARATOR : '/';
  return std::string("aidl") + seperator + cpp::HeaderFile(defined_type, name, use_os_sep);
}

// This represents a type in AIDL (e.g. 'String' which can be referenced in multiple ways)
struct TypeInfo {
  // name of the type in C++ output
  std::string cpp_name;
  // whether to prefer 'value type' over 'const&'
  bool value_is_cheap = false;
};

std::string ConstantValueDecorator(
    const AidlTypeSpecifier& type,
    const std::variant<std::string, std::vector<std::string>>& raw_value) {
  return cpp::CppConstantValueDecorator(type, raw_value, /*is_ndk=*/true);
};

// map from AIDL built-in type name to the corresponding Ndk type info
static map<std::string, TypeInfo> kNdkTypeInfoMap = {
    {"void", {"void", true}},
    {"boolean", {"bool", true}},
    {"byte", {"int8_t", true}},
    {"char", {"char16_t", true}},
    {"int", {"int32_t", true}},
    {"long", {"int64_t", true}},
    {"float", {"float", true}},
    {"double", {"double", true}},
    {"String", {"std::string"}},
    // TODO(b/136048684) {"Map", ""},
    {"IBinder", {"::ndk::SpAIBinder"}},
    {"ParcelFileDescriptor", {"::ndk::ScopedFileDescriptor"}},
    {"ParcelableHolder", {"::ndk::AParcelableHolder"}},
};

// map from AIDL built-in type name to the corresponding C Type Ndk info
static map<std::string, TypeInfo> kCTypeNdkTypeInfoMap = {
    {"void", {"void", true}},
    {"boolean", {"bool", true}},
    {"byte", {"int8_t", true}},
    {"char", {"char16_t", true}},
    {"int", {"int32_t", true}},
    {"long", {"int64_t", true}},
    {"float", {"float", true}},
    {"double", {"double", true}},
    {"String", {"char*", true}},
    // TODO(b/136048684) {"Map", ""},
    {"IBinder", {"::ndk::SpAIBinder"}},
    {"ParcelFileDescriptor", {"::ndk::ScopedFileDescriptor"}},
    {"ParcelableHolder", {"::ndk::AParcelableHolder"}},
};

static TypeInfo GetBaseTypeInfo(const AidlTypenames& types, const AidlTypeSpecifier& aidl, bool ndk_ctype) {
  auto& aidl_name = aidl.GetName();

  if (AidlTypenames::IsBuiltinTypename(aidl_name)) {
    const auto kTypeInfoMap = ndk_ctype ? kCTypeNdkTypeInfoMap : kNdkTypeInfoMap;
    auto it = kTypeInfoMap.find(aidl_name);
    AIDL_FATAL_IF(it == kTypeInfoMap.end(), aidl_name);
    return it->second;
  }
  const AidlDefinedType* type = types.TryGetDefinedType(aidl_name);
  AIDL_FATAL_IF(type == nullptr, aidl_name) << "Unrecognized type.";

  if (const AidlInterface* intf = type->AsInterface(); intf != nullptr) {
    const std::string clazz = NdkFullClassName(*intf, cpp::ClassNames::INTERFACE);
    return TypeInfo{"std::shared_ptr<" + clazz + ">"};
  } else if (const AidlParcelable* parcelable = type->AsParcelable(); parcelable != nullptr) {
    std::string clazz = NdkFullClassName(*parcelable, cpp::ClassNames::RAW);
    std::string template_params = "";
    if (aidl.IsGeneric()) {
      std::vector<std::string> type_params;
      for (const auto& parameter : aidl.GetTypeParameters()) {
        type_params.push_back(NdkNameOf(types, *parameter, StorageMode::STACK, ndk_ctype));
      }
      clazz += base::StringPrintf("<%s>", base::Join(type_params, ", ").c_str());
    }
    return TypeInfo{clazz};
  } else if (const AidlEnumDeclaration* enum_decl = type->AsEnumDeclaration();
             enum_decl != nullptr) {
    const std::string clazz = NdkFullClassName(*enum_decl, cpp::ClassNames::RAW);
    return TypeInfo{clazz, true};
  } else {
    AIDL_FATAL(aidl_name) << "Unrecognized type";
  }
}

static bool IsPointerTypeName(const std::string& cpp_name) {
  if (cpp_name == "bool*" || cpp_name == "uint8_t*" ||
      cpp_name == "char16_t*" || cpp_name == "int32_t*" ||
      cpp_name == "int64_t*" || cpp_name == "float*" || cpp_name == "double*" ||
      cpp_name == "char*" || cpp_name == "char**") {
    return true;
  }
  return false;
}

static TypeInfo WrapNullableType(TypeInfo info, bool is_heap, bool ndk_ctype) {
  if (is_heap) {
    info.cpp_name = "std::unique_ptr<" + info.cpp_name + ">";
  } else {
    if (ndk_ctype && IsPointerTypeName(info.cpp_name)) {
      return info;
    }
    info.cpp_name = "std::optional<" + info.cpp_name + ">";
  }
  info.value_is_cheap = false;
  return info;
}

static TypeInfo WrapArrayType(TypeInfo info, const ArrayType* array, bool ndk_ctype) {
  AIDL_FATAL_IF(!array, AIDL_LOCATION_HERE) << "not an array";
  // When "byte"(AIDL) is used in an array, use "uint8_t" because it's more C++ idiomatic.
  if (info.cpp_name == "int8_t") {
    info.cpp_name = "uint8_t";
  }
  if (ndk_ctype) {
    info.cpp_name = info.cpp_name + "*";
    info.value_is_cheap = true;
    return info;
  }
  if (std::get_if<DynamicArray>(array)) {
    info.cpp_name = "std::vector<" + info.cpp_name + ">";
  } else {
    const auto& dimensions = std::get<FixedSizeArray>(*array).dimensions;
    for (auto it = rbegin(dimensions), end = rend(dimensions); it != end; it++) {
      info.cpp_name = "std::array<" + info.cpp_name + ", " +
                      (*it)->ValueString(kIntType, ConstantValueDecorator) + ">";
    }
  }
  info.value_is_cheap = false;
  return info;
}

static bool ShouldWrapNullable(const AidlTypenames& types, const std::string& aidl_name) {
  if (AidlTypenames::IsPrimitiveTypename(aidl_name) || aidl_name == "ParcelableHolder" ||
      aidl_name == "IBinder" || aidl_name == "ParcelFileDescriptor") {
    return false;
  }
  if (auto defined_type = types.TryGetDefinedType(aidl_name); defined_type) {
    if (defined_type->AsEnumDeclaration() || defined_type->AsInterface()) {
      return false;
    }
  }
  return true;
}

static TypeInfo GetTypeInfo(const AidlTypenames& types, const AidlTypeSpecifier& aidl, bool ndk_ctype) {
  AIDL_FATAL_IF(!aidl.IsResolved(), aidl) << aidl.ToString();
  // Keep original @nullable to handle the case of List<T>. "@nullable" is attached to "List" not
  // "T"
  bool is_nullable = aidl.IsNullable();
  const ArrayType* array = nullptr;
  const AidlTypeSpecifier* element_type = &aidl;

  // List<T> is converted to T[].
  if (aidl.GetName() == "List") {
    static const ArrayType kDynamicArray{DynamicArray{}};

    AIDL_FATAL_IF(!aidl.IsGeneric(), aidl) << "List must be generic type.";
    AIDL_FATAL_IF(aidl.GetTypeParameters().size() != 1, aidl)
        << "List can accept only one type parameter.";
    const auto& type_param = *aidl.GetTypeParameters()[0];
    // TODO(b/136048684) AIDL doesn't support nested type parameter yet.
    AIDL_FATAL_IF(type_param.IsGeneric(), aidl) << "AIDL doesn't support nested type parameter";
    // Treat "List<T>" as an array of T.
    array = &kDynamicArray;
    element_type = &type_param;
  } else if (aidl.IsArray()) {
    array = &aidl.GetArray();
  }

  TypeInfo info = GetBaseTypeInfo(types, *element_type, ndk_ctype);

  if (is_nullable && ShouldWrapNullable(types, element_type->GetName())) {
    info = WrapNullableType(info, aidl.IsHeapNullable(), ndk_ctype);
  }
  if (array) {
    info = WrapArrayType(info, array, ndk_ctype);
    if (is_nullable) {
      AIDL_FATAL_IF(aidl.IsHeapNullable(), aidl) << "Array/List can't be @nullable(heap=true)";
      info = WrapNullableType(info, /*is_heap=*/false, ndk_ctype);
    }
  }
  return info;
}

std::string NdkFullClassName(const AidlDefinedType& type, cpp::ClassNames name) {
  std::vector<std::string> pieces = {"::aidl"};
  std::vector<std::string> split_name = Split(type.GetCanonicalName(), ".");
  pieces.insert(pieces.end(), split_name.begin(), split_name.end());
  // Override name part with cpp::ClassName(type, name)
  pieces.back() = cpp::ClassName(type, name);
  return Join(pieces, "::");
}

std::string NdkNameOf(const AidlTypenames& types, const AidlTypeSpecifier& aidl, StorageMode mode, bool ndk_ctype) {
  TypeInfo aspect = GetTypeInfo(types, aidl, ndk_ctype);

  switch (mode) {
    case StorageMode::STACK:
      return aspect.cpp_name;
    case StorageMode::ARGUMENT:
      if (aspect.value_is_cheap) {
        if (ndk_ctype && IsPointerTypeName(aspect.cpp_name)) {
          return "const " + aspect.cpp_name;
        }
        return aspect.cpp_name;
      } else {
        return "const " + aspect.cpp_name + "&";
      }
    case StorageMode::OUT_ARGUMENT:
      if (ndk_ctype && aidl.IsFixedSizeArray() && !aidl.IsNullable()) {
        return aspect.cpp_name;
      }
      return aspect.cpp_name + "*";
    default:
      AIDL_FATAL(aidl.GetName()) << "Unrecognized mode type: " << static_cast<int>(mode);
  }
}

void WriteToParcelFor(const CodeGeneratorContext& c) {
  if (c.type.IsNullable()) {
    c.writer << "::ndk::AParcel_writeNullableData(" << c.parcel << ", " << c.var << ")";
  } else {
    c.writer << "::ndk::AParcel_writeData(" << c.parcel << ", " << c.var << ")";
  }
}

void ReadFromParcelFor(const CodeGeneratorContext& c) {
  if (c.type.IsNullable()) {
    c.writer << "::ndk::AParcel_readNullableData(" << c.parcel << ", " << c.var << ")";
  } else {
    c.writer << "::ndk::AParcel_readData(" << c.parcel << ", " << c.var << ")";
  }
}

std::string NdkArgList(
    const AidlTypenames& types, const AidlMethod& method,
    std::function<std::string(const std::string& type, const std::string& name, bool isOut)>
        formatter, bool ndk_ctype, bool isDefinition) {
  std::vector<std::string> method_arguments;
  for (const auto& a : method.GetArguments()) {
    StorageMode mode = a->IsOut() ? StorageMode::OUT_ARGUMENT : StorageMode::ARGUMENT;
    std::string type = NdkNameOf(types, a->GetType(), mode, ndk_ctype);
    std::string name = cpp::BuildVarName(*a);
    bool is_out = a->IsOut();
    if (isDefinition && a->GetType().IsFixedSizeArray()) {
      if (a->GetType().IsNullable()) {
        name = name + "_p";
      } else {
        is_out = false;
      }
    }
    method_arguments.emplace_back(formatter(type, name, is_out));
    if (IsCtypeArray(a->GetType(), ndk_ctype) &&
        !a->GetType().IsFixedSizeArray()) {
      const std::string length_type =
          mode == StorageMode::OUT_ARGUMENT ? "int32_t*" : "const int32_t";
      method_arguments.emplace_back(
          formatter(length_type, name + "_length", is_out));
    }
  }

  if (method.GetType().GetName() != "void") {
    std::string type = NdkNameOf(types, method.GetType(), StorageMode::OUT_ARGUMENT, ndk_ctype);
    std::string name = "_aidl_return";
    bool is_out = true;
    if (isDefinition && method.GetType().IsFixedSizeArray()) {
      if (method.GetType().IsNullable()) {
        name = name + "_p";
      } else {
        is_out = false;
      }
    }
    method_arguments.emplace_back(formatter(type, name, is_out));
    if (IsCtypeArray(method.GetType(), ndk_ctype) &&
        !method.GetType().IsFixedSizeArray()) {
      const std::string length_type = "int32_t*";
      method_arguments.emplace_back(
          formatter(length_type, name + "_length", is_out));
    }
  }

  return Join(method_arguments, ", ");
}

std::string NdkMethodDecl(const AidlTypenames& types, const AidlMethod& method,
                          bool ndk_ctype, const std::string& clazz) {
  std::string class_prefix = clazz.empty() ? "" : (clazz + "::");
  return "::ndk::ScopedAStatus " + class_prefix + method.GetName() + "(" +
         NdkArgList(types, method, FormatArgForDecl, ndk_ctype) + ")";
}

bool IsCtypeArray(const AidlTypeSpecifier& aidl, bool ndk_ctype) {
  if (!ndk_ctype) {
    return false;
  }
  if (aidl.IsArray() || aidl.GetName() == "List") {
    return true;
  }
  return false;
}

std::string GetFixedSizeArrayLength(const AidlTypenames& types,
                                    const AidlTypeSpecifier& aidl) {
  AIDL_FATAL_IF(!aidl.IsResolved(), aidl) << aidl.ToString();
  if (!aidl.IsFixedSizeArray()) {
    return "";
  }

  TypeInfo info = GetBaseTypeInfo(types, aidl, true);

  const ArrayType *array = &aidl.GetArray();
  std::string array_length;
  if (array) {
    const auto &dimensions = std::get<FixedSizeArray>(*array).dimensions;
    for (auto it = rbegin(dimensions), end = rend(dimensions); it != end;
         it++) {
      array_length = (*it)->ValueString(kIntType, ConstantValueDecorator);
    }
  }

  return array_length;
}

}  // namespace ndk
}  // namespace aidl
}  // namespace android
