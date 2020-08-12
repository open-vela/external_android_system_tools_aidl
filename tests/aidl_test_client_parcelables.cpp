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

#include <vector>

using android::IInterface;
using android::sp;
using android::String16;
using android::aidl::tests::ConstantExpressionEnum;
using android::aidl::tests::GenericStructuredParcelable;
using android::aidl::tests::INamedCallback;
using android::aidl::tests::IntEnum;
using android::aidl::tests::ITestService;
using android::aidl::tests::SimpleParcelable;
using android::aidl::tests::StructuredParcelable;
using android::binder::Status;
using android::os::PersistableBundle;
using std::vector;

TEST_F(AidlTest, RepeatSimpleParcelable) {
  SimpleParcelable input("Booya", 42);
  SimpleParcelable out_param, returned;
  Status status = service->RepeatSimpleParcelable(input, &out_param, &returned);
  ASSERT_TRUE(status.isOk()) << status.toString8();
  EXPECT_EQ(input, out_param);
  EXPECT_EQ(input, returned);
}

TEST_F(AidlTest, RepeatGenericStructureParcelable) {
  GenericStructuredParcelable<int32_t, StructuredParcelable, IntEnum> input, out_param, returned;
  input.a = 41;
  input.b = 42;
  Status status = service->RepeatGenericParcelable(input, &out_param, &returned);
  ASSERT_TRUE(status.isOk()) << status.toString8();
  EXPECT_EQ(input, out_param);
  EXPECT_EQ(input, returned);
}

TEST_F(AidlTest, ReverseSimpleParcelable) {
  const vector<SimpleParcelable> original{SimpleParcelable("first", 0),
                                          SimpleParcelable("second", 1),
                                          SimpleParcelable("third", 2)};
  vector<SimpleParcelable> repeated;
  vector<SimpleParcelable> reversed;
  Status status = service->ReverseSimpleParcelables(original, &repeated, &reversed);
  ASSERT_TRUE(status.isOk()) << status.toString8();

  EXPECT_EQ(repeated, original);

  std::reverse(reversed.begin(), reversed.end());
}

TEST_F(AidlTest, ConfirmPersistableBundles) {
  PersistableBundle empty_bundle, returned;
  Status status = service->RepeatPersistableBundle(empty_bundle, &returned);
  ASSERT_TRUE(status.isOk()) << status.toString8();
  EXPECT_EQ(empty_bundle, returned);
}

TEST_F(AidlTest, ConfirmPersistableBundlesNonEmpty) {
  PersistableBundle non_empty_bundle, returned;
  non_empty_bundle.putBoolean(String16("test_bool"), false);
  non_empty_bundle.putInt(String16("test_int"), 33);
  non_empty_bundle.putLong(String16("test_long"), 34359738368L);
  non_empty_bundle.putDouble(String16("test_double"), 1.1);
  non_empty_bundle.putString(String16("test_string"), String16("Woot!"));
  non_empty_bundle.putBooleanVector(String16("test_bool_vector"),
                                    {true, false, true});
  non_empty_bundle.putIntVector(String16("test_int_vector"), {33, 44, 55, 142});
  non_empty_bundle.putLongVector(String16("test_long_vector"),
                                 {34L, 8371L, 34359738375L});
  non_empty_bundle.putDoubleVector(String16("test_double_vector"), {2.2, 5.4});
  non_empty_bundle.putStringVector(String16("test_string_vector"),
                                   {String16("hello"), String16("world!")});
  PersistableBundle nested_bundle;
  nested_bundle.putInt(String16("test_nested_int"), 345);
  non_empty_bundle.putPersistableBundle(String16("test_persistable_bundle"),
                                        nested_bundle);

  Status status = service->RepeatPersistableBundle(non_empty_bundle, &returned);
  ASSERT_TRUE(status.isOk()) << status.toString8();
  EXPECT_EQ(non_empty_bundle, returned);
}

TEST_F(AidlTest, ReversePersistableBundles) {
  PersistableBundle first;
  PersistableBundle second;
  PersistableBundle third;
  first.putInt(String16("test_int"), 1231);
  second.putLong(String16("test_long"), 222222L);
  third.putDouble(String16("test_double"), 10.8);
  const vector<PersistableBundle> original{first, second, third};

  vector<PersistableBundle> repeated;
  vector<PersistableBundle> reversed;
  Status status = service->ReversePersistableBundles(original, &repeated, &reversed);
  ASSERT_TRUE(status.isOk()) << status.toString8();

  EXPECT_EQ(repeated, original);

  std::reverse(reversed.begin(), reversed.end());
  EXPECT_EQ(reversed, original);
}

