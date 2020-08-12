/*
 * Copyright (C) 2016, The Android Open Source Project
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

#include "generate_java.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <map>
#include <memory>
#include <sstream>

#include <android-base/stringprintf.h>

#include "aidl_to_java.h"
#include "code_writer.h"
#include "logging.h"

using std::unique_ptr;
using ::android::aidl::java::Variable;
using std::string;

namespace {
inline string get_setter_name(const string& variablename) {
  CHECK(variablename.size() > 0) << "A field name cannot be empty.";
  std::ostringstream out;
  out << "set" << static_cast<char>(toupper(variablename[0])) << variablename.substr(1);
  return out.str();
}
}  // namespace

namespace android {
namespace aidl {
namespace java {

bool generate_java_interface(const string& filename, const AidlInterface* iface,
                             const AidlTypenames& typenames, const IoDelegate& io_delegate,
                             const Options& options) {
  auto cl = generate_binder_interface_class(iface, typenames, options);

  std::unique_ptr<Document> document =
      std::make_unique<Document>("" /* no comment */, iface->GetPackage(), std::move(cl));

  CodeWriterPtr code_writer = io_delegate.GetCodeWriter(filename);
  document->Write(code_writer.get());

  return true;
}

bool generate_java_parcel(const std::string& filename, const AidlStructuredParcelable* parcel,
                          const AidlTypenames& typenames, const IoDelegate& io_delegate) {
  auto cl = generate_parcel_class(parcel, typenames);

  std::unique_ptr<Document> document =
      std::make_unique<Document>("" /* no comment */, parcel->GetPackage(), std::move(cl));

  CodeWriterPtr code_writer = io_delegate.GetCodeWriter(filename);
  document->Write(code_writer.get());

  return true;
}

bool generate_java_enum_declaration(const std::string& filename,
                                    const AidlEnumDeclaration* enum_decl,
                                    const AidlTypenames& typenames, const IoDelegate& io_delegate) {
  CodeWriterPtr code_writer = io_delegate.GetCodeWriter(filename);
  generate_enum(code_writer, enum_decl, typenames);
  return true;
}

bool generate_java(const std::string& filename, const AidlDefinedType* defined_type,
                   const AidlTypenames& typenames, const IoDelegate& io_delegate,
                   const Options& options) {
  if (const AidlStructuredParcelable* parcelable = defined_type->AsStructuredParcelable();
      parcelable != nullptr) {
    return generate_java_parcel(filename, parcelable, typenames, io_delegate);
  }

  if (const AidlEnumDeclaration* enum_decl = defined_type->AsEnumDeclaration();
      enum_decl != nullptr) {
    return generate_java_enum_declaration(filename, enum_decl, typenames, io_delegate);
  }

  if (const AidlInterface* interface = defined_type->AsInterface(); interface != nullptr) {
    return generate_java_interface(filename, interface, typenames, io_delegate, options);
  }

  CHECK(false) << "Unrecognized type sent for java generation.";
  return false;
}

