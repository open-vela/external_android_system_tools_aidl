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
#include "aidl_to_cpp_common.h"

#include <android-base/format.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include <set>
#include <unordered_map>

#include "ast_cpp.h"
#include "logging.h"
#include "os.h"

using ::android::base::Join;

namespace android {
namespace aidl {
namespace cpp {

string ClassName(const AidlDefinedType& defined_type, ClassNames type) {
  string base_name = defined_type.GetName();
  if (base_name.length() >= 2 && base_name[0] == 'I' && isupper(base_name[1])) {
    base_name = base_name.substr(1);
  }

  switch (type) {
    case ClassNames::CLIENT:
      return "Bp" + base_name;
    case ClassNames::SERVER:
      return "Bn" + base_name;
    case ClassNames::INTERFACE:
      return "I" + base_name;
    case ClassNames::DEFAULT_IMPL:
      return "I" + base_name + "Default";
    case ClassNames::BASE:
      return base_name;
    case ClassNames::RAW:
      [[fallthrough]];
    default:
      return defined_type.GetName();
  }
}

std::string HeaderFile(const AidlDefinedType& defined_type, ClassNames class_type,
                       bool use_os_sep) {
  std::string file_path = defined_type.GetPackage();
  for (char& c : file_path) {
    if (c == '.') {
      c = (use_os_sep) ? OS_PATH_SEPARATOR : '/';
    }
  }
  if (!file_path.empty()) {
    file_path += (use_os_sep) ? OS_PATH_SEPARATOR : '/';
  }
  file_path += ClassName(defined_type, class_type);
  file_path += ".h";

  return file_path;
}

void EnterNamespace(CodeWriter& out, const AidlDefinedType& defined_type) {
  const std::vector<std::string> packages = defined_type.GetSplitPackage();
  for (const std::string& package : packages) {
    out << "namespace " << package << " {\n";
  }
}
void LeaveNamespace(CodeWriter& out, const AidlDefinedType& defined_type) {
  const std::vector<std::string> packages = defined_type.GetSplitPackage();
  for (auto it = packages.rbegin(); it != packages.rend(); ++it) {
    out << "}  // namespace " << *it << "\n";
  }
}

string BuildVarName(const AidlArgument& a) {
  string prefix = "out_";
  if (a.GetDirection() & AidlArgument::IN_DIR) {
    prefix = "in_";
  }
  return prefix + a.GetName();
}

struct TypeInfo {
  // name of the type in C++ output
  std::string cpp_name;

