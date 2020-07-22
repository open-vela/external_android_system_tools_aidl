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

#include "aidl_language_y-module.h"

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

bool Parser::Resolve() {
  bool success = true;
  for (AidlTypeSpecifier* typespec : unresolved_typespecs_) {
    if (!typespec->Resolve(typenames_)) {
      AIDL_ERROR(typespec) << "Failed to resolve '" << typespec->GetUnresolvedName() << "'";
      success = false;
      // don't stop to show more errors if any
    }
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
