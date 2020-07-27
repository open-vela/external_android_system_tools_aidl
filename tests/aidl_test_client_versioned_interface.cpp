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

class VersionedInterfaceTest : public testing::Test {
 public:
  void SetUp() override {
    constexpr char name[] = "android.aidl.versioned.tests.IFooInterface";
    ASSERT_EQ(OK, android::getService(String16(name), &service));
    ASSERT_NE(nullptr, service);
  }

  sp<android::aidl::versioned::tests::IFooInterface> service;
};

TEST_F(VersionedInterfaceTest, getInterfaceVersion) {
  EXPECT_EQ(1, service->getInterfaceVersion());
}

TEST_F(VersionedInterfaceTest, getInterfaceHash) {
  EXPECT_EQ("fcd4f9c806cbc8af3694d569fd1de1ecc8cf7d22", service->getInterfaceHash());
}