  // function that writes an expression to convert a variable to a Json::Value
  // object
  std::function<void(CodeWriter& w, const string& var_name, bool isNdk)> toJsonValueExpr;
};

const static std::unordered_map<std::string, TypeInfo> kTypeInfoMap = {
    {"void", {"void", nullptr}},
    {"boolean",
     {
         "bool",
         [](CodeWriter& c, const string& var_name, bool) {
           c << "Json::Value(" << var_name << "? \"true\" : \"false\")";
         },
     }},
    {"byte",
     {
         "int8_t",
         [](CodeWriter& c, const string& var_name, bool) {
           c << "Json::Value(" << var_name << ")";
         },
     }},
    {"char",
     {
         "char16_t",
         [](CodeWriter& c, const string& var_name, bool isNdk) {
           if (isNdk) {
             c << "Json::Value(" << var_name << ")";
           } else {
             c << "Json::Value(std::string(android::String8(&" << var_name << ", 1)))";
           }
         },
     }},
    {"int",
     {
         "int32_t",
         [](CodeWriter& c, const string& var_name, bool) {
           c << "Json::Value(" << var_name << ")";
         },
     }},
    {"long",
     {
         "int64_t",
         [](CodeWriter& c, const string& var_name, bool) {
           c << "Json::Value(static_cast<Json::Int64>(" << var_name << "))";
         },
     }},
    {"float",
     {
         "float",
         [](CodeWriter& c, const string& var_name, bool) {
           c << "Json::Value(" << var_name << ")";
         },
     }},
    {"double",
     {
         "double",
         [](CodeWriter& c, const string& var_name, bool) {
           c << "Json::Value(" << var_name << ")";
         },
     }},
    {"String",
     {
         "std::string",
         [](CodeWriter& c, const string& var_name, bool) {
           c << "Json::Value(" << var_name << ")";
         },
     }}
    // missing List, Map, ParcelFileDescriptor, IBinder
};

TypeInfo GetTypeInfo(const AidlTypeSpecifier& aidl) {
  AIDL_FATAL_IF(!aidl.IsResolved(), aidl) << aidl.ToString();
  const string& aidl_name = aidl.GetName();

  TypeInfo info;
  if (AidlTypenames::IsBuiltinTypename(aidl_name)) {
    auto it = kTypeInfoMap.find(aidl_name);
    if (it != kTypeInfoMap.end()) {
      info = it->second;
    }
  }
  // Missing interface and parcelable type
  return info;
}

inline bool CanWriteLog(const TypeInfo& t) {
  return t.cpp_name != "";
}

bool CanWriteLog(const AidlTypeSpecifier& aidl) {
  return CanWriteLog(GetTypeInfo(aidl));
}

void WriteLogFor(CodeWriter& writer, const AidlTypeSpecifier& type, const std::string& name,
                 bool isPointer, const std::string& log, bool isNdk) {
  const TypeInfo info = GetTypeInfo(type);
  if (!CanWriteLog(info)) {
    return;
  }

  const string var_object_expr = ((isPointer ? "*" : "")) + name;
  if (type.IsArray()) {
    writer << log << " = Json::Value(Json::arrayValue);\n";
    writer << "for (const auto& v: " << var_object_expr << ") " << log << ".append(";
    info.toJsonValueExpr(writer, "v", isNdk);
    writer << ");";
  } else {
    writer << log << " = ";
    info.toJsonValueExpr(writer, var_object_expr, isNdk);
    writer << ";";
  }
  writer << "\n";
}

void WriteLogForArguments(CodeWriterPtr& writer, const AidlArgument& a, bool isServer,
                          string logVarName, bool isNdk) {
  if (!CanWriteLog(a.GetType())) {
    return;
  }
  string logElementVarName = "_log_arg_element";
  (*writer) << "{\n";
  (*writer).Indent();
  (*writer) << "Json::Value " << logElementVarName << "(Json::objectValue);\n";
  string varName = isServer || isNdk ? BuildVarName(a) : a.GetName();
  (*writer) << logElementVarName << "[\"name\"] = \"" << varName << "\";\n";

  bool isPointer = a.IsOut() && !isServer;
  WriteLogFor(*(writer.get()), a.GetType(), varName, isPointer, logElementVarName + "[\"value\"]",
              isNdk);
  (*writer) << logVarName << ".append(" << logElementVarName << ");\n";
  (*writer) << "}\n";
  (*writer).Dedent();
}

const string GenLogBeforeExecute(const string className, const AidlMethod& method, bool isServer,
                                 bool isNdk) {
  string code;
  CodeWriterPtr writer = CodeWriter::ForString(&code);
  (*writer) << "Json::Value _log_input_args(Json::arrayValue);\n";

  (*writer) << "if (" << className << "::logFunc != nullptr) {\n";
  (*writer).Indent();

  for (const auto& a : method.GetArguments()) {
    if (a->IsIn()) {
      WriteLogForArguments(writer, *a, isServer, "_log_input_args", isNdk);
    }
  }

  (*writer).Dedent();
  (*writer) << "}\n";

  (*writer) << "auto _log_start = std::chrono::steady_clock::now();\n";
  writer->Close();
  return code;
}

const string GenLogAfterExecute(const string className, const AidlInterface& interface,
                                const AidlMethod& method, const string& statusVarName,
                                const string& returnVarName, bool isServer, bool isNdk) {
  string code;
  CodeWriterPtr writer = CodeWriter::ForString(&code);

  (*writer) << "if (" << className << "::logFunc != nullptr) {\n";
  (*writer).Indent();

  // Write the log as a Json object. For example,
  //
  // Json log object for following interface description
  //
  // package foo.bar;
  // interface IFoo {
  //   String TestMethod(int arg1, inout String[] arg2, out double arg3);
  // }
  //
  // would be:
  //
  // {
  //   duration_ms: 100.42,
  //   interface_name: "foo.bar.IFoo",
  //   method_name: "TestMethod",
  //   (proxy|stub)_address: "0x12345678",
  //   input_args: [
  //     {name: "arg1", value: 30,},
  //     {name: "arg2", value: ["apple", "grape"],},
  //   ],
  //   output_args: [
  //     {name: "arg2", value: ["mango", "banana"],},
  //     {name: "arg3", value: "10.5",},
  //   ],
  //   _aidl_return: "ok",
  //   binder_status: {
  //     exception_code: -8,
  //     exception_message: "Something wrong",
  //     transaction_error: 0,
  //     service_specific_error_code: -42,
  //   },
  // }
  (*writer) << "auto _log_end = std::chrono::steady_clock::now();\n";
  (*writer) << "Json::Value _log_transaction(Json::objectValue);\n";
  (*writer) << "_log_transaction[\"duration_ms\"] = "
            << "std::chrono::duration<double, std::milli>(_log_end - "
               "_log_start).count();\n";
  (*writer) << "_log_transaction[\"interface_name\"] = "
            << "Json::Value(\"" << interface.GetCanonicalName() << "\");\n";
  (*writer) << "_log_transaction[\"method_name\"] = "
            << "Json::Value(\"" << method.GetName() << "\");\n";

  (*writer) << "_log_transaction[\"" << (isServer ? "stub_address" : "proxy_address") << "\"] = ";
  (*writer) << "Json::Value("
            << "(std::ostringstream() << "
            << (isNdk && isServer ? "_aidl_impl" : "static_cast<const void*>(this)") << ").str()"
            << ");\n";
  (*writer) << "_log_transaction[\"input_args\"] = _log_input_args;\n";
  (*writer) << "Json::Value _log_output_args(Json::arrayValue);\n";

  (*writer) << "Json::Value _log_status(Json::objectValue);\n";
  if (isNdk) {
    (*writer) << "_log_status[\"exception_code\"] = Json::Value(AStatus_getExceptionCode("
              << statusVarName << ".get()));\n";
    (*writer) << "_log_status[\"exception_message\"] = Json::Value(AStatus_getMessage("
              << statusVarName << ".get()));\n";
    (*writer) << "_log_status[\"transaction_error\"] = Json::Value(AStatus_getStatus("
              << statusVarName << ".get()));\n";
    (*writer) << "_log_status[\"service_specific_error_code\"] = "
                 "Json::Value(AStatus_getServiceSpecificError("
              << statusVarName << ".get()));\n";
  } else {
    (*writer) << "_log_status[\"exception_code\"] = Json::Value(" << statusVarName
              << ".exceptionCode());\n";
    (*writer) << "_log_status[\"exception_message\"] = Json::Value(" << statusVarName
              << ".exceptionMessage());\n";
    (*writer) << "_log_status[\"transaction_error\"] = Json::Value(" << statusVarName
              << ".transactionError());\n";
    (*writer) << "_log_status[\"service_specific_error_code\"] = Json::Value(" << statusVarName
              << ".serviceSpecificErrorCode());\n";
  }

  (*writer) << "_log_transaction[\"binder_status\"] = _log_status;\n";

  for (const auto& a : method.GetOutArguments()) {
    WriteLogForArguments(writer, *a, isServer, "_log_output_args", isNdk);
  }

  (*writer) << "_log_transaction[\"output_args\"] = _log_output_args;\n";

  if (method.GetType().GetName() != "void") {
    WriteLogFor(*(writer.get()), method.GetType(), returnVarName, !isServer,
                "_log_transaction[\"" + returnVarName + "\"]", isNdk);
  }

  // call the user-provided function with the Json object for the entire
  // transaction
  (*writer) << className << "::logFunc(_log_transaction);\n";

  (*writer).Dedent();
  (*writer) << "}\n";

  writer->Close();
  return code;
}

std::string GenerateEnumValues(const AidlEnumDeclaration& enum_decl,
                               const std::vector<std::string>& enclosing_namespaces_of_enum_decl) {
  const auto fq_name =
      Join(Append(enclosing_namespaces_of_enum_decl, enum_decl.GetSplitPackage()), "::") +
      "::" + enum_decl.GetName();
  const auto size = enum_decl.GetEnumerators().size();
  std::ostringstream code;
  code << "#pragma clang diagnostic push\n";
  code << "#pragma clang diagnostic ignored \"-Wc++17-extensions\"\n";
  code << "template <>\n";
  code << "constexpr inline std::array<" << fq_name << ", " << size << "> enum_values<" << fq_name
       << "> = {\n";
  for (const auto& enumerator : enum_decl.GetEnumerators()) {
    code << "  " << fq_name << "::" << enumerator->GetName() << ",\n";
  }
  code << "};\n";
  code << "#pragma clang diagnostic pop\n";
  return code.str();
}

std::string TemplateDecl(const AidlParcelable& defined_type) {
  std::string decl = "";
  if (defined_type.IsGeneric()) {
    std::vector<std::string> template_params;
    for (const auto& parameter : defined_type.GetTypeParameters()) {
      template_params.push_back(parameter);
    }
    decl = base::StringPrintf("template <typename %s>\n",
                              base::Join(template_params, ", typename ").c_str());
  }
  return decl;
}

void GenerateParcelableComparisonOperators(CodeWriter& out, const AidlParcelable& parcelable) {
  std::set<string> operators{"<", ">", "==", ">=", "<=", "!="};
  bool is_empty = false;

  auto comparable = [&](const string& prefix) {
    vector<string> fields;
    if (auto p = parcelable.AsStructuredParcelable(); p != nullptr) {
      is_empty = p->GetFields().empty();
      for (const auto& f : p->GetFields()) {
        fields.push_back(prefix + f->GetName());
      }
      return "std::tie(" + Join(fields, ", ") + ")";
    } else if (auto p = parcelable.AsUnionDeclaration(); p != nullptr) {
      return prefix + "_value";
    } else {
      AIDL_FATAL(parcelable) << "Unknown paracelable type";
    }
  };

  string lhs = comparable("");
  string rhs = comparable("rhs.");
  for (const auto& op : operators) {
    out << "inline bool operator" << op << "(const " << parcelable.GetName() << "&"
        << (is_empty ? "" : " rhs") << ") const {\n"
        << "  return " << lhs << " " << op << " " << rhs << ";\n"
        << "}\n";
  }
  out << "\n";
}

const vector<string> UnionWriter::headers{
    "type_traits",  // std::is_same_v
    "utility",      // std::mode/forward for value
    "variant",      // std::variant for value
};

void UnionWriter::PrivateFields(CodeWriter& out) const {
  vector<string> field_types;
  for (const auto& f : decl.GetFields()) {
    field_types.push_back(name_of(f->GetType(), typenames));
  }
  out << "std::variant<" + Join(field_types, ", ") + "> _value;\n";
}

void UnionWriter::PublicFields(CodeWriter& out) const {
  AidlTypeSpecifier tag_type(AIDL_LOCATION_HERE, "int", /* is_array= */ false,
                             /* type_params= */ nullptr, /* comments= */ "");
  tag_type.Resolve(typenames);

  out << "enum Tag : " << name_of(tag_type, typenames) << " {\n";
  bool is_first = true;
  for (const auto& f : decl.GetFields()) {
    out << "  " << f->GetName() << (is_first ? " = 0" : "") << ",  // " << f->Signature() << ";\n";
    is_first = false;
  }
  out << "};\n";

  const auto& name = decl.GetName();

  AIDL_FATAL_IF(decl.GetFields().empty(), decl) << "Union '" << name << "' is empty.";
  const auto& first_field = decl.GetFields()[0];
  const auto& default_name = first_field->GetName();
  const auto& default_value =
      name_of(first_field->GetType(), typenames) + "(" + first_field->ValueString(decorator) + ")";

  auto tmpl = R"--(
template<typename _Tp>
static constexpr bool _not_self = !std::is_same_v<std::remove_cv_t<std::remove_reference_t<_Tp>>, {name}>;

{name}() : _value(std::in_place_index<{default_name}>, {default_value}) {{ }}
{name}(const {name}&) = default;
{name}({name}&&) = default;
{name}& operator=(const {name}&) = default;
{name}& operator=({name}&&) = default;

template <typename _Tp, std::enable_if_t<_not_self<_Tp>, int> = 0>
constexpr {name}(_Tp&& _arg)
    : _value(std::forward<_Tp>(_arg)) {{}}

template <typename... _Tp>
constexpr explicit {name}(_Tp&&... _args)
    : _value(std::forward<_Tp>(_args)...) {{}}

template <Tag _tag, typename... _Tp>
static {name} make(_Tp&&... _args) {{
  return {name}(std::in_place_index<_tag>, std::forward<_Tp>(_args)...);
}}

template <Tag _tag, typename _Tp, typename... _Up>
static {name} make(std::initializer_list<_Tp> _il, _Up&&... _args) {{
  return {name}(std::in_place_index<_tag>, std::move(_il), std::forward<_Up>(_args)...);
}}

Tag getTag() const {{
  return static_cast<Tag>(_value.index());
}}

template <Tag _tag>
const auto& get() const {{
  if (getTag() != _tag) {{ abort(); }}
  return std::get<_tag>(_value);
}}

template <Tag _tag>
auto& get() {{
  if (getTag() != _tag) {{ abort(); }}
  return std::get<_tag>(_value);
}}

template <Tag _tag, typename... _Tp>
void set(_Tp&&... _args) {{
  _value.emplace<_tag>(std::forward<_Tp>(_args)...);
}}

)--";
  out << fmt::format(tmpl, fmt::arg("name", name), fmt::arg("default_name", default_name),
                     fmt::arg("default_value", default_value));
}

void UnionWriter::ReadFromParcel(CodeWriter& out, const ParcelWriterContext& ctx) const {
  AidlTypeSpecifier tag_type(AIDL_LOCATION_HERE, "int", /* is_array= */ false,
                             /* type_params= */ nullptr, /* comments= */ "");
  tag_type.Resolve(typenames);

  const string tag = "_aidl_tag";
  const string value = "_aidl_value";
  const string status = "_aidl_ret_status";

  auto read_var = [&](const string& var, const AidlTypeSpecifier& type) {
    out << fmt::format("{} {};\n", name_of(type, typenames), var);
    out << fmt::format("if (({} = ", status);
    ctx.read_func(out, var, type);
    out << fmt::format(") != {}) return {};\n", ctx.status_ok, status);
  };

  out << fmt::format("{} {};\n", ctx.status_type, status);
  read_var(tag, tag_type);
  out << fmt::format("switch ({}) {{\n", tag);
  for (const auto& variable : decl.GetFields()) {
    out << fmt::format("case {}: {{\n", variable->GetName());
    out.Indent();
    read_var(value, variable->GetType());
    out << fmt::format("set<{}>(std::move({}));\n", variable->GetName(), value);
    out << fmt::format("return {}; }}\n", ctx.status_ok);
    out.Dedent();
  }
  out << "}\n";
  out << fmt::format("return {};\n", ctx.status_bad);
}

void UnionWriter::WriteToParcel(CodeWriter& out, const ParcelWriterContext& ctx) const {
  AidlTypeSpecifier tag_type(AIDL_LOCATION_HERE, "int", /* is_array= */ false,
                             /* type_params= */ nullptr, /* comments= */ "");
  tag_type.Resolve(typenames);

  const string tag = "_aidl_tag";
  const string value = "_aidl_value";
  const string status = "_aidl_ret_status";

  out << fmt::format("{} {} = ", ctx.status_type, status);
  ctx.write_func(out, "getTag()", tag_type);
  out << ";\n";
  out << fmt::format("if ({} != {}) return {};\n", status, ctx.status_ok, status);
  out << "switch (getTag()) {\n";
  for (const auto& variable : decl.GetFields()) {
    out << fmt::format("case {}: return ", variable->GetName());
    ctx.write_func(out, "get<" + variable->GetName() + ">()", variable->GetType());
    out << ";\n";
  }
  out << "}\n";
  out << "abort();\n";
}

}  // namespace cpp
}  // namespace aidl
}  // namespace android
