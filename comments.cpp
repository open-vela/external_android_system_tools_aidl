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
#include "comments.h"

#include <android-base/result.h>
#include <android-base/strings.h>

#include <optional>
#include <string>
#include <vector>

#include "logging.h"

using android::base::EndsWith;
using android::base::Error;
using android::base::Join;
using android::base::Result;
using android::base::Split;
using android::base::StartsWith;
using android::base::Trim;

namespace android {
namespace aidl {

namespace {

static const std::string_view kLineCommentBegin = "//";
static const std::string_view kBlockCommentBegin = "/*";
static const std::string_view kBlockCommentEnd = "*/";

std::string ConsumePrefix(const std::string& s, std::string_view prefix) {
  AIDL_FATAL_IF(!StartsWith(s, prefix), AIDL_LOCATION_HERE);
  return s.substr(prefix.size());
}

std::string ConsumeSuffix(const std::string& s, std::string_view suffix) {
  AIDL_FATAL_IF(!EndsWith(s, suffix), AIDL_LOCATION_HERE);
  return s.substr(0, s.size() - suffix.size());
}

struct BlockTag {
  std::string name;
  std::string description;
};

struct Comments {
  enum class Type { LINE, BLOCK };
  Type type;
  std::string body;

  std::vector<std::string> TrimmedLines() const;
  std::vector<BlockTag> BlockTags() const;
};

// Removes comment markers: //, /*, /**, */, optional leading "*" in doc/block comments
// - keeps leading spaces, but trims trailing spaces
// - keeps empty lines
std::vector<std::string> Comments::TrimmedLines() const {
  if (type == Type::LINE) return std::vector{ConsumePrefix(body, kLineCommentBegin)};

  std::string stripped = ConsumeSuffix(ConsumePrefix(body, kBlockCommentBegin), kBlockCommentEnd);

  std::vector<std::string> lines;
  bool found_first_line = false;

  for (auto& line : Split(stripped, "\n")) {
    // Delete prefixes like "    * ", "   *", or "    ".
    size_t idx = 0;
    for (; idx < line.size() && isspace(line[idx]); idx++)
      ;
    if (idx < line.size() && line[idx] == '*') idx++;
    if (idx < line.size() && line[idx] == ' ') idx++;

    const std::string& sanitized_line = line.substr(idx);
    size_t i = sanitized_line.size();
    for (; i > 0 && isspace(sanitized_line[i - 1]); i--)
      ;

    // Either the size is 0 or everything was whitespace.
    bool is_empty_line = i == 0;

    found_first_line = found_first_line || !is_empty_line;
    if (!found_first_line) continue;

    // if is_empty_line, i == 0 so substr == ""
    lines.push_back(sanitized_line.substr(0, i));
  }
  // remove trailing empty lines
  while (!lines.empty() && Trim(lines.back()).empty()) {
    lines.pop_back();
  }
  return lines;
}

std::vector<BlockTag> Comments::BlockTags() const {
  std::vector<BlockTag> tags;

  // current tag and paragraph
  std::string tag;
  std::vector<std::string> paragraph;

  auto end_paragraph = [&]() {
    if (tag.empty()) {
      paragraph.clear();
      return;
    }
    // paragraph lines are trimed at both ends
    tags.push_back({tag, Join(paragraph, " ")});
    tag.clear();
    paragraph.clear();
  };

  for (const auto& line : TrimmedLines()) {
    size_t idx = 0;
    // skip leading spaces
    for (; idx < line.size() && isspace(line[idx]); idx++)
      ;

    if (idx == line.size()) {
      // skip empty lines
    } else if (line[idx] == '@') {
      // end the current paragraph before reading a new block tag (+ description paragraph)
      end_paragraph();

      size_t end_idx = idx + 1;
      for (; end_idx < line.size() && isalpha(line[end_idx]); end_idx++)
        ;

      tag = line.substr(idx, end_idx - idx);

      if (end_idx < line.size() && line[end_idx] == ' ') end_idx++;
      // skip empty line
      if (end_idx < line.size()) {
        paragraph.push_back(line.substr(end_idx));
      }
    } else {
      // gather paragraph lines with leading spaces trimmed
      paragraph.push_back(line.substr(idx));
    }
  }

  end_paragraph();

  return tags;
}

// TODO(b/177276676) remove this when comments are kept as parsed in AST
Result<std::vector<Comments>> ParseComments(const std::string& comments) {
  enum ParseState {
    INITIAL,
    SLASH,
    SLASHSLASH,
    SLASHSTAR,
    STAR,
  };
  ParseState st = INITIAL;
  std::string body;
  std::vector<Comments> result;
  for (const auto& c : comments) {
    switch (st) {
      case INITIAL:  // trim ws & newlines
        if (c == '/') {
          st = SLASH;
          body += c;
        } else if (std::isspace(c)) {
          // skip whitespaces outside comments
        } else {
          return Error() << "expecing / or space, but got unknown: " << c;
        }
        break;
      case SLASH:
        if (c == '/') {
          st = SLASHSLASH;
          body += c;
        } else if (c == '*') {
          st = SLASHSTAR;
          body += c;
        } else {
          return Error() << "expecting / or *, but got unknown: " << c;
        }
        break;
      case SLASHSLASH:
        if (c == '\n') {
          st = INITIAL;
          result.push_back({Comments::Type::LINE, std::move(body)});
          body.clear();
        } else {
          body += c;
        }
        break;
      case SLASHSTAR:
        body += c;
        if (c == '*') {
          st = STAR;
        }
        break;
      case STAR:  // read "*", about to close
        body += c;
        if (c == '/') {  // close!
          st = INITIAL;
          result.push_back({Comments::Type::BLOCK, std::move(body)});
          body.clear();
        } else if (c == '*') {
          // about to close...
        } else {
          st = SLASHSTAR;
        }
        break;
      default:
        return Error() << "unexpected state: " << st;
    }
  }
  return result;
}

}  // namespace

// Finds @deprecated tag and returns it with optional note which follows the tag.
std::optional<Deprecated> FindDeprecated(const std::string& comments) {
  auto result = ParseComments(comments);
  AIDL_FATAL_IF(!result.ok(), AIDL_LOCATION_HERE) << result.error();

  const std::string kTagDeprecated = "@deprecated";
  for (const auto& c : *result) {
    for (const auto& [name, description] : c.BlockTags()) {
      // take the first @deprecated
      if (kTagDeprecated == name) {
        return Deprecated{description};
      }
    }
  }
  return std::nullopt;
}

}  // namespace aidl
}  // namespace android