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

#include <android/aidl/loggable/ILoggableInterface.h>

#include "aidl_test_client.h"

#include <android/aidl/loggable/BpLoggableInterface.h>
#include <android/aidl/tests/BackendType.h>
#include <binder/IServiceManager.h>
#include <binder/ParcelFileDescriptor.h>
#include <gtest/gtest.h>
#include <utils/String16.h>

using android::IBinder;
using android::sp;
using android::String16;
using android::aidl::loggable::BpLoggableInterface;
using android::aidl::loggable::Data;
using android::aidl::loggable::Enum;
using android::aidl::loggable::ILoggableInterface;
using android::aidl::loggable::Union;
using android::aidl::tests::BackendType;
using android::os::ParcelFileDescriptor;
using std::optional;
using std::string;
using std::vector;

TEST_F(AidlTest, LoggableInterface) {
  BackendType backendType;
  auto status = service->getBackendType(&backendType);
  EXPECT_TRUE(status.isOk());
  if (backendType != BackendType::CPP) GTEST_SKIP();

  sp<ILoggableInterface> loggable;
  EXPECT_EQ(android::OK, android::getService(ILoggableInterface::descriptor, &loggable));
  ASSERT_NE(nullptr, loggable);

  string captured;
  BpLoggableInterface::logFunc = [&](const Json::Value& tx) {
    auto copy = tx;
    copy.removeMember("duration_ms");
    copy.removeMember("proxy_address");
    captured = copy.toStyledString();
  };

  bool boolValue = true;
  vector<bool> boolArray{false, true};
  int8_t byteValue = 41;
  vector<uint8_t> byteArray{42, 43};
  char16_t charValue = 'x';
  vector<char16_t> charArray{'a', 'b', 'c'};
  int32_t intValue{44};
  vector<int32_t> intArray{45, 46};
  int64_t longValue = 47;
  vector<int64_t> longArray{48, 49};
  float floatValue{50};
  vector<float> floatArray{51, 52};
  double doubleValue{52};
  vector<double> doubleArray{53, 54};
  String16 stringValue("def");
  vector<String16> stringArray{String16("ghi"), String16("jkl")};
  vector<String16> listValue{String16("mno")};
  Data dataValue;
  dataValue.num = 42;
  dataValue.str = "abc";
  dataValue.nestedUnion = "def";
  dataValue.nestedEnum = Enum::FOO;
  sp<IBinder> binderValue;
  optional<ParcelFileDescriptor> pfdValue;
  vector<ParcelFileDescriptor> pfdArray;
  vector<String16> _aidl_return;

  status = loggable->LogThis(boolValue, &boolArray, byteValue, &byteArray, charValue, &charArray,
                             intValue, &intArray, longValue, &longArray, floatValue, &floatArray,
                             doubleValue, &doubleArray, stringValue, &stringArray, &listValue,
                             dataValue, binderValue, &pfdValue, &pfdArray, &_aidl_return);
  EXPECT_TRUE(status.isOk());
  EXPECT_EQ(vector<String16>{String16("loggable")}, _aidl_return);

  string expected = R"({
   "_aidl_return" : [ true ],
   "binder_status" : {
      "exception_code" : 0,
      "exception_message" : "",
      "service_specific_error_code" : 0,
      "transaction_error" : 0
   },
   "input_args" : [
      {
         "name" : "boolValue",
         "value" : "true"
      },
      {
         "name" : "boolArray",
         "value" : [ "false", "true" ]
      },
      {
         "name" : "byteValue",
         "value" : 41
      },
      {
         "name" : "byteArray",
         "value" : [ 42, 43 ]
      },
      {
         "name" : "charValue",
         "value" : "x"
      },
      {
         "name" : "charArray",
         "value" : [ "a", "b", "c" ]
      },
      {
         "name" : "intValue",
         "value" : 44
      },
      {
         "name" : "intArray",
         "value" : [ 45, 46 ]
      },
      {
         "name" : "longValue",
         "value" : 47
      },
      {
         "name" : "longArray",
         "value" : [ 48, 49 ]
      },
      {
         "name" : "floatValue",
         "value" : 50
      },
      {
         "name" : "floatArray",
         "value" : [ 51, 52 ]
      },
      {
         "name" : "doubleValue",
         "value" : 52
      },
      {
         "name" : "doubleArray",
         "value" : [ 53, 54 ]
      },
      {
         "name" : "stringValue",
         "value" : true
      },
      {
         "name" : "stringArray",
         "value" : [ true, true ]
      },
      {
         "name" : "dataValue",
         "value" : "Data{num: 42, str: abc, nestedUnion: Union{str: def}, nestedEnum: FOO}"
      }
   ],
   "interface_name" : "android.aidl.loggable.ILoggableInterface",
   "method_name" : "LogThis",
   "output_args" : [
      {
         "name" : "boolArray",
         "value" : [ "false", "true" ]
      },
      {
         "name" : "byteArray",
         "value" : [ 42, 43 ]
      },
      {
         "name" : "charArray",
         "value" : [ "a", "b", "c" ]
      },
      {
         "name" : "intArray",
         "value" : [ 45, 46 ]
      },
      {
         "name" : "longArray",
         "value" : [ 48, 49 ]
      },
      {
         "name" : "floatArray",
         "value" : [ 51, 52 ]
      },
      {
         "name" : "doubleArray",
         "value" : [ 53, 54 ]
      },
      {
         "name" : "stringArray",
         "value" : [ true, true ]
      }
   ]
}
)";
  EXPECT_EQ(expected, captured);
}
