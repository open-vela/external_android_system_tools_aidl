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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "aidl.h"
#include "parser.h"
#include "tests/fake_io_delegate.h"

using android::aidl::AidlError;
using android::aidl::AidlTypenames;
using android::aidl::DiagnosticID;
using android::aidl::DiagnosticsContext;
using android::aidl::kDiagnosticsNames;
using android::aidl::Options;
using android::aidl::internals::load_and_validate_aidl;
using android::aidl::test::FakeIoDelegate;
using testing::internal::CaptureStderr;
using testing::internal::GetCapturedStderr;

struct DiagnosticsTest : testing::Test {
  void ParseFiles(std::vector<std::pair<std::string, std::string>>&& files) {
    ASSERT_TRUE(files.size() > 0);
    const std::string main = files.begin()->first;
    for (const auto& [file, contents] : files) {
      io.SetFileContents(file, contents);
    }
    // emit diagnostics as warnings.
    // "java" has no specific meaning here because we're testing CheckValid()
    const Options options = Options::From("aidl -I . --lang java -o out -Weverything " + main);
    CaptureStderr();
    EXPECT_EQ(AidlError::OK, load_and_validate_aidl(main, options, io, &typenames, nullptr));
    const std::string err = GetCapturedStderr();
    if (expect_diagnostics.empty()) {
      EXPECT_EQ("", err);
    } else {
      for (const auto id : expect_diagnostics) {
        EXPECT_THAT(err, testing::HasSubstr("-W" + kDiagnosticsNames.at(id)));
      }
    }
  }

  AidlTypenames typenames;
  FakeIoDelegate io;
  std::vector<DiagnosticID> expect_diagnostics;
};

TEST_F(DiagnosticsTest, interface_name) {
  expect_diagnostics = {DiagnosticID::interface_name};
  ParseFiles({{"Foo.aidl", "interface Foo { }"}});
}

TEST_F(DiagnosticsTest, enum_zero) {
  expect_diagnostics = {DiagnosticID::enum_zero};
  ParseFiles({{"Enum.aidl", "enum Enum { A = 1 }"}});
}

TEST_F(DiagnosticsTest, inout_parameter) {
  expect_diagnostics = {DiagnosticID::inout_parameter};
  ParseFiles({{"IFoo.aidl", "interface IFoo { void foo(inout Bar bar); }"},
              {"Bar.aidl", "parcelable Bar {}"}});
}
