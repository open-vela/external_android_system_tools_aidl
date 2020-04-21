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

#include "aidl_test_client_nullables.h"

#include <utils/String16.h>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

// libutils:
using android::sp;
using android::String16;

// libbinder:
using android::binder::Status;

// generated
using android::aidl::tests::ByteEnum;
using android::aidl::tests::IntEnum;
using android::aidl::tests::ITestService;
using android::aidl::tests::LongEnum;
using android::aidl::tests::SimpleParcelable;

using std::cerr;
using std::cout;
using std::endl;
using std::make_optional;
using std::optional;
using std::string;
using std::vector;

namespace android {
namespace aidl {
namespace tests {
namespace client {

namespace {
template <typename T>
bool ValuesEqual(const optional<T>& in, const optional<T>& out) {
  return *in == *out;
}

template <>
bool ValuesEqual<vector<optional<String16>>>(const optional<vector<optional<String16>>>& in,
                                             const optional<vector<optional<String16>>>& out) {
  if (!in) {
    return !out;
  }

  if (!out) {
    return false;
  }

  if (in->size() != out->size()) {
    return false;
  }

  for (size_t i = 0; i < in->size(); i++) {
    const auto& a = (*in)[i];
    const auto& b = (*out)[i];

    if (!(a || b)) {
      continue;
    }

    if (!(a && b)) {
      return false;
    }

    if (*a != *b) {
      return false;
    }
  }

  return true;
}

template <typename T>
bool ConfirmNullableType(const sp<ITestService>& s, const string& type_name, optional<T> in,
                         Status (ITestService::*func)(const optional<T>&, optional<T>*)) {
  cout << "... Confirming nullables for " << type_name << " ..." << endl;
  Status status;
  optional<T> out;

  status = (*s.*func)(in, &out);

  if (!status.isOk()) {
    cerr << "Could not repeat nullable " << type_name << "." << endl;
    return false;
  }

  if (!out) {
    cerr << "Got back null when repeating " << type_name << "." << endl;
    return false;
  }

  if (!ValuesEqual(in, out)) {
    cerr << "Got back a non-matching value when repeating " << type_name
         << "." << endl;
    return false;
  }

  in.reset();
  status = (*s.*func)(in, &out);

  if (!status.isOk()) {
    cerr << "Could not repeat null as " << type_name << "." << endl;
    return false;
  }

  if (out) {
    cerr << "Got back a value when sent null for " << type_name << "."
         << endl;
    return false;
  }

  return true;
}

bool CheckAppropriateIBinderHandling(const sp<ITestService>& s) {

  Status status;
  sp<IBinder> binder = new BBinder();
  sp<IBinder> null_binder = nullptr;
  vector<sp<IBinder>> list_with_nulls{binder, null_binder};
  vector<sp<IBinder>> list_without_nulls{binder, binder};

  // Methods without @nullable throw up when given null binders
  if (s->TakesAnIBinder(null_binder).exceptionCode() !=
      binder::Status::EX_NULL_POINTER) {
    cerr << "Did not receive expected null exception on line: "
         << __LINE__ << endl;
    return false;
  }
  if (s->TakesAnIBinderList(list_with_nulls).exceptionCode() != binder::Status::EX_NULL_POINTER) {
    cerr << "Did not receive expected null exception on line: "
         << __LINE__ << endl;
    return false;
  }

  // But those same methods are fine with valid binders
  if (!s->TakesAnIBinder(binder).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (!s->TakesAnIBinderList(list_without_nulls).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }

  // And methods with @nullable don't care.
  if (!s->TakesANullableIBinder(null_binder).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (!s->TakesANullableIBinder(binder).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (!s->TakesANullableIBinderList(list_with_nulls).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (!s->TakesANullableIBinderList(list_without_nulls).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }

  return true;
}

bool CheckAppropriateIInterfaceHandling(const sp<ITestService>& s) {

  sp<INamedCallback> callback;
  if (!s->GetCallback(false, &callback).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (callback.get() == nullptr) {
    cerr << "Expected to receive a non-null binder on line: "
         << __LINE__ << endl;
    return false;
  }
  if (!s->GetCallback(true, &callback).isOk()) {
    cerr << "Received unexpected exception on line "
         << __LINE__ << endl;
    return false;
  }
  if (callback.get() != nullptr) {
    cerr << "Expected to receive a null binder on line: "
         << __LINE__ << endl;
    return false;
  }
  return true;
}

}  // namespace

bool ConfirmNullables(const sp<ITestService>& s) {
  Status status;
  cout << "Confirming passing and returning nullable values works." << endl;

  if (!ConfirmNullableType(s, "integer array", make_optional(vector<int32_t>{1, 2, 3}),
                           &ITestService::RepeatNullableIntArray)) {
    return false;
  }

  if (!ConfirmNullableType(s, "byte enum array",
                           make_optional(vector<ByteEnum>{ByteEnum::FOO, ByteEnum::BAR}),
                           &ITestService::RepeatNullableByteEnumArray)) {
    return false;
  }

  if (!ConfirmNullableType(s, "int enum array",
                           make_optional(vector<IntEnum>{IntEnum::FOO, IntEnum::BAR}),
                           &ITestService::RepeatNullableIntEnumArray)) {
    return false;
  }

  if (!ConfirmNullableType(s, "long enum array",
                           make_optional(vector<LongEnum>{LongEnum::FOO, LongEnum::BAR}),
                           &ITestService::RepeatNullableLongEnumArray)) {
    return false;
  }

  if (!ConfirmNullableType(s, "string", make_optional(String16("Blooob")),
                           &ITestService::RepeatNullableString)) {
    return false;
  }

  vector<optional<String16>> test_string_array;
  test_string_array.push_back(String16("Wat"));
  test_string_array.push_back(String16("Blooob"));
  test_string_array.push_back(String16("Wat"));
  test_string_array.push_back(std::nullopt);
  test_string_array.push_back(String16("YEAH"));
  test_string_array.push_back(String16("OKAAAAY"));

  if (!ConfirmNullableType(s, "string array", make_optional(test_string_array),
                           &ITestService::RepeatNullableStringList)) {
    return false;
  }

  if (!ConfirmNullableType(s, "parcelable", make_optional<SimpleParcelable>("Booya", 42),
                           &ITestService::RepeatNullableParcelable)) {
    return false;
  }

  if (!CheckAppropriateIBinderHandling(s)) {
    cerr << "Handled null IBinders poorly." << endl;
    return false;
  }

  if (!CheckAppropriateIInterfaceHandling(s)) {
    cerr << "Handled nullable IInterface instances poorly." << endl;
    return false;
  }

  return true;
}

}  // namespace client
}  // namespace tests
}  // namespace aidl
}  // namespace android
