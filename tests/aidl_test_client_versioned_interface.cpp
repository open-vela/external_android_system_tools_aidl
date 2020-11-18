/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <android/aidl/versioned/tests/IFooInterface.h>
#include <binder/IServiceManager.h>
#include <gtest/gtest.h>
#include <utils/String16.h>

using android::OK;
using android::sp;
using android::String16;
using android::aidl::versioned::tests::BazUnion;
using android::aidl::versioned::tests::IFooInterface;

class VersionedInterfaceTest : public testing::Test {
 public:
  void SetUp() override {
    ASSERT_EQ(OK, android::getService(IFooInterface::descriptor, &service));
    ASSERT_NE(nullptr, service);
  }

  sp<IFooInterface> service;
};

TEST_F(VersionedInterfaceTest, getInterfaceVersion) {
  EXPECT_EQ(1, service->getInterfaceVersion());
}

TEST_F(VersionedInterfaceTest, getInterfaceHash) {
  EXPECT_EQ("796b4ab269d476662bed4ab57092ed000e48d5d7", service->getInterfaceHash());
}

TEST_F(VersionedInterfaceTest, noProblemWhenPassingAUnionWithOldField) {
  std::string result;
  auto status = service->acceptUnionAndReturnString(BazUnion::make<BazUnion::intNum>(42), &result);
  EXPECT_TRUE(status.isOk());
  EXPECT_EQ("42", result);
}

TEST_F(VersionedInterfaceTest, errorWhenPassingAUnionWithNewField) {
  std::string result;
  auto status =
      service->acceptUnionAndReturnString(BazUnion::make<BazUnion::longNum>(42L), &result);
  EXPECT_FALSE(status.isOk());
  EXPECT_EQ(::android::BAD_VALUE, status.transactionError());
}
