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
#include "diagnostics.h"

#include "logging.h"

namespace android {
namespace aidl {

const std::map<std::string, DiagnosticOption> kAllDiagnostics = {
#define DIAG(ENUM, NAME, ENABLED) {NAME, DiagnosticOption{DiagnosticID::ENUM, NAME, ENABLED}},
#include "diagnostics.inc"
#undef DIAG
};

const std::map<DiagnosticID, std::string> kDiagnosticsNames = {
#define DIAG(ENUM, NAME, ENABLED) {DiagnosticID::ENUM, NAME},
#include "diagnostics.inc"
#undef DIAG
};

void DiagnosticMapping::Severity(DiagnosticID id, DiagnosticSeverity severity) {
  mapping_[id] = severity;
}

DiagnosticSeverity DiagnosticMapping::Severity(DiagnosticID id) const {
  return mapping_.at(id);
}

AidlErrorLog DiagnosticsContext::Report(const AidlLocation& loc, DiagnosticID id) {
  return DoReport(loc, id, mapping_.Severity(id));
}

}  // namespace aidl
}  // namespace android