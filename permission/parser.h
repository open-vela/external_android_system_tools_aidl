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

#pragma once

#include <string>
#include <vector>

#include "android-base/result.h"

struct yy_buffer_state;
typedef yy_buffer_state* YY_BUFFER_STATE;

using android::base::Result;

namespace perm {

// Base class.
class Expression {
 public:
  virtual std::string ToString() const = 0;
  virtual ~Expression(){};
};

class Quantifier : public Expression {
 public:
  void Append(std::unique_ptr<Expression> expr);

 protected:
  std::string String(const std::string& separator) const;
  std::vector<std::unique_ptr<Expression>> exprs_;
};

class AndQuantifier : public Quantifier {
  std::string ToString() const override;
};

class OrQuantifier : public Quantifier {
  std::string ToString() const override;
};

// A predicate on an attribute. For instance, "permission = ACCESS_FINE_LOCATION".
class Predicate : public Expression {
 public:
  enum class Type { kPermission, kUid };

  Predicate(Type type, const std::string& value) : type_(type), value_(value) {}

  Type GetType() const { return type_; }
  std::string GetTypeAsString() const;
  std::string ToString() const override;

 private:
  const Type type_;
  const std::string value_;
};

// Parser for permission expressions. Use the factory method Parse.
// Example:
//    auto result = perm::Parser::Parse("permission = ABC");
//    if (result.ok()) {
//      std::cout << result.value()->ToString() << std::endl;
//    }
class Parser {
 public:
  // non-copyable, non-assignable
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;

  ~Parser();

  void* Scanner() const { return scanner_; }
  void SetRoot(Expression* root) { root_ = root; }
  void AddError(const std::string& error) { errors_.push_back(error); }

  static Result<std::unique_ptr<Expression>> Parse(std::string s);

 private:
  explicit Parser(std::string& raw_buffer);
  Expression* GetRoot() const { return root_; }
  std::string GetErrorMessage() const;

  void* scanner_ = nullptr;
  YY_BUFFER_STATE buffer_;
  Expression* root_ = nullptr;
  std::vector<std::string> errors_;
};

}  // namespace perm
