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
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <aidl/android/aidl/tests/ITestService.h>

using aidl::android::aidl::tests::INamedCallback;
using aidl::android::aidl::tests::ITestService;
using testing::Eq;

struct AidlTest : testing::Test {
  template <typename T>
  std::shared_ptr<T> getService() {
    ndk::SpAIBinder binder = ndk::SpAIBinder(AServiceManager_getService(T::descriptor));
    return T::fromBinder(binder);
  }
};

TEST_F(AidlTest, InterfaceExchange) {
  auto service = getService<ITestService>();
  std::vector<std::string> names = {"Larry", "Curly", "Moe"};

  for (int i = 0; i < 3; i++) {
    std::shared_ptr<INamedCallback> got;
    ASSERT_TRUE(service->GetOtherTestService(names[i], &got).isOk());
    std::string name;
    ASSERT_TRUE(got->GetName(&name).isOk());
    ASSERT_THAT(name, Eq(names[i]));
  }
}
