/*
 * Copyright (C) 2019, The Android Open Source Project
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
#include "aidl_typenames.h"
#include "io_delegate.h"
#include "options.h"

#include <memory>
#include <string>
#include <vector>

struct yy_buffer_state;
typedef yy_buffer_state* YY_BUFFER_STATE;

class Parser {
 public:
  // non-copyable, non-assignable
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;

  ~Parser();

  // Parse contents of file |filename|. Should only be called once.
  static std::unique_ptr<Parser> Parse(const std::string& filename,
                                       const android::aidl::IoDelegate& io_delegate,
                                       AidlTypenames& typenames);

  void AddError() { error_++; }
  bool HasError() { return error_ != 0; }

  const std::string& FileName() const { return filename_; }
  void* Scanner() const { return scanner_; }

  void SetPackage(unique_ptr<AidlQualifiedName> name) { package_ = std::move(name); }
  std::vector<std::string> Package() const;

  void DeferResolution(AidlTypeSpecifier* typespec) {
    unresolved_typespecs_.emplace_back(typespec);
  }

  const vector<AidlTypeSpecifier*>& GetUnresolvedTypespecs() const { return unresolved_typespecs_; }

  bool Resolve();
  void SetDocument(std::unique_ptr<AidlDocument>&& document) {
    // AidlDocument does not have the ownership to AidlDefinedTypes.
    // AidlTypenames has the ownership.
    // TODO(jiyong): fix this by making AidlTypenames to own
    // AidlDocuments and AidlDocument to own AidlDefinedTypes.
    for (auto* t : document->DefinedTypes()) {
      if (!typenames_.AddDefinedType(std::unique_ptr<AidlDefinedType>(t))) {
        AddError();
      }
    }
    document_ = std::move(document);
  }

  const AidlDocument& Document() const { return *document_; }

 private:
  explicit Parser(const std::string& filename, std::string& raw_buffer,
                  android::aidl::AidlTypenames& typenames);

  std::string filename_;
  std::unique_ptr<AidlQualifiedName> package_;
  AidlTypenames& typenames_;

  void* scanner_ = nullptr;
  YY_BUFFER_STATE buffer_;
  int error_ = 0;

  vector<AidlTypeSpecifier*> unresolved_typespecs_;
  std::unique_ptr<AidlDocument> document_;
};
