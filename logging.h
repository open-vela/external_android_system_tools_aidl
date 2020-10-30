/*
 * Copyright (C) 2015, The Android Open Source Project
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

#include <iostream>

// Generic point for printing any error in the AIDL compiler.
class AidlErrorLog {
 public:
  AidlErrorLog(bool fatal, const AidlLocation& location)
      : os_(std::cerr), fatal_(fatal), location_(location) {
    sHadError = true;

    os_ << "ERROR: ";
    os_ << location << ": ";
  }
  AidlErrorLog(bool fatal, const std::string& filename)
      : AidlErrorLog(fatal, AidlLocation(filename, AidlLocation::Source::EXTERNAL)) {}
  AidlErrorLog(bool fatal, const AidlNode& node) : AidlErrorLog(fatal, node.location_) {}
  AidlErrorLog(bool fatal, const AidlNode* node) : AidlErrorLog(fatal, *node) {}

  template <typename T>
  AidlErrorLog(bool fatal, const std::unique_ptr<T>& node) : AidlErrorLog(fatal, *node) {}
  ~AidlErrorLog() {
    os_ << std::endl;
    if (fatal_) abort();
    if (location_.IsInternal()) {
      os_ << "Logging an internal location should not happen. Offending location: " << location_
          << std::endl;
      abort();
    }
  }

  // AidlErrorLog is a single use object. No need to copy or move
  AidlErrorLog(const AidlErrorLog&) = delete;
  AidlErrorLog(AidlErrorLog&&) = delete;
  AidlErrorLog& operator=(const AidlErrorLog&) = delete;
  AidlErrorLog& operator=(AidlErrorLog&&) = delete;

  std::ostream& os_;

  static void clearError() { sHadError = false; }
  static bool hadError() { return sHadError; }

 private:
  bool fatal_;
  const AidlLocation location_;
  static bool sHadError;
};

// A class used to make it obvious to clang that code is going to abort. This
// informs static analyses of the aborting behavior of `AIDL_FATAL`, and
// helps generate slightly faster/smaller code.
class AidlAbortOnDestruction {
 public:
  __attribute__((noreturn)) ~AidlAbortOnDestruction() { abort(); }
};

#define AIDL_ERROR(CONTEXT) ::AidlErrorLog(false /*fatal*/, (CONTEXT)).os_
#define AIDL_FATAL(CONTEXT) \
  (::AidlAbortOnDestruction(), ::AidlErrorLog(true /*fatal*/, (CONTEXT)).os_)
#define AIDL_FATAL_IF(CONDITION, CONTEXT) \
  if (CONDITION) AIDL_FATAL(CONTEXT) << "Bad internal state: " << #CONDITION << ": "