TEST_F(AidlTest, StructuredParcelableEquality) {
  // TODO: break up equality tests, these are hard to read, because you need to
  // keep the state of the parcelables in mind
  StructuredParcelable parcelable1;
  StructuredParcelable parcelable2;

  parcelable1.f = 11;
  parcelable2.f = 11;

  service->FillOutStructuredParcelable(&parcelable1);
  service->FillOutStructuredParcelable(&parcelable2);

  sp<INamedCallback> callback1;
  sp<INamedCallback> callback2;
  service->GetOtherTestService(String16("callback1"), &callback1);
  service->GetOtherTestService(String16("callback2"), &callback2);

  parcelable1.ibinder = IInterface::asBinder(callback1);
  parcelable2.ibinder = IInterface::asBinder(callback1);

  EXPECT_EQ(parcelable1, parcelable2);

  parcelable1.f = 0;
  EXPECT_LT(parcelable1, parcelable2);
  parcelable1.f = 11;

  parcelable1.shouldBeJerry = "Jarry";
  EXPECT_LT(parcelable1, parcelable2);
  parcelable1.shouldBeJerry = "Jerry";

  parcelable2.shouldContainThreeFs = {};
  EXPECT_GT(parcelable1, parcelable2);
  parcelable2.shouldContainThreeFs = {parcelable2.f, parcelable2.f, parcelable2.f};

  parcelable2.shouldBeIntBar = IntEnum::FOO;
  EXPECT_GT(parcelable1, parcelable2);
  parcelable2.shouldBeIntBar = IntEnum::BAR;

  parcelable2.ibinder = IInterface::asBinder(callback2);
  EXPECT_NE(parcelable1, parcelable2);
}

TEST_F(AidlTest, ConfirmStructuredParcelables) {
  constexpr int kDesiredValue = 23;

  StructuredParcelable parcelable;
  parcelable.f = kDesiredValue;

  EXPECT_EQ(parcelable.stringDefaultsToFoo, String16("foo"));
  EXPECT_EQ(parcelable.byteDefaultsToFour, 4);
  EXPECT_EQ(parcelable.intDefaultsToFive, 5);
  EXPECT_EQ(parcelable.longDefaultsToNegativeSeven, -7);
  EXPECT_EQ(parcelable.booleanDefaultsToTrue, true);
  EXPECT_EQ(parcelable.charDefaultsToC, 'C');
  EXPECT_TRUE(parcelable.floatDefaultsToPi == 3.14f) << parcelable.floatDefaultsToPi;
  EXPECT_TRUE(parcelable.doubleWithDefault == -3.14e17) << parcelable.doubleWithDefault;

  EXPECT_EQ(parcelable.boolDefault, false);
  EXPECT_EQ(parcelable.byteDefault, 0);
  EXPECT_EQ(parcelable.intDefault, 0);
  EXPECT_EQ(parcelable.longDefault, 0);
  EXPECT_EQ(parcelable.floatDefault, 0.0f);
  EXPECT_EQ(parcelable.doubleDefault, 0.0);

  ASSERT_EQ(parcelable.arrayDefaultsTo123.size(), 3u);
  EXPECT_EQ(parcelable.arrayDefaultsTo123[0], 1);
  EXPECT_EQ(parcelable.arrayDefaultsTo123[1], 2);
  EXPECT_EQ(parcelable.arrayDefaultsTo123[2], 3);
  EXPECT_TRUE(parcelable.arrayDefaultsToEmpty.empty());

  service->FillOutStructuredParcelable(&parcelable);

  ASSERT_EQ(parcelable.shouldContainThreeFs.size(), 3u);
  EXPECT_EQ(parcelable.shouldContainThreeFs[0], kDesiredValue);
  EXPECT_EQ(parcelable.shouldContainThreeFs[1], kDesiredValue);
  EXPECT_EQ(parcelable.shouldContainThreeFs[2], kDesiredValue);

  EXPECT_EQ(parcelable.shouldBeJerry, "Jerry");
  EXPECT_EQ(parcelable.int32_min, INT32_MIN);
  EXPECT_EQ(parcelable.int32_max, INT32_MAX);
  EXPECT_EQ(parcelable.int64_max, INT64_MAX);
  EXPECT_EQ(parcelable.hexInt32_neg_1, -1);

  for (size_t ndx = 0; ndx < parcelable.int32_1.size(); ndx++) {
    EXPECT_EQ(parcelable.int32_1[ndx], 1) << ndx;
  }

  for (size_t ndx = 0; ndx < parcelable.int64_1.size(); ndx++) {
    EXPECT_EQ(parcelable.int64_1[ndx], 1) << ndx;
  }

  EXPECT_EQ(parcelable.hexInt32_pos_1, 1);
  EXPECT_EQ(parcelable.hexInt64_pos_1, 1);

  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_1), 1);
  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_2), 1);
  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_3), 1);
  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_4), 1);
  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_5), 1);
  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_6), 1);
  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_7), 1);
  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_8), 1);
  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_9), 1);
  EXPECT_EQ(static_cast<int>(parcelable.const_exprs_10), 1);

  EXPECT_EQ(parcelable.addString1, "hello world!");
  EXPECT_EQ(parcelable.addString2, "The quick brown fox jumps over the lazy dog.");
}
