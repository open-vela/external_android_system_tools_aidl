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

#include <android/aidl/tests/extension/MyExt.h>
#include <android/aidl/tests/extension/MyExt2.h>
#include <android/aidl/tests/extension/MyExtLike.h>
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
using android::aidl::tests::Union;
using android::aidl::tests::extension::ExtendableParcelable;
using android::aidl::tests::extension::MyExt;
using android::aidl::tests::extension::MyExt2;
using android::aidl::tests::extension::MyExtLike;
using android::binder::Status;
using android::os::PersistableBundle;
using std::vector;

TEST_F(AidlTest, RepeatSimpleParcelable) {
  if (!cpp_java_tests) GTEST_SKIP() << "Service does not support the CPP/Java-only tests.";

  SimpleParcelable input("Booya", 42);
  SimpleParcelable out_param, returned;
  Status status = cpp_java_tests->RepeatSimpleParcelable(input, &out_param, &returned);
  ASSERT_TRUE(status.isOk()) << status.toString8();
  EXPECT_EQ(input, out_param);
  EXPECT_EQ(input, returned);
}

TEST_F(AidlTest, RepeatGenericStructureParcelable) {
  if (!cpp_java_tests) GTEST_SKIP() << "Service does not support the CPP/Java-only tests.";

  GenericStructuredParcelable<int32_t, StructuredParcelable, IntEnum> input, out_param, returned;
  input.a = 41;
  input.b = 42;
  Status status = cpp_java_tests->RepeatGenericParcelable(input, &out_param, &returned);
  ASSERT_TRUE(status.isOk()) << status.toString8();
  EXPECT_EQ(input, out_param);
  EXPECT_EQ(input, returned);
}

TEST_F(AidlTest, ReverseSimpleParcelable) {
  if (!cpp_java_tests) GTEST_SKIP() << "Service does not support the CPP/Java-only tests.";

  const vector<SimpleParcelable> original{SimpleParcelable("first", 0),
                                          SimpleParcelable("second", 1),
                                          SimpleParcelable("third", 2)};
  vector<SimpleParcelable> repeated;
  vector<SimpleParcelable> reversed;
  Status status = cpp_java_tests->ReverseSimpleParcelables(original, &repeated, &reversed);
  ASSERT_TRUE(status.isOk()) << status.toString8();

  EXPECT_EQ(repeated, original);

  std::reverse(reversed.begin(), reversed.end());
}

TEST_F(AidlTest, ConfirmPersistableBundles) {
  if (!cpp_java_tests) GTEST_SKIP() << "Service does not support the CPP/Java-only tests.";

  PersistableBundle empty_bundle, returned;
  Status status = cpp_java_tests->RepeatPersistableBundle(empty_bundle, &returned);
  ASSERT_TRUE(status.isOk()) << status.toString8();
  EXPECT_EQ(empty_bundle, returned);
}

TEST_F(AidlTest, ConfirmPersistableBundlesNonEmpty) {
  if (!cpp_java_tests) GTEST_SKIP() << "Service does not support the CPP/Java-only tests.";

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

  Status status = cpp_java_tests->RepeatPersistableBundle(non_empty_bundle, &returned);
  ASSERT_TRUE(status.isOk()) << status.toString8();
  EXPECT_EQ(non_empty_bundle, returned);
}

TEST_F(AidlTest, ReversePersistableBundles) {
  if (!cpp_java_tests) GTEST_SKIP() << "Service does not support the CPP/Java-only tests.";

  PersistableBundle first;
  PersistableBundle second;
  PersistableBundle third;
  first.putInt(String16("test_int"), 1231);
  second.putLong(String16("test_long"), 222222L);
  third.putDouble(String16("test_double"), 10.8);
  const vector<PersistableBundle> original{first, second, third};

  vector<PersistableBundle> repeated;
  vector<PersistableBundle> reversed;
  Status status = cpp_java_tests->ReversePersistableBundles(original, &repeated, &reversed);
  ASSERT_TRUE(status.isOk()) << status.toString8();

  EXPECT_EQ(repeated, original);

  std::reverse(reversed.begin(), reversed.end());
  EXPECT_EQ(reversed, original);
}

TEST_F(AidlTest, ReverseUnion) {
  if (!cpp_java_tests) GTEST_SKIP() << "Service does not support the CPP/Java-only tests.";

  Union original = Union::make<Union::ns>({1, 2, 3});
  Union repeated, reversed;
  Status status = cpp_java_tests->ReverseUnion(original, &repeated, &reversed);
  ASSERT_TRUE(status.isOk()) << status.toString8();

  EXPECT_EQ(repeated, original);

  std::reverse(reversed.get<Union::ns>().begin(), reversed.get<Union::ns>().end());
  EXPECT_EQ(reversed, original);
}

