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
#include "parser.h"
#include "grammar.h"
#define YYSTYPE PERMSTYPE

#include <iostream>
%}

%option reentrant
%option warn
%option noyywrap
%option noinput
%option nounput
%option bison-bridge
%option prefix="perm"

identifier  [_a-zA-Z0-9]+
whitespace  ([ \t\r]+)

%%

{whitespace}          {}

 /* operators */
"="                   { return '='; }
"&&"                  { return perm::parser::token::LOGICAL_AND; }
"||"                  { return perm::parser::token::LOGICAL_OR;  }

 /* keywords */
permission            { return perm::parser::token::PERMISSION;  }
uid                   { return perm::parser::token::UID; }

{identifier}          { yylval->s = new std::string(yytext);
                        return perm::parser::token::STRING; };

