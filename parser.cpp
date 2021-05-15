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

#include "parser.h"
#include "aidl_language_y.h"
#include "logging.h"

void yylex_init(void**);
void yylex_destroy(void*);
void yyset_in(FILE* f, void*);
int yyparse(Parser*);
YY_BUFFER_STATE yy_scan_buffer(char*, size_t, void*);
void yy_delete_buffer(YY_BUFFER_STATE, void*);

std::unique_ptr<Parser> Parser::Parse(const std::string& filename,
                                      const android::aidl::IoDelegate& io_delegate,
                                      AidlTypenames& typenames) {
  // Make sure we can read the file first, before trashing previous state.
  unique_ptr<string> raw_buffer = io_delegate.GetFileContents(filename);
  if (raw_buffer == nullptr) {
    AIDL_ERROR(filename) << "Error while opening file for parsing";
    return nullptr;
  }

  // We're going to scan this buffer in place, and yacc demands we put two
  // nulls at the end.
  raw_buffer->append(2u, '\0');

  std::unique_ptr<Parser> parser(new Parser(filename, *raw_buffer, typenames));

  if (yy::parser(parser.get()).parse() != 0 || parser->HasError()) {
    return nullptr;
  }

  return parser;
}

void Parser::SetTypeParameters(AidlTypeSpecifier* type,
                               std::vector<std::unique_ptr<AidlTypeSpecifier>>* type_args) {
  if (type->IsArray()) {
    AIDL_ERROR(type) << "Must specify type parameters (<>) before array ([]).";
    AddError();
  }
  if (!type->SetTypeParameters(type_args)) {
    AIDL_ERROR(type) << "Can only specify one set of type parameters.";
    AddError();
    delete type_args;
  }
}

class ReferenceResolver : public AidlVisitor {
 public:
  ReferenceResolver(const AidlDefinedType* scope, const AidlTypenames& typenames,
                    TypeResolver& resolver, bool* success)
      : scope_(scope), typenames_(typenames), resolver_(resolver), success_(success) {}

  void Visit(const AidlTypeSpecifier& t) override {
    AidlTypeSpecifier& type = const_cast<AidlTypeSpecifier&>(t);
    if (!resolver_(typenames_.GetDocumentFor(scope_), &type)) {
      AIDL_ERROR(type) << "Failed to resolve '" << type.GetUnresolvedName() << "'";
      *success_ = false;
    }
  }

  void Visit(const AidlConstantReference& v) override {
    if (IsCircularReference(&v)) {
      *success_ = false;
      return;
    }

    const AidlConstantValue* resolved = v.Resolve(scope_);
    if (!resolved) {
      AIDL_ERROR(v) << "Unknown reference '" << v.Literal() << "'";
      *success_ = false;
      return;
    }

    // resolve recursive references
    Push(&v);
    VisitBottomUp(*this, *resolved);
    Pop();
  }

 private:
  struct StackElem {
    const AidlDefinedType* scope;
    const AidlConstantReference* ref;
  };

  void Push(const AidlConstantReference* ref) {
    stack_.push_back({scope_, ref});
    if (ref->GetRefType()) {
      scope_ = ref->GetRefType()->GetDefinedType();
    }
  }

  void Pop() {
    scope_ = stack_.back().scope;
    stack_.pop_back();
  }

  bool IsCircularReference(const AidlConstantReference* ref) {
    auto it = std::find_if(stack_.begin(), stack_.end(),
                           [&](const auto& elem) { return elem.ref == ref; });
    if (it == stack_.end()) {
      return false;
    }
    std::vector<std::string> path;
    while (it != stack_.end()) {
      path.push_back(it->ref->Literal());
      ++it;
    }
    path.push_back(ref->Literal());
    AIDL_ERROR(ref) << "Found a circular reference: " << android::base::Join(path, " -> ");
    return true;
  }

  const AidlDefinedType* scope_;
  const AidlTypenames& typenames_;
  TypeResolver& resolver_;
  bool* success_;
  std::vector<StackElem> stack_ = {};
};

// Resolve "unresolved" types in the "main" document.
bool Parser::Resolve(TypeResolver& type_resolver) {
  bool success = true;

  for (const auto& type : document_->DefinedTypes()) {
    ReferenceResolver ref_resolver{type.get(), typenames_, type_resolver, &success};
    VisitBottomUp(ref_resolver, *type);
  }

  return success;
}

Parser::Parser(const std::string& filename, std::string& raw_buffer,
               android::aidl::AidlTypenames& typenames)
    : filename_(filename), typenames_(typenames) {
  yylex_init(&scanner_);
  buffer_ = yy_scan_buffer(&raw_buffer[0], raw_buffer.length(), scanner_);
}

Parser::~Parser() {
  yy_delete_buffer(buffer_, scanner_);
  yylex_destroy(scanner_);
}