std::unique_ptr<android::aidl::java::Class> generate_parcel_class(
    const AidlStructuredParcelable* parcel, const AidlTypenames& typenames) {
  auto parcel_class = std::make_unique<Class>();
  parcel_class->comment = parcel->GetComments();
  parcel_class->modifiers = PUBLIC;
  parcel_class->what = Class::CLASS;
  parcel_class->type = parcel->GetCanonicalName();
  parcel_class->interfaces.push_back("android.os.Parcelable");
  parcel_class->annotations = generate_java_annotations(*parcel);

  if (parcel->IsGeneric()) {
    parcel_class->type += "<" + base::Join(parcel->GetTypeParameters(), ",") + ">";
  }

  for (const auto& variable : parcel->GetFields()) {
    std::ostringstream out;
    out << variable->GetType().GetComments() << "\n";
    for (const auto& a : generate_java_annotations(variable->GetType())) {
      out << a << "\n";
    }
    out << "public ";

    if (variable->GetType().GetName() == "ParcelableHolder" || parcel->IsJavaOnlyImmutable()) {
      out << "final ";
    }
    out << JavaSignatureOf(variable->GetType(), typenames) << " " << variable->GetName();
    if (!parcel->IsJavaOnlyImmutable() && variable->GetDefaultValue()) {
      out << " = " << variable->ValueString(ConstantValueDecorator);
    } else if (variable->GetType().GetName() == "ParcelableHolder") {
      out << std::boolalpha;
      out << " = new " << JavaSignatureOf(variable->GetType(), typenames) << "(";
      if (parcel->IsVintfStability()) {
        out << "android.os.Parcelable.PARCELABLE_STABILITY_VINTF";
      } else {
        out << "android.os.Parcelable.PARCELABLE_STABILITY_LOCAL";
      }
      out << ")";
      out << std::noboolalpha;
    }
    out << ";\n";
    parcel_class->elements.push_back(std::make_shared<LiteralClassElement>(out.str()));
  }

  std::ostringstream out;
  if (parcel->IsJavaOnlyImmutable()) {
    auto builder_class = std::make_shared<Class>();
    builder_class->modifiers = PUBLIC | FINAL | STATIC;
    builder_class->what = Class::CLASS;
    builder_class->type = "Builder";

    out.str("");
    for (const auto& variable : parcel->GetFields()) {
      out << "private " << JavaSignatureOf(variable->GetType(), typenames) << " "
          << variable->GetName();
      if (variable->GetDefaultValue()) {
        out << " = " << variable->ValueString(ConstantValueDecorator);
      }
      out << ";\n";
      out << "public Builder " << get_setter_name(variable->GetName()) << "("
          << JavaSignatureOf(variable->GetType(), typenames) << " " << variable->GetName()
          << ") {\n"
          << "  "
          << "this." << variable->GetName() << " = " << variable->GetName() << ";\n"
          << "  return this;\n"
          << "}\n";
    }
    out << "public " << parcel->GetCanonicalName() << " build() {\n"
        << "  return new " << parcel->GetCanonicalName() << "(";
    std::vector<std::string> variables;
    std::transform(parcel->GetFields().begin(), parcel->GetFields().end(),
                   std::back_inserter(variables), [](const auto& f) { return f->GetName(); });
    out << base::Join(variables, ", ") << ");\n"
        << "}\n";
    builder_class->elements.push_back(std::make_shared<LiteralClassElement>(out.str()));
    parcel_class->elements.push_back(builder_class);
  }
  if (parcel->IsVintfStability()) {
    parcel_class->elements.push_back(std::make_shared<LiteralClassElement>(
        "@Override\n public final int getStability() { return "
        "android.os.Parcelable.PARCELABLE_STABILITY_VINTF; }\n"));
  }

  out.str("");
  out << "public static final android.os.Parcelable.Creator<" << parcel->GetName() << "> CREATOR = "
      << "new android.os.Parcelable.Creator<" << parcel->GetName() << ">() {\n";
  out << "  @Override\n";
  out << "  public " << parcel->GetName()
      << " createFromParcel(android.os.Parcel _aidl_source) {\n";
  if (parcel->IsJavaOnlyImmutable()) {
    out << "    return internalCreateFromParcel(_aidl_source);\n";
  } else {
    out << "    " << parcel->GetName() << " _aidl_out = new " << parcel->GetName() << "();\n";
    out << "    _aidl_out.readFromParcel(_aidl_source);\n";
    out << "    return _aidl_out;\n";
  }
  out << "  }\n";
  out << "  @Override\n";
  out << "  public " << parcel->GetName() << "[] newArray(int _aidl_size) {\n";
  out << "    return new " << parcel->GetName() << "[_aidl_size];\n";
  out << "  }\n";
  out << "};\n";
  parcel_class->elements.push_back(std::make_shared<LiteralClassElement>(out.str()));

  auto flag_variable = std::make_shared<Variable>("int", "_aidl_flag");
  auto parcel_variable = std::make_shared<Variable>("android.os.Parcel", "_aidl_parcel");

  auto write_method = std::make_shared<Method>();
  write_method->modifiers = PUBLIC | OVERRIDE | FINAL;
  write_method->returnType = "void";
  write_method->name = "writeToParcel";
  write_method->parameters.push_back(parcel_variable);
  write_method->parameters.push_back(flag_variable);
  write_method->statements = std::make_shared<StatementBlock>();

  out.str("");
  out << "int _aidl_start_pos = _aidl_parcel.dataPosition();\n"
      << "_aidl_parcel.writeInt(0);\n";
  write_method->statements->Add(std::make_shared<LiteralStatement>(out.str()));

  for (const auto& field : parcel->GetFields()) {
    string code;
    CodeWriterPtr writer = CodeWriter::ForString(&code);
    CodeGeneratorContext context{
        .writer = *(writer.get()),
        .typenames = typenames,
        .type = field->GetType(),
        .parcel = parcel_variable->name,
        .var = field->GetName(),
        .is_return_value = false,
    };
    WriteToParcelFor(context);
    writer->Close();
    write_method->statements->Add(std::make_shared<LiteralStatement>(code));
  }

  out.str("");
  out << "int _aidl_end_pos = _aidl_parcel.dataPosition();\n"
      << "_aidl_parcel.setDataPosition(_aidl_start_pos);\n"
      << "_aidl_parcel.writeInt(_aidl_end_pos - _aidl_start_pos);\n"
      << "_aidl_parcel.setDataPosition(_aidl_end_pos);\n";

  write_method->statements->Add(std::make_shared<LiteralStatement>(out.str()));

  parcel_class->elements.push_back(write_method);

  if (parcel->IsJavaOnlyImmutable()) {
    auto constructor = std::make_shared<Method>();
    constructor->modifiers = PUBLIC;
    constructor->name = parcel->GetName();
    constructor->statements = std::make_shared<StatementBlock>();
    for (const auto& field : parcel->GetFields()) {
      constructor->parameters.push_back(std::make_shared<Variable>(
          JavaSignatureOf(field->GetType(), typenames), field->GetName()));
      out.str("");

      out << "this." << field->GetName() << " = ";
      if (field->GetType().GetName() == "List") {
        out << field->GetName() << " == null ? null : java.util.Collections.unmodifiableList("
            << field->GetName() << ");\n";
      } else if (field->GetType().GetName() == "Map") {
        out << field->GetName() << " == null ? null : java.util.Collections.unmodifiableMap("
            << field->GetName() << ");\n";
      } else {
        out << field->GetName() << ";\n";
      }
      constructor->statements->Add(std::make_shared<LiteralStatement>(out.str()));
    }
    parcel_class->elements.push_back(constructor);
  }

  // For an immutable parcelable, generate internalCreateFromParcel method.
  // Otherwise, generate readFromParcel method.
  auto read_or_create_method = std::make_shared<Method>();
  if (parcel->IsJavaOnlyImmutable()) {
    auto constructor = std::make_shared<Method>();
    read_or_create_method->modifiers = PRIVATE | STATIC;
    read_or_create_method->returnType = parcel->GetName();
    read_or_create_method->name = "internalCreateFromParcel";
    read_or_create_method->parameters.push_back(parcel_variable);
    read_or_create_method->statements = std::make_shared<StatementBlock>();
  } else {
    read_or_create_method->modifiers = PUBLIC | FINAL;
    read_or_create_method->returnType = "void";
    read_or_create_method->name = "readFromParcel";
    read_or_create_method->parameters.push_back(parcel_variable);
    read_or_create_method->statements = std::make_shared<StatementBlock>();
  }
  out.str("");
  const string builder_variable = "_aidl_parcelable_builder";
  if (parcel->IsJavaOnlyImmutable()) {
    out << "Builder " << builder_variable << " = new Builder();\n";
  }
  out << "int _aidl_start_pos = _aidl_parcel.dataPosition();\n"
      << "int _aidl_parcelable_size = _aidl_parcel.readInt();\n"
      << "try {\n"
      << "  if (_aidl_parcelable_size < 0) return";
  if (parcel->IsJavaOnlyImmutable()) {
    out << " " << builder_variable << ".build()";
  }
  out << ";\n";

  read_or_create_method->statements->Add(std::make_shared<LiteralStatement>(out.str()));

  out.str("");
  out << "  if (_aidl_parcel.dataPosition() - _aidl_start_pos >= _aidl_parcelable_size) return";
  if (parcel->IsJavaOnlyImmutable()) {
    out << " " << builder_variable << ".build()";
  }
  out << ";\n";

  std::shared_ptr<LiteralStatement> sizeCheck = nullptr;
  // keep this across different fields in order to create the classloader
  // at most once.
  bool is_classloader_created = false;
  for (const auto& field : parcel->GetFields()) {
    const auto field_variable_name =
        (parcel->IsJavaOnlyImmutable() ? "_aidl_temp_" : "") + field->GetName();
    string code;
    CodeWriterPtr writer = CodeWriter::ForString(&code);
    CodeGeneratorContext context{
        .writer = *(writer.get()),
        .typenames = typenames,
        .type = field->GetType(),
        .parcel = parcel_variable->name,
        .var = field_variable_name,
        .is_classloader_created = &is_classloader_created,
    };
    context.writer.Indent();
    if (parcel->IsJavaOnlyImmutable()) {
      context.writer.Write("%s %s;\n", JavaSignatureOf(field->GetType(), typenames).c_str(),
                           field_variable_name.c_str());
    }
    CreateFromParcelFor(context);
    if (parcel->IsJavaOnlyImmutable()) {
      context.writer.Write("%s.%s(%s);\n", builder_variable.c_str(),
                           get_setter_name(field->GetName()).c_str(), field_variable_name.c_str());
    }
    writer->Close();
    read_or_create_method->statements->Add(std::make_shared<LiteralStatement>(code));
    if (!sizeCheck) sizeCheck = std::make_shared<LiteralStatement>(out.str());
    read_or_create_method->statements->Add(sizeCheck);
  }

  out.str("");
  out << "} finally {\n"
      << "  if (_aidl_start_pos > (Integer.MAX_VALUE - _aidl_parcelable_size)) {\n"
      << "    throw new android.os.BadParcelableException(\"Overflow in the size of "
         "parcelable\");\n"
      << "  }\n"
      << "  _aidl_parcel.setDataPosition(_aidl_start_pos + _aidl_parcelable_size);\n";
  if (parcel->IsJavaOnlyImmutable()) {
    out << "  return " << builder_variable << ".build();\n";
  }
  out << "}\n";

  read_or_create_method->statements->Add(std::make_shared<LiteralStatement>(out.str()));

  parcel_class->elements.push_back(read_or_create_method);

  if (parcel->IsJavaDebug()) {
    out.str("");
    out << "@Override\n";
    out << "public String toString() {\n";
    out << "  java.util.StringJoiner _aidl_sj = new java.util.StringJoiner(";
    out << "\", \", \"{\", \"}\");\n";
    for (const auto& field : parcel->GetFields()) {
      std::string code;
      CodeWriterPtr writer = CodeWriter::ForString(&code);
      CodeGeneratorContext context{
          .writer = *(writer.get()),
          .typenames = typenames,
          .type = field->GetType(),
          .parcel = parcel_variable->name,
          .var = field->GetName(),
          .is_classloader_created = &is_classloader_created,
      };
      ToStringFor(context);
      writer->Close();
      out << "  _aidl_sj.add(\"" << field->GetName() << ": \" + (" << code << "));\n";
    }
    out << "  return \"" << parcel->GetCanonicalName() << "\" + _aidl_sj.toString()  ;\n";
    out << "}\n";
    parcel_class->elements.push_back(std::make_shared<LiteralClassElement>(out.str()));
  }

  auto describe_contents_method = std::make_shared<Method>();
  describe_contents_method->modifiers = PUBLIC | OVERRIDE;
  describe_contents_method->returnType = "int";
  describe_contents_method->name = "describeContents";
  describe_contents_method->statements = std::make_shared<StatementBlock>();
  describe_contents_method->statements->Add(std::make_shared<LiteralStatement>("return 0;\n"));
  parcel_class->elements.push_back(describe_contents_method);

  return parcel_class;
}

