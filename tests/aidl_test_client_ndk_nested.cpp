/*
 * Copyright (C) 2021 The Android Open Source Project
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

#include <android/binder_auto_utils.h>
#include <android/binder_manager.h>
#include <binder/IServiceManager.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <utils/String16.h>

#include <aidl/android/aidl/tests/BackendType.h>
#include <aidl/android/aidl/tests/ITestService.h>
#include <aidl/android/aidl/tests/nested/BpNestedService.h>
#include <aidl/android/aidl/tests/nested/INestedService.h>

using aidl::android::aidl::tests::nested::INestedService;
using aidl::android::aidl::tests::nested::ParcelableWithNested;
using NestedResult = aidl::android::aidl::tests::nested::INestedService::Result;
using NestedStatus = aidl::android::aidl::tests::nested::ParcelableWithNested::Status;
using aidl::android::aidl::tests::BackendType;
using aidl::android::aidl::tests::ITestService;
using std::shared_ptr;
using std::vector;
using testing::Eq;

struct AidlTest : testing::Test {
  template <typename T>
  std::shared_ptr<T> getService() {
    ndk::SpAIBinder binder = ndk::SpAIBinder(AServiceManager_getService(T::descriptor));
    return T::fromBinder(binder);
  }
};

TEST_F(AidlTest, NestedService) {
  BackendType backendType;
  auto status = getService<ITestService>()->getBackendType(&backendType);
  EXPECT_TRUE(status.isOk());
  // TODO(b/201729533) enable test when Rust backend supports nested types
  if (backendType == BackendType::RUST) GTEST_SKIP();

  auto nestedService = getService<INestedService>();
  ASSERT_NE(nullptr, nestedService);

  ParcelableWithNested p;
  p.status = NestedStatus::OK;
  NestedResult r;
  // OK -> NOT_OK
  status = nestedService->flipStatus(p, &r);
  EXPECT_TRUE(status.isOk());
  EXPECT_EQ(r.status, NestedStatus::NOT_OK);

  // android::enum_ranges<>
  vector<NestedStatus> values{ndk::enum_range<NestedStatus>().begin(),
                              ndk::enum_range<NestedStatus>().end()};
  EXPECT_EQ(values, vector<NestedStatus>({NestedStatus::OK, NestedStatus::NOT_OK}));

  // toString()
  EXPECT_EQ(toString(NestedStatus::NOT_OK), "NOT_OK");
}