TEST_F(AidlTest, UnionUsage) {
  // make<tag>(...) to create a value for a tag.
  Union one_two_three = Union::make<Union::ns>({1, 2, 3});

  // getTag() queries the tag of the content
  EXPECT_EQ(Union::ns, one_two_three.getTag());

  // Ctor(...) works if a target tag has a unique type among fields.
  EXPECT_EQ(one_two_three, Union(std::vector{1, 2, 3}));
  EXPECT_EQ(one_two_three, std::vector<int>({1, 2, 3}));

  // Use std::in_place_index<tag> to avoid "move"
  // Note that make<tag>(...) involves "move" of the content value
  EXPECT_EQ(Union::make<Union::ns>(3, 0), Union(std::in_place_index<Union::ns>, 3, 0));

  Union one_two = one_two_three;
  // get<tag> can be used to modify the content
  one_two.get<Union::ns>().pop_back();
  EXPECT_EQ(one_two, std::vector<int>({1, 2}));
  // get<tag> can be lvalue
  one_two.get<Union::ns>() = std::vector<int>{1, 2};
  EXPECT_EQ(one_two, std::vector<int>({1, 2}));

  // abort with a bad access
  EXPECT_DEATH(one_two.get<Union::n>(), "");

  // set<tag>(...) overwrites the content with a new tag
  one_two_three.set<Union::s>("123");
  EXPECT_EQ(one_two_three, std::string("123"));

  // Or, you can simply assign a new value.
  // note that this works only if the target type is unique
  one_two_three = std::vector<std::string>{"1", "2", "3"};
  EXPECT_EQ(Union::ss, one_two_three.getTag());
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

TEST_F(AidlTest, EmptyParcelableHolder) {
  using namespace android::aidl::tests::extension;
  android::Parcel parcel;
  {
    ExtendableParcelable ep;
    ep.writeToParcel(&parcel);
    auto emptyExt = ep.ext.getParcelable<MyExt>();
    EXPECT_FALSE(emptyExt);
  }
  {
    parcel.setDataPosition(0);
    ExtendableParcelable ep;
    ep.readFromParcel(&parcel);
    auto emptyExt = ep.ext.getParcelable<MyExt>();
    EXPECT_FALSE(emptyExt);
  }
}

TEST_F(AidlTest, NativeExtednableParcelable) {
  using namespace android::aidl::tests::extension;
  MyExt ext;
  ext.a = 42;
  ext.b = "EXT";

  MyExt2 ext2;
  ext2.a = 42;
  ext2.b.a = 24;
  ext2.b.b = "INEXT";
  ext2.c = "EXT2";
  android::Parcel parcel;
  {
    ExtendableParcelable ep;
    ep.a = 1;
    ep.b = "a";
    ep.c = 42L;

    EXPECT_TRUE(ep.ext.setParcelable(ext));
    EXPECT_TRUE(ep.ext2.setParcelable(ext2));

    auto extLike = ep.ext.getParcelable<MyExtLike>();
    EXPECT_FALSE(extLike) << "The extension type must be MyExt, so it has to fail even though "
                             "MyExtLike has the same structure as MyExt.";

    auto actualExt = ep.ext.getParcelable<MyExt>();
    auto actualExt2 = ep.ext2.getParcelable<MyExt2>();

    EXPECT_TRUE(actualExt);
    EXPECT_TRUE(actualExt2);

    EXPECT_EQ(ext, *actualExt);
    EXPECT_EQ(ext2, *actualExt2);

    ep.writeToParcel(&parcel);
  }

  parcel.setDataPosition(0);
  {
    ExtendableParcelable ep;
    ep.readFromParcel(&parcel);

    auto extLike = ep.ext.getParcelable<MyExtLike>();
    EXPECT_FALSE(extLike) << "The extension type must be MyExt, so it has to fail even though "
                             "MyExtLike has the same structure as MyExt.";

    auto actualExt = ep.ext.getParcelable<MyExt>();
    auto actualExt2 = ep.ext2.getParcelable<MyExt2>();

    auto emptyExt = ep.ext2.getParcelable<MyExt>();
    EXPECT_FALSE(emptyExt);

    EXPECT_TRUE(actualExt);
    EXPECT_TRUE(actualExt2);

    EXPECT_EQ(ext, *actualExt);
    EXPECT_EQ(ext2, *actualExt2);
  }
}
