/*
 * Copyright (C) 2020, The Android Open Source Project
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

#include "generate_rust.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <memory>
#include <sstream>

#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "aidl_to_cpp_common.h"
#include "aidl_to_rust.h"
#include "code_writer.h"
#include "logging.h"

using android::base::Join;
using std::ostringstream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace aidl {
namespace rust {

static constexpr const char kArgumentPrefix[] = "_arg_";
static constexpr const char kGetInterfaceVersion[] = "getInterfaceVersion";
static constexpr const char kGetInterfaceHash[] = "getInterfaceHash";

void GenerateMangledAlias(CodeWriter& out, const AidlDefinedType* type) {
  ostringstream alias;
  for (const auto& component : type->GetSplitPackage()) {
    alias << "_" << component.size() << "_" << component;
  }
  alias << "_" << type->GetName().size() << "_" << type->GetName();
  out << "pub(crate) mod mangled { pub use super::" << type->GetName() << " as " << alias.str()
      << "; }\n";
}

string BuildArg(const AidlArgument& arg, const AidlTypenames& typenames) {
  // We pass in parameters that are not primitives by const reference.
  // Arrays get passed in as slices, which is handled in RustNameOf.
  auto arg_mode = ArgumentStorageMode(arg, typenames);
  auto arg_type = RustNameOf(arg.GetType(), typenames, arg_mode);
  return kArgumentPrefix + arg.GetName() + ": " + arg_type;
}

string BuildMethod(const AidlMethod& method, const AidlTypenames& typenames) {
  auto method_type = RustNameOf(method.GetType(), typenames, StorageMode::VALUE);
  auto return_type = string{"binder::public_api::Result<"} + method_type + ">";
  string parameters = "&self";
  for (const std::unique_ptr<AidlArgument>& arg : method.GetArguments()) {
    parameters += ", ";
    parameters += BuildArg(*arg, typenames);
  }
  return "fn " + method.GetName() + "(" + parameters + ") -> " + return_type;
}

void GenerateClientMethod(CodeWriter& out, const AidlMethod& method, const AidlTypenames& typenames,
                          const Options& options, const std::string& trait_name) {
  // Generate the method
  out << BuildMethod(method, typenames) << " {\n";
  out.Indent();

  if (!method.IsUserDefined()) {
    if (method.GetName() == kGetInterfaceVersion && options.Version() > 0) {
      // Check if the version is in the cache
      out << "let _aidl_version = "
             "self.cached_version.load(std::sync::atomic::Ordering::Relaxed);\n";
      out << "if _aidl_version != -1 { return Ok(_aidl_version); }\n";
    }

    if (method.GetName() == kGetInterfaceHash && !options.Hash().empty()) {
      out << "{\n";
      out << "  let _aidl_hash_lock = self.cached_hash.lock().unwrap();\n";
      out << "  if let Some(ref _aidl_hash) = *_aidl_hash_lock {\n";
      out << "    return Ok(_aidl_hash.clone());\n";
      out << "  }\n";
      out << "}\n";
    }
  }

  // Call transact()
  auto transact_flags = method.IsOneway() ? "binder::SpIBinder::FLAG_ONEWAY" : "0";
  out << "let _aidl_reply = self.binder.transact("
      << "transactions::" << method.GetName() << ", " << transact_flags << ", |_aidl_data| {\n";
  out.Indent();

  // Arguments
  for (const std::unique_ptr<AidlArgument>& arg : method.GetArguments()) {
    auto arg_name = kArgumentPrefix + arg->GetName();
    if (arg->IsIn()) {
      // If the argument is already a reference, don't reference it again
      // (unless we turned it into an Option<&T>)
      auto ref_mode = ArgumentReferenceMode(*arg, typenames);
      if (IsReference(ref_mode)) {
        out << "_aidl_data.write(" << arg_name << ")?;\n";
      } else {
        out << "_aidl_data.write(&" << arg_name << ")?;\n";
      }
    } else if (arg->GetType().IsArray()) {
      // For out-only arrays, send the array size
      if (arg->GetType().IsNullable()) {
        out << "_aidl_data.write_slice_size(" << arg_name << ".as_deref())?;\n";
      } else {
        out << "_aidl_data.write_slice_size(Some(" << arg_name << "))?;\n";
      }
    }
  }

  // Return Ok(()) if all the `_aidl_data.write(...)?;` calls pass
  out << "Ok(())\n";
  out.Dedent();
  out << "});\n";

  // Check for UNKNOWN_TRANSACTION and call the default impl
  string default_args;
  for (const std::unique_ptr<AidlArgument>& arg : method.GetArguments()) {
    if (!default_args.empty()) {
      default_args += ", ";
    }
    default_args += kArgumentPrefix;
    default_args += arg->GetName();
  }
  out << "if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {\n";
  out << "  if let Some(_aidl_default_impl) = <Self as " << trait_name << ">::getDefaultImpl() {\n";
  out << "    return _aidl_default_impl." << method.GetName() << "(" << default_args << ");\n";
  out << "  }\n";
  out << "}\n";

  // Return all other errors
  out << "let _aidl_reply = _aidl_reply?;\n";

  string return_val = "()";
  if (!method.IsOneway()) {
    // Check for errors
    out << "let _aidl_status: binder::Status = _aidl_reply.read()?;\n";
    out << "if !_aidl_status.is_ok() { return Err(_aidl_status); }\n";

    // Return reply value
    if (method.GetType().GetName() != "void") {
      auto return_type = RustNameOf(method.GetType(), typenames, StorageMode::VALUE);
      out << "let _aidl_return: " << return_type << " = _aidl_reply.read()?;\n";
      return_val = "_aidl_return";

      if (!method.IsUserDefined()) {
        if (method.GetName() == kGetInterfaceVersion && options.Version() > 0) {
          out << "self.cached_version.store(_aidl_return, std::sync::atomic::Ordering::Relaxed);\n";
        }
        if (method.GetName() == kGetInterfaceHash && !options.Hash().empty()) {
          out << "*self.cached_hash.lock().unwrap() = Some(_aidl_return.clone());\n";
        }
      }
    }

    for (const AidlArgument* arg : method.GetOutArguments()) {
      out << "*" << kArgumentPrefix << arg->GetName() << " = _aidl_reply.read()?;\n";
    }
  }

  // Return the result
  out << "Ok(" << return_val << ")\n";
  out.Dedent();
  out << "}\n";
}

void GenerateServerTransaction(CodeWriter& out, const AidlMethod& method,
                               const AidlTypenames& typenames) {
  out << "transactions::" << method.GetName() << " => {\n";
  out.Indent();

  string args;
  for (const auto& arg : method.GetArguments()) {
    string arg_name = kArgumentPrefix + arg->GetName();
    StorageMode arg_mode;
    if (arg->IsIn()) {
      arg_mode = StorageMode::VALUE;
    } else {
      // We need a value we can call Default::default() on
      arg_mode = StorageMode::DEFAULT_VALUE;
    }
    auto arg_type = RustNameOf(arg->GetType(), typenames, arg_mode);

    string arg_mut = arg->IsOut() ? "mut " : "";
    string arg_init = arg->IsIn() ? "_aidl_data.read()?" : "Default::default()";
    out << "let " << arg_mut << arg_name << ": " << arg_type << " = " << arg_init << ";\n";
    if (!arg->IsIn() && arg->GetType().IsArray()) {
      // _aidl_data.resize_[nullable_]out_vec(&mut _arg_foo)?;
      auto resize_name = arg->GetType().IsNullable() ? "resize_nullable_out_vec" : "resize_out_vec";
      out << "_aidl_data." << resize_name << "(&mut " << arg_name << ")?;\n";
    }

    auto ref_mode = ArgumentReferenceMode(*arg, typenames);
    if (!args.empty()) {
      args += ", ";
    }
    args += TakeReference(ref_mode, arg_name);
  }
  out << "let _aidl_return = _aidl_service." << method.GetName() << "(" << args << ");\n";

  if (!method.IsOneway()) {
    out << "match &_aidl_return {\n";
    out.Indent();
    out << "Ok(_aidl_return) => {\n";
    out.Indent();
    out << "_aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;\n";
    if (method.GetType().GetName() != "void") {
      out << "_aidl_reply.write(_aidl_return)?;\n";
    }

    // Serialize out arguments
    for (const AidlArgument* arg : method.GetOutArguments()) {
      string arg_name = kArgumentPrefix + arg->GetName();

      auto& arg_type = arg->GetType();
      if (!arg->IsIn() && arg_type.IsArray() && arg_type.GetName() == "ParcelFileDescriptor") {
        // We represent arrays of ParcelFileDescriptor as
        // Vec<Option<ParcelFileDescriptor>> when they're out-arguments,
        // but we need all of them to be initialized to Some; if there's
        // any None, return UNEXPECTED_NULL (this is what libbinder_ndk does)
        out << "if " << arg_name << ".iter().any(Option::is_none) { "
            << "return Err(binder::StatusCode::UNEXPECTED_NULL); }\n";
      } else if (!arg->IsIn() && !TypeHasDefault(arg_type, typenames)) {
        // Unwrap out-only arguments that we wrapped in Option<T>
        out << "let " << arg_name << " = " << arg_name
            << ".ok_or(binder::StatusCode::UNEXPECTED_NULL)?;\n";
      }

      out << "_aidl_reply.write(&" << arg_name << ")?;\n";
    }
    out.Dedent();
    out << "}\n";
    out << "Err(_aidl_status) => _aidl_reply.write(_aidl_status)?\n";
    out.Dedent();
    out << "}\n";
  }
  out << "Ok(())\n";
  out.Dedent();
  out << "}\n";
}

void GenerateServerItems(CodeWriter& out, const AidlInterface* iface,
                         const AidlTypenames& typenames) {
  auto trait_name = ClassName(*iface, cpp::ClassNames::INTERFACE);
  auto server_name = ClassName(*iface, cpp::ClassNames::SERVER);

  // Forward all IFoo functions from Binder to the inner object
  out << "impl " << trait_name << " for binder::Binder<" << server_name << "> {\n";
  out.Indent();
  for (const auto& method : iface->GetMethods()) {
    string args;
    for (const std::unique_ptr<AidlArgument>& arg : method->GetArguments()) {
      if (!args.empty()) {
        args += ", ";
      }
      args += kArgumentPrefix;
      args += arg->GetName();
    }
    out << BuildMethod(*method, typenames) << " { "
        << "self.0." << method->GetName() << "(" << args << ") }\n";
  }
  out.Dedent();
  out << "}\n";

  out << "fn on_transact("
         "_aidl_service: &dyn "
      << trait_name
      << ", "
         "_aidl_code: binder::TransactionCode, "
         "_aidl_data: &binder::parcel::Parcel, "
         "_aidl_reply: &mut binder::parcel::Parcel) -> binder::Result<()> {\n";
  out.Indent();
  out << "match _aidl_code {\n";
  out.Indent();
  for (const auto& method : iface->GetMethods()) {
    GenerateServerTransaction(out, *method, typenames);
  }
  out << "_ => Err(binder::StatusCode::UNKNOWN_TRANSACTION)\n";
  out.Dedent();
  out << "}\n";
  out.Dedent();
  out << "}\n";
}

bool GenerateRustInterface(const string& filename, const AidlInterface* iface,
                           const AidlTypenames& typenames, const IoDelegate& io_delegate,
                           const Options& options) {
  CodeWriterPtr code_writer = io_delegate.GetCodeWriter(filename);

  *code_writer << "#![allow(non_upper_case_globals)]\n";
  *code_writer << "#![allow(non_snake_case)]\n";
  // Import IBinder for transact()
  *code_writer << "#[allow(unused_imports)] use binder::IBinder;\n";

  auto trait_name = ClassName(*iface, cpp::ClassNames::INTERFACE);
  auto client_name = ClassName(*iface, cpp::ClassNames::CLIENT);
  auto server_name = ClassName(*iface, cpp::ClassNames::SERVER);
  *code_writer << "use binder::declare_binder_interface;\n";
  *code_writer << "declare_binder_interface! {\n";
  code_writer->Indent();
  *code_writer << trait_name << "[\"" << iface->GetDescriptor() << "\"] {\n";
  code_writer->Indent();
  *code_writer << "native: " << server_name << "(on_transact),\n";
  *code_writer << "proxy: " << client_name << " {\n";
  code_writer->Indent();
  if (options.Version() > 0) {
    string comma = options.Hash().empty() ? "" : ",";
    *code_writer << "cached_version: "
                    "std::sync::atomic::AtomicI32 = "
                    "std::sync::atomic::AtomicI32::new(-1)"
                 << comma << "\n";
  }
  if (!options.Hash().empty()) {
    *code_writer << "cached_hash: "
                    "std::sync::Mutex<Option<String>> = "
                    "std::sync::Mutex::new(None)\n";
  }
  code_writer->Dedent();
  *code_writer << "},\n";
  code_writer->Dedent();
  *code_writer << "}\n";
  code_writer->Dedent();
  *code_writer << "}\n";

  *code_writer << "pub trait " << trait_name << ": binder::Interface + Send {\n";
  code_writer->Indent();
  *code_writer << "fn get_descriptor() -> &'static str where Self: Sized { \""
               << iface->GetDescriptor() << "\" }\n";

  for (const auto& method : iface->GetMethods()) {
    // Generate the method
    *code_writer << BuildMethod(*method, typenames) << " {\n";
    code_writer->Indent();
    if (method->IsUserDefined()) {
      // Return Err(UNKNOWN_TRANSACTION) by default
      *code_writer << "Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())\n";
    } else {
      // Generate default implementations for meta methods
      // FIXME: is this fine, or do we want to leave the defaults out
      // and force users to implement them manually (or with a helper macro we
      // provide) on the server side?
      if (method->GetName() == kGetInterfaceVersion && options.Version() > 0) {
        *code_writer << "Ok(VERSION)\n";
      } else if (method->GetName() == kGetInterfaceHash && !options.Hash().empty()) {
        *code_writer << "Ok(HASH.into())\n";
      }
    }
    code_writer->Dedent();
    *code_writer << "}\n";
  }

  // Emit the default implementation code inside the trait
  auto default_name = ClassName(*iface, cpp::ClassNames::DEFAULT_IMPL);
  *code_writer << "fn getDefaultImpl()"
               << " -> " << default_name << " where Self: Sized {\n";
  *code_writer << "  DEFAULT_IMPL.lock().unwrap().clone()\n";
  *code_writer << "}\n";
  *code_writer << "fn setDefaultImpl(d: " << default_name << ")"
               << " -> " << default_name << " where Self: Sized {\n";
  *code_writer << "  std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)\n";
  *code_writer << "}\n";
  code_writer->Dedent();
  *code_writer << "}\n";

  // Generate the transaction code constants
  // The constants get their own sub-module to avoid conflicts
  *code_writer << "pub mod transactions {\n";
  code_writer->Indent();
  // Import IBinder so we can access FIRST_CALL_TRANSACTION
  *code_writer << "#[allow(unused_imports)] use binder::IBinder;\n";
  for (const auto& method : iface->GetMethods()) {
    // Generate the transaction code constant
    *code_writer << "pub const " << method->GetName()
                 << ": binder::TransactionCode = "
                    "binder::SpIBinder::FIRST_CALL_TRANSACTION + " +
                        std::to_string(method->GetId()) + ";\n";
  }
  code_writer->Dedent();
  *code_writer << "}\n";

  // Emit the default implementation code outside the trait
  *code_writer << "pub type " << default_name << " = Option<std::sync::Arc<dyn " << trait_name
               << " + Sync>>;\n";
  *code_writer << "use lazy_static::lazy_static;\n";
  *code_writer << "lazy_static! {\n";
  *code_writer << "  static ref DEFAULT_IMPL: std::sync::Mutex<" << default_name
               << "> = std::sync::Mutex::new(None);\n";
  *code_writer << "}\n";

  // Emit the interface constants
  for (const auto& constant : iface->GetConstantDeclarations()) {
    const AidlConstantValue& value = constant->GetValue();
    string const_type;
    switch (value.GetType()) {
      case AidlConstantValue::Type::STRING: {
        const_type = "&str";
        break;
      }
      case AidlConstantValue::Type::BOOLEAN:  // fall-through
      case AidlConstantValue::Type::INT8:     // fall-through
      case AidlConstantValue::Type::INT32: {
        const_type = "i32";
        break;
      }
      default: {
        AIDL_FATAL(value) << "Unrecognized constant type: " << static_cast<int>(value.GetType());
      }
    }
    *code_writer << "pub const " << constant->GetName() << ": " << const_type << " = "
                 << constant->ValueString(ConstantValueDecoratorRef) << ";\n";
  }

  GenerateMangledAlias(*code_writer, iface);

  // Emit VERSION and HASH
  // These need to be top-level item constants instead of associated consts
  // because the latter are incompatible with trait objects, see
  // https://doc.rust-lang.org/reference/items/traits.html#object-safety
  if (options.Version() > 0) {
    *code_writer << "pub const VERSION: i32 = " << std::to_string(options.Version()) << ";\n";
  }
  if (!options.Hash().empty()) {
    *code_writer << "pub const HASH: &str = \"" << options.Hash() << "\";\n";
  }

  // Generate the client-side methods
  *code_writer << "impl " << trait_name << " for " << client_name << " {\n";
  code_writer->Indent();
  for (const auto& method : iface->GetMethods()) {
    GenerateClientMethod(*code_writer, *method, typenames, options, trait_name);
  }
  code_writer->Dedent();
  *code_writer << "}\n";

  // Generate the server-side methods
  GenerateServerItems(*code_writer, iface, typenames);

  return true;
}

void GenerateParcelDefault(CodeWriter& out, const AidlStructuredParcelable* parcel) {
  out << "impl Default for " << parcel->GetName() << " {\n";
  out.Indent();
  out << "fn default() -> Self {\n";
  out.Indent();
  out << "Self {\n";
  out.Indent();
  for (const auto& variable : parcel->GetFields()) {
    if (variable->GetDefaultValue()) {
      out << variable->GetName() << ": " << variable->ValueString(ConstantValueDecorator) << ",\n";
    } else {
      out << variable->GetName() << ": Default::default(),\n";
    }
  }
  out.Dedent();
  out << "}\n";
  out.Dedent();
  out << "}\n";
  out.Dedent();
  out << "}\n";
}

void GenerateParcelSerialize(CodeWriter& out, const AidlStructuredParcelable* parcel,
                             const AidlTypenames& typenames) {
  out << "impl binder::parcel::Serialize for " << parcel->GetName() << " {\n";
  out << "  fn serialize(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {\n";
  out << "    <Self as binder::parcel::SerializeOption>::serialize_option(Some(self), parcel)\n";
  out << "  }\n";
  out << "}\n";

  out << "impl binder::parcel::SerializeArray for " << parcel->GetName() << " {}\n";

  out << "impl binder::parcel::SerializeOption for " << parcel->GetName() << " {\n";
  out.Indent();
  out << "fn serialize_option(this: Option<&Self>, parcel: &mut binder::parcel::Parcel) -> "
         "binder::Result<()> {\n";
  out.Indent();
  out << "let this = if let Some(this) = this {\n";
  out << "  parcel.write(&1i32)?;\n";
  out << "  this\n";
  out << "} else {\n";
  out << "  return parcel.write(&0i32);\n";
  out << "};\n";
  out << "let start_pos = parcel.get_data_position();\n";
  out << "parcel.write(&0i32)?;\n";
  for (const auto& variable : parcel->GetFields()) {
    if (!TypeHasDefault(variable->GetType(), typenames)) {
      out << "let __field_ref = this." << variable->GetName()
          << ".as_ref().ok_or(binder::StatusCode::UNEXPECTED_NULL)?;\n";
      out << "parcel.write(__field_ref)?;\n";
    } else {
      out << "parcel.write(&this." << variable->GetName() << ")?;\n";
    }
  }
  out << "let end_pos = parcel.get_data_position();\n";
  out << "let parcelable_size = (end_pos - start_pos) as i32;\n";
  out << "unsafe { parcel.set_data_position(start_pos)?; }\n";
  out << "parcel.write(&parcelable_size)?;\n";
  out << "unsafe { parcel.set_data_position(end_pos)?; }\n";
  out << "Ok(())\n";
  out.Dedent();
  out << "}\n";
  out.Dedent();
  out << "}\n";
}

void GenerateParcelDeserialize(CodeWriter& out, const AidlStructuredParcelable* parcel,
                               const AidlTypenames& typenames) {
  out << "impl binder::parcel::Deserialize for " << parcel->GetName() << " {\n";
  out << "  fn deserialize(parcel: &binder::parcel::Parcel) -> binder::Result<Self> {\n";
  out << "    <Self as binder::parcel::DeserializeOption>::deserialize_option(parcel)\n";
  out << "       .transpose()\n";
  out << "       .unwrap_or(Err(binder::StatusCode::UNEXPECTED_NULL))\n";
  out << "  }\n";
  out << "}\n";

  out << "impl binder::parcel::DeserializeArray for " << parcel->GetName() << " {}\n";

  out << "impl binder::parcel::DeserializeOption for " << parcel->GetName() << " {\n";
  out << "  fn deserialize_option(parcel: &binder::parcel::Parcel) -> binder::Result<Option<Self>> "
         "{\n";
  out << "    let status: i32 = parcel.read()?;\n";
  out << "    if status == 0 { return Ok(None); }\n";
  out << "    let start_pos = parcel.get_data_position();\n";
  out << "    let parcelable_size: i32 = parcel.read()?;\n";
  out << "    if parcelable_size < 0 { return Err(binder::StatusCode::BAD_VALUE); }\n";

  // Pre-emit the common field epilogue code, shared between all fields:
  ostringstream epilogue;
  epilogue << "    if (parcel.get_data_position() - start_pos) == parcelable_size {\n";
  // We assume the lhs can never be > parcelable_size, because then the read
  // immediately preceding this check would have returned NOT_ENOUGH_DATA
  epilogue << "      return Ok(Some(result));\n";
  epilogue << "    }\n";
  string epilogue_str = epilogue.str();

  out << "    let mut result = Self::default();\n";
  for (const auto& variable : parcel->GetFields()) {
    if (!TypeHasDefault(variable->GetType(), typenames)) {
      out << "    result." << variable->GetName() << " = Some(parcel.read()?);\n";
    } else {
      out << "    result." << variable->GetName() << " = parcel.read()?;\n";
    }
    out << epilogue_str;
  }

  out << "    Ok(Some(result))\n";
  out << "  }\n";
  out << "}\n";
}

bool GenerateRustParcel(const string& filename, const AidlStructuredParcelable* parcel,
                        const AidlTypenames& typenames, const IoDelegate& io_delegate) {
  CodeWriterPtr code_writer = io_delegate.GetCodeWriter(filename);

  // Debug is always derived because all Rust AIDL types implement it
  // ParcelFileDescriptor doesn't support any of the others because
  // it's a newtype over std::fs::File which only implements Debug
  vector<string> derives{"Debug"};
  const AidlAnnotation* derive_annotation = parcel->RustDerive();
  if (derive_annotation != nullptr) {
    for (const auto& name_and_param : derive_annotation->AnnotationParams(ConstantValueDecorator)) {
      if (name_and_param.second == "true") {
        derives.push_back(name_and_param.first);
      }
    }
  }

  *code_writer << "#[derive(" << Join(derives, ", ") << ")]\n";
  *code_writer << "pub struct " << parcel->GetName() << " {\n";
  code_writer->Indent();
  for (const auto& variable : parcel->GetFields()) {
    auto field_type = RustNameOf(variable->GetType(), typenames, StorageMode::PARCELABLE_FIELD);
    *code_writer << "pub " << variable->GetName() << ": " << field_type << ", \n";
  }
  code_writer->Dedent();
  *code_writer << "}\n";

  GenerateMangledAlias(*code_writer, parcel);
  GenerateParcelDefault(*code_writer, parcel);
  GenerateParcelSerialize(*code_writer, parcel, typenames);
  GenerateParcelDeserialize(*code_writer, parcel, typenames);

  return true;
}

bool GenerateRustEnumDeclaration(const string& filename, const AidlEnumDeclaration* enum_decl,
                                 const AidlTypenames& typenames, const IoDelegate& io_delegate) {
  CodeWriterPtr code_writer = io_delegate.GetCodeWriter(filename);

  const auto& aidl_backing_type = enum_decl->GetBackingType();
  auto backing_type = RustNameOf(aidl_backing_type, typenames, StorageMode::VALUE);

  *code_writer << "#![allow(non_upper_case_globals)]\n";
  *code_writer << "use binder::declare_binder_enum;\n";
  *code_writer << "declare_binder_enum! { " << enum_decl->GetName() << " : " << backing_type
               << " {\n";
  code_writer->Indent();
  for (const auto& enumerator : enum_decl->GetEnumerators()) {
    auto value = enumerator->GetValue()->ValueString(aidl_backing_type, ConstantValueDecorator);
    *code_writer << enumerator->GetName() << " = " << value << ",\n";
  }
  code_writer->Dedent();
  *code_writer << "} }\n";

  GenerateMangledAlias(*code_writer, enum_decl);

  return true;
}

bool GenerateRust(const string& filename, const AidlDefinedType* defined_type,
                  const AidlTypenames& typenames, const IoDelegate& io_delegate,
                  const Options& options) {
  if (const AidlStructuredParcelable* parcelable = defined_type->AsStructuredParcelable();
      parcelable != nullptr) {
    return GenerateRustParcel(filename, parcelable, typenames, io_delegate);
  }

  if (const AidlEnumDeclaration* enum_decl = defined_type->AsEnumDeclaration();
      enum_decl != nullptr) {
    return GenerateRustEnumDeclaration(filename, enum_decl, typenames, io_delegate);
  }

  if (const AidlInterface* interface = defined_type->AsInterface(); interface != nullptr) {
    return GenerateRustInterface(filename, interface, typenames, io_delegate, options);
  }

  AIDL_FATAL(filename) << "Unrecognized type sent for Rust generation.";
  return false;
}

}  // namespace rust
}  // namespace aidl
}  // namespace android
