/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "aidl_test_client.h"

#include "aidl_test_client_defaultimpl.h"
#include "aidl_test_client_file_descriptors.h"
#include "aidl_test_client_nullables.h"
#include "aidl_test_client_parcelables.h"
#include "aidl_test_client_primitives.h"
#include "aidl_test_client_service_exceptions.h"
#include "aidl_test_client_utf8_strings.h"

#include <android-base/logging.h>

// These tests were written without gtest, and not all have been converted to
// gtest. Places which haven't been converted are still included as part of this
// test here.
TEST_F(AidlTest, UnconvertedTests) {
  namespace client_tests = android::aidl::tests::client;

  EXPECT_TRUE(client_tests::ConfirmPrimitiveRepeat(service));
  EXPECT_TRUE(client_tests::ConfirmReverseArrays(service));
  EXPECT_TRUE(client_tests::ConfirmReverseLists(service));
  EXPECT_TRUE(client_tests::ConfirmReverseBinderLists(service));
  EXPECT_TRUE(client_tests::ConfirmSimpleParcelables(service));
  EXPECT_TRUE(client_tests::ConfirmPersistableBundles(service));
  EXPECT_TRUE(client_tests::ConfirmIntfConstantExpressions(service));
  EXPECT_TRUE(client_tests::ConfirmStructuredParcelables(service));
  EXPECT_TRUE(client_tests::ConfirmStructuredParcelablesEquality(service));
  EXPECT_TRUE(client_tests::ConfirmFileDescriptors(service));
  EXPECT_TRUE(client_tests::ConfirmFileDescriptorArrays(service));
  EXPECT_TRUE(client_tests::ConfirmParcelFileDescriptors(service));
  EXPECT_TRUE(client_tests::ConfirmParcelFileDescriptorArrays(service));
  EXPECT_TRUE(client_tests::ConfirmServiceSpecificExceptions(service));
  EXPECT_TRUE(client_tests::ConfirmNullables(service));
  EXPECT_TRUE(client_tests::ConfirmUtf8InCppStringRepeat(service));
  EXPECT_TRUE(client_tests::ConfirmUtf8InCppStringArrayReverse(service));
  EXPECT_TRUE(client_tests::ConfirmUtf8InCppStringListReverse(service));
  EXPECT_TRUE(client_tests::ConfirmDefaultImpl(service));
}

int main(int argc, char* argv[]) {
  android::base::InitLogging(argv, android::base::StderrLogger);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
