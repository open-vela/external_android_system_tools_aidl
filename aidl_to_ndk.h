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
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "aidl_language.h"
#include "aidl_to_cpp_common.h"

namespace android {
namespace aidl {
namespace ndk {

enum class StorageMode {
  STACK,
  ARGUMENT,      // Value for primitives, const& for larger types
  OUT_ARGUMENT,  // Pointer to raw type
};

std::string NdkHeaderFile(const AidlDefinedType& defined_type, cpp::ClassNames name,
                          bool use_os_sep = true);

std::string ConstantValueDecorator(
    const AidlTypeSpecifier& type,
    const std::variant<std::string, std::vector<std::string>>& raw_value);

// Returns ::aidl::some_package::some_sub_package::foo::IFoo/BpFoo/BnFoo
std::string NdkFullClassName(const AidlDefinedType& type, cpp::ClassNames name);

// Returns the corresponding Ndk type name for an AIDL type spec including
// array modifiers.
std::string NdkNameOf(const AidlTypenames& types, const AidlTypeSpecifier& aidl,
                      StorageMode mode, bool ndk_ctype);

struct CodeGeneratorContext {
  CodeWriter& writer;

  const AidlTypenames& types;
  const AidlTypeSpecifier& type;

  const string parcel;
  const string var;
};

void WriteToParcelFor(const CodeGeneratorContext& c);
void ReadFromParcelFor(const CodeGeneratorContext& c);

// Returns argument list of a method where each arg is formatted by the fomatter
std::string NdkArgList(
    const AidlTypenames& types, const AidlMethod& method,
    std::function<std::string(const std::string& type, const std::string& name, bool isOut)>
        formatter, bool ndk_ctype, bool isDefinition = false);

inline std::string FormatArgForDecl(const std::string& type, const std::string& name,
                                    bool /*isOut*/) {
  return type + " " + name;
}

inline std::string FormatArgNameUnused(const std::string& type, const std::string& name,
                                       bool /*isOut*/) {
  return type + " /*" + name + "*/";
}

inline std::string FormatArgForCall(const std::string& /*type*/, const std::string& name,
                                    bool isOut) {
  std::string reference_prefix = isOut ? "&" : "";
  return reference_prefix + name;
}

inline std::string FormatArgNameOnly(const std::string& /*type*/, const std::string& name,
                                     bool /*isOut*/) {
  return name;
}

inline std::string FormatArgForCtypeCall(const std::string& type,
                                         const std::string& name, bool isOut) {
  std::string reference_prefix = isOut ? "&" : "";
  std::string argname = name;
  if (!isOut && type == "const char**") {
    argname = "const_cast<const char**>(" + argname + ")";
  } else if (isOut && type == "const int32_t") {
    return argname;
  }
  return reference_prefix + argname;
}

inline std::string FormatArgForStlCall(const std::string& type,
                                       const std::string& name, bool isOut) {
  std::string reference_prefix = isOut ? "&" : "";
  std::string argname = name + "_c";
  if (!isOut) {
    if (type == "const char**") {
      argname = "const_cast<const char**>(" + argname + ")";
    }
  } else {
    if (type == "char**") {
      argname = "_aidl_return_c";
    } else if (type == "const int32_t") {
      return argname;
    }
  }
  return reference_prefix + argname;
}

// -> 'status (class::)name(type name, ...)' for a method
std::string NdkMethodDecl(const AidlTypenames& types, const AidlMethod& method,
                          bool ndk_ctype, const std::string& clazz = "");

bool IsCtypeArray(const AidlTypeSpecifier& aidl, bool ndk_ctype);

std::string GetFixedSizeArrayLength(const AidlTypenames& types,
                                    const AidlTypeSpecifier& aidl);

}  // namespace ndk
}  // namespace aidl
}  // namespace android
