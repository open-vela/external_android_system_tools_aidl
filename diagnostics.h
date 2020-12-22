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
#pragma once

#include <map>
#include <string>
#include <vector>

class AidlLocation;
class AidlErrorLog;

namespace android {
namespace aidl {

enum class DiagnosticSeverity {
  DISABLED,
  WARNING,
  ERROR,
};

enum class DiagnosticID {
#define DIAG(ENUM, NAME, ENABLED) ENUM,

#include "diagnostics.inc"
#undef DIAG
};

class DiagnosticsContext {
 public:
  virtual ~DiagnosticsContext() {}
  // Returns true if it's okay to proceed after reporting diagnostics.
  virtual AidlErrorLog Report(const AidlLocation& loc, DiagnosticID id) = 0;
};

struct DiagnosticOption {
  DiagnosticID id;
  const std::string name;
  bool default_enabled;
};

extern const std::map<std::string, DiagnosticOption> kAllDiagnostics;

}  // namespace aidl
}  // namespace android