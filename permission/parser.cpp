/*
 * Copyright (C) 2021, The Android Open Source Project
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

#include <iostream>
#include <string>

#include "android-base/result.h"
#include "android-base/strings.h"

#include "grammar.h"

struct yy_buffer_state;
typedef yy_buffer_state* YY_BUFFER_STATE;
typedef void* yyscan_t;
int permlex_init(yyscan_t* scanner);
void permlex_destroy(void*);
YY_BUFFER_STATE perm_scan_buffer(char*, size_t, void*);
void perm_delete_buffer(YY_BUFFER_STATE, void*);

using android::base::Error;
using android::base::Result;

namespace perm {

Parser::Parser(std::string& raw_buffer) {
  /* Flex requires two null bytes at the end of the input */
  raw_buffer.append(2u, '\0');

  permlex_init(&scanner_);
  buffer_ = perm_scan_buffer(&raw_buffer[0], raw_buffer.length(), scanner_);
}

Parser::~Parser() {
  perm_delete_buffer(buffer_, scanner_);
  permlex_destroy(scanner_);
}

Result<std::unique_ptr<Expression>> Parser::Parse(std::string s) {
  Parser parser(s);
  auto p = perm::parser(&parser);

  if (p.parse() != 0) {
    return Error() << "Unable to parse the expression: " << parser.GetErrorMessage();
  }
  return std::unique_ptr<Expression>(parser.GetRoot());
}

std::string Parser::GetErrorMessage() const {
  return android::base::Join(errors_, ", ");
}

void Quantifier::Append(std::unique_ptr<Expression> expr) {
  exprs_.push_back(std::move(expr));
}

std::string Quantifier::String(const std::string& separator) const {
  std::string ret;
  for (size_t i = 0; i < exprs_.size(); i++) {
    ret += exprs_[i]->ToString();
    if (i != exprs_.size() - 1) ret += separator;
  }
  return ret;
}

std::string AndQuantifier::ToString() const {
  return Quantifier::String(" && ");
}

std::string OrQuantifier::ToString() const {
  return Quantifier::String(" || ");
}

std::string Predicate::GetTypeAsString() const {
  if (type_ == Type::kPermission) {
    return "permission";
  } else if (type_ == Type::kUid) {
    return "uid";
  } else {
    return "unknown type";
  }
}

std::string Predicate::ToString() const {
  return GetTypeAsString() + " = " + value_;
}

}  // namespace perm
