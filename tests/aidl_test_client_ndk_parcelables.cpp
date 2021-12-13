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

#include <aidl/android/aidl/loggable/ILoggableInterface.h>

#include <android/binder_auto_utils.h>
#include <android/binder_manager.h>
#include <gtest/gtest.h>

#include <aidl/android/aidl/tests/ITestService.h>
#include <aidl/android/aidl/tests/RecursiveList.h>

using aidl::android::aidl::tests::ITestService;
using aidl::android::aidl::tests::RecursiveList;

struct AidlTest : testing::Test {
  template <typename T>
  std::shared_ptr<T> getService() {
    ndk::SpAIBinder binder = ndk::SpAIBinder(AServiceManager_getService(T::descriptor));
    return T::fromBinder(binder);
  }
};

// TODO(b/196454897): copy more tests from aidl_test_client

TEST_F(AidlTest, ReverseRecursiveList) {
  std::unique_ptr<RecursiveList> head;
  for (int i = 0; i < 10; i++) {
    auto node = std::make_unique<RecursiveList>();
    node->value = i;
    node->next = std::move(head);
    head = std::move(node);
  }
  // head: [9, 8, ... 0]

  RecursiveList reversed;
  auto status = getService<ITestService>()->ReverseList(*head, &reversed);
  ASSERT_TRUE(status.isOk());

  // reversed should be [0, 1, .. 9]
  RecursiveList* cur = &reversed;
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(i, cur->value);
    cur = cur->next.get();
  }
  EXPECT_EQ(nullptr, cur);
}