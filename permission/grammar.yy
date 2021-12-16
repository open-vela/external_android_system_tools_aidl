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

%{
#include <stdlib.h>
#include "parser.h"
#include "grammar.h"

int permlex(perm::parser::semantic_type *, void *);

#define lex_scanner ps->Scanner()
%}

%define parse.error verbose
%define api.namespace {perm}
%define api.prefix {perm}
%glr-parser
%skeleton "glr.cc"

%parse-param { perm::Parser* ps}
%lex-param {void *lex_scanner}
%expect 0

%union {
  perm::Expression *expression;
  std::string *s;
}

%token<s> PERMISSION UID
%token<s> LOGICAL_AND LOGICAL_OR
%token<s> STRING

%left LOGICAL_OR
%left LOGICAL_AND

%type<expression> expr predicate

%destructor { delete $$; } predicate expr STRING

%%

input
  : expr {
    ps->SetRoot($1);
   };

predicate
  : PERMISSION '=' STRING {
    $$ = new perm::Predicate(perm::Predicate::Type::kPermission, *$3);
    delete $3;
   }
  | UID '=' STRING {
    $$ = new perm::Predicate(perm::Predicate::Type::kUid, *$3);
    delete $3;
   };

/* TODO: Support subexpressions using parentheses */
expr
  : predicate {
    $$ = $1;
   }
  | expr LOGICAL_OR expr {
    auto q = new perm::OrQuantifier();
    q->Append(std::move(std::unique_ptr<perm::Expression>($1)));
    q->Append(std::move(std::unique_ptr<perm::Expression>($3)));
    $$ = q;
   }
  | expr LOGICAL_AND expr {
    auto q = new perm::AndQuantifier();
    q->Append(std::move(std::unique_ptr<perm::Expression>($1)));
    q->Append(std::move(std::unique_ptr<perm::Expression>($3)));
    $$ = q;
   };

%%

void perm::parser::error(const std::string& errstr) {
  ps->AddError(errstr);
}