void generate_enum(const CodeWriterPtr& code_writer, const AidlEnumDeclaration* enum_decl,
                   const AidlTypenames& typenames) {
  code_writer->Write(
      "/*\n"
      " * This file is auto-generated.  DO NOT MODIFY.\n"
      " */\n");

  code_writer->Write("package %s;\n", enum_decl->GetPackage().c_str());
  code_writer->Write("%s\n", enum_decl->GetComments().c_str());
  for (const std::string& annotation : generate_java_annotations(*enum_decl)) {
    code_writer->Write("%s", annotation.c_str());
  }
  code_writer->Write("public @interface %s {\n", enum_decl->GetName().c_str());
  code_writer->Indent();
  for (const auto& enumerator : enum_decl->GetEnumerators()) {
    code_writer->Write("%s", enumerator->GetComments().c_str());
    code_writer->Write(
        "public static final %s %s = %s;\n",
        JavaSignatureOf(enum_decl->GetBackingType(), typenames).c_str(),
        enumerator->GetName().c_str(),
        enumerator->ValueString(enum_decl->GetBackingType(), ConstantValueDecorator).c_str());
  }
  code_writer->Dedent();
  code_writer->Write("}\n");
}

std::string dump_location(const AidlNode& method) {
  return method.PrintLocation();
}

