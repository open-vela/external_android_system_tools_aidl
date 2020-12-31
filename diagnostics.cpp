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

#include <functional>
#include <stack>

#include "aidl_language.h"
#include "logging.h"

using std::placeholders::_1;

namespace android {
namespace aidl {

const std::map<std::string, DiagnosticOption> kAllDiagnostics = {
#define DIAG(ENUM, NAME, ENABLED) {NAME, DiagnosticOption{DiagnosticID::ENUM, NAME, ENABLED}},
#include "diagnostics.inc"
#undef DIAG
};

static const std::map<DiagnosticID, std::string> kDiagnosticsNames = {
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

std::string to_string(DiagnosticID id) {
  return kDiagnosticsNames.at(id);
}

class DiagnosticsContext {
 public:
  DiagnosticsContext(DiagnosticMapping mapping) : mapping_({std::move(mapping)}) {}
  AidlErrorLog Report(const AidlLocation& loc, DiagnosticID id,
                      DiagnosticSeverity force_severity = DiagnosticSeverity::DISABLED) {
    const std::string suffix = " [-W" + to_string(id) + "]";
    auto severity = std::max(force_severity, mapping_.top().Severity(id));
    switch (severity) {
      case DiagnosticSeverity::DISABLED:
        return AidlErrorLog(AidlErrorLog::NO_OP, loc);
      case DiagnosticSeverity::WARNING:
        return AidlErrorLog(AidlErrorLog::WARNING, loc, suffix);
      case DiagnosticSeverity::ERROR:
        error_count_++;
        return AidlErrorLog(AidlErrorLog::ERROR, loc, suffix);
    }
  }
  size_t ErrorCount() const { return error_count_; }
  void Suppress(const AidlAnnotatable& a) {
    const auto& warnings = a.SuppressWarnings();
    DiagnosticMapping new_mapping = mapping_.top();
    for (const auto& w : warnings) {
      auto it = kAllDiagnostics.find(w);
      if (it == kAllDiagnostics.end()) {
        Report(a.GetLocation(), DiagnosticID::unknown_warning, DiagnosticSeverity::ERROR)
            << "unknown warning: " << w;
        continue;
      }
      new_mapping.Severity(it->second.id, DiagnosticSeverity::DISABLED);
    }
    mapping_.push(std::move(new_mapping));
  }
  void Restore(const AidlAnnotatable&) {
    mapping_.pop();
  }
 private:
  std::stack<DiagnosticMapping> mapping_;
  size_t error_count_ = {};
};

class DiagnosticsVisitor : public AidlVisitor {
 public:
  DiagnosticsVisitor(DiagnosticsContext& diag) : diag(diag) {}
  void Check(const AidlDocument& doc) {
    DiagnosticsVisitor* visitor = this;
    using Fun = std::function<void(const AidlAnnotatable&)>;
    struct Hook : public AidlVisitor {
      Fun fun;
      Hook(Fun fun) : fun(fun) {}
      void Visit(const AidlInterface& a) override { fun(a); }
      void Visit(const AidlEnumDeclaration& a) override { fun(a); }
      void Visit(const AidlStructuredParcelable& a) override { fun(a); }
      void Visit(const AidlUnionDecl& a) override { fun(a); }
      void Visit(const AidlParcelable& a) override { fun(a); }
      void Visit(const AidlMethod& a) override { fun(a.GetType()); }
    };
    Hook suppress{std::bind(&DiagnosticsContext::Suppress, &diag, _1)};
    Hook restore{std::bind(&DiagnosticsContext::Restore, &diag, _1)};
    std::function<void(const AidlTraversable&)> topDown =
        [&topDown, &suppress, &restore, visitor](const AidlTraversable& a) {
      a.DispatchVisit(suppress);
      a.DispatchVisit(*visitor);
      a.TraverseChildren(topDown);
      a.DispatchVisit(restore);
    };
    topDown(doc);
  }
 protected:
  DiagnosticsContext& diag;
};

struct DiagnoseInterfaceName : DiagnosticsVisitor {
  DiagnoseInterfaceName(DiagnosticsContext& diag) : DiagnosticsVisitor(diag) {}
  void Visit(const AidlInterface& i) override {
    if (auto name = i.GetName(); name.size() < 1 || name[0] != 'I') {
      diag.Report(i.GetLocation(), DiagnosticID::interface_name)
          << "Interface names should start with I.";
    }
  }
};

struct DiagnoseEnumZero : DiagnosticsVisitor {
  DiagnoseEnumZero(DiagnosticsContext& diag) : DiagnosticsVisitor(diag) {}
  void Visit(const AidlEnumDeclaration& e) override {
    AIDL_FATAL_IF(e.GetEnumerators().empty(), e)
        << "The enum '" << e.GetName() << "' has no enumerators.";
    const auto& first = e.GetEnumerators()[0];
    if (auto first_value = first->ValueString(e.GetBackingType(), AidlConstantValueDecorator);
        first_value != "0") {
      diag.Report(first->GetLocation(), DiagnosticID::enum_zero)
          << "The first enumerator '" << first->GetName() << "' should be 0, but it is "
          << first_value << ".";
    }
  }
};

struct DiagnoseInoutParameter : DiagnosticsVisitor {
  DiagnoseInoutParameter(DiagnosticsContext& diag) : DiagnosticsVisitor(diag) {}
  void Visit(const AidlArgument& a) override {
    if (a.GetDirection() == AidlArgument::INOUT_DIR) {
      diag.Report(a.GetLocation(), DiagnosticID::inout_parameter)
          << a.GetName()
          << " is 'inout'. Avoid inout parameters. This is somewhat confusing for clients "
             "because although the parameters are 'in', they look out 'out' parameters.";
    }
  }
};

struct DiagnoseConstName : DiagnosticsVisitor {
  DiagnoseConstName(DiagnosticsContext& diag) : DiagnosticsVisitor(diag) {}
  void Visit(const AidlEnumerator& e) override {
    if (ToUpper(e.GetName()) != e.GetName()) {
      diag.Report(e.GetLocation(), DiagnosticID::const_name)
          << "Enum values should be named in upper cases: " << ToUpper(e.GetName());
    }
  }
  void Visit(const AidlConstantDeclaration& c) override {
    if (ToUpper(c.GetName()) != c.GetName()) {
      diag.Report(c.GetLocation(), DiagnosticID::const_name)
          << "Constants should be named in upper cases: " << ToUpper(c.GetName());
    }
  }
  static std::string ToUpper(std::string name) {
    for (auto& c : name) c = std::toupper(c);
    return name;
  }
};

bool Diagnose(const AidlDocument& doc, const DiagnosticMapping& mapping) {
  DiagnosticsContext diag(mapping);

  DiagnoseInterfaceName{diag}.Check(doc);
  DiagnoseEnumZero{diag}.Check(doc);
  DiagnoseInoutParameter{diag}.Check(doc);
  DiagnoseConstName{diag}.Check(doc);

  return diag.ErrorCount() == 0;
}
}  // namespace aidl
}  // namespace android