std::string generate_java_unsupportedappusage_parameters(const AidlAnnotation& a) {
  const std::map<std::string, std::string> params = a.AnnotationParams(ConstantValueDecorator);
  std::vector<string> parameters_decl;
  for (const auto& name_and_param : params) {
    const std::string& param_name = name_and_param.first;
    const std::string& param_value = name_and_param.second;
    parameters_decl.push_back(param_name + " = " + param_value);
  }
  parameters_decl.push_back("overrideSourcePosition=\"" + dump_location(a) + "\"");
  return "(" + base::Join(parameters_decl, ", ") + ")";
}

std::vector<std::string> generate_java_annotations(const AidlAnnotatable& a) {
  std::vector<std::string> result;
  if (a.IsHide()) {
    result.emplace_back("@android.annotation.Hide");
  }

  const AidlAnnotation* unsupported_app_usage = a.UnsupportedAppUsage();
  if (unsupported_app_usage != nullptr) {
    result.emplace_back("@android.compat.annotation.UnsupportedAppUsage" +
                        generate_java_unsupportedappusage_parameters(*unsupported_app_usage));
  }

  auto strip_double_quote = [](const AidlTypeSpecifier& type, const std::string& raw_value) -> std::string {
    if (!android::base::StartsWith(raw_value, "\"") ||
        !android::base::EndsWith(raw_value, "\"")) {
      AIDL_FATAL(type) << "Java passthrough annotation " << raw_value << " is not properly quoted";
      return "";
    }
    return raw_value.substr(1, raw_value.size() - 2);
  };

  const AidlAnnotation* java_passthrough = a.JavaPassthrough();
  if (java_passthrough != nullptr) {
    for (const auto& name_and_param : java_passthrough->AnnotationParams(strip_double_quote)) {
      if (name_and_param.first == "annotation") {
        result.emplace_back(name_and_param.second);
        break;
      }
    }
  }

  return result;
}

}  // namespace java
}  // namespace aidl
}  // namespace android
