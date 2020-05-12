/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tests/test_data.h"

namespace android {
namespace aidl {
namespace test_data {
namespace ping_responder {

// clang-format off
const char kCanonicalName[] = "android.os.IPingResponder";
const char kInterfaceDefinition[] = R"(
package android.os;

import bar.Unused;

interface IPingResponder {
  String Ping(String input);

  @nullable
  String NullablePing(@nullable String input);

  @utf8InCpp
  String Utf8Ping(@utf8InCpp String input);

  @utf8InCpp @nullable
  String NullableUtf8Ping(@utf8InCpp @nullable String input);
}
)";

const char kCppOutputPath[] = "some/path/to/output.cpp";

const char kCppParcelableHeader[] = "cpp-header-str";

const char* kImportedParcelables[] = {
  "bar.Unused",
  nullptr,
};

const char* kImportedInterfaces[] = {
  nullptr,
};

const char kGenHeaderDir[] = "some/path";
const char kGenInterfaceHeaderPath[] = "some/path/android/os/IPingResponder.h";
const char kGenClientHeaderPath[] = "some/path/android/os/BpPingResponder.h";
const char kGenServerHeaderPath[] = "some/path/android/os/BnPingResponder.h";

const char kExpectedCppDepsOutput[] =
R"(some/path/to/output.cpp : \
  android/os/IPingResponder.aidl \
  ./bar/Unused.aidl

android/os/IPingResponder.aidl :
./bar/Unused.aidl :

some/path/android/os/BpPingResponder.h \
    some/path/android/os/BnPingResponder.h \
    some/path/android/os/IPingResponder.h : \
    android/os/IPingResponder.aidl \
    ./bar/Unused.aidl
)";

const char kExpectedCppOutput[] =
    R"(#include <android/os/IPingResponder.h>
#include <android/os/BpPingResponder.h>

namespace android {

namespace os {

DO_NOT_DIRECTLY_USE_ME_IMPLEMENT_META_INTERFACE(PingResponder, "android.os.IPingResponder")

}  // namespace os

}  // namespace android
#include <android/os/BpPingResponder.h>
#include <binder/Parcel.h>
#include <android-base/macros.h>

namespace android {

namespace os {

BpPingResponder::BpPingResponder(const ::android::sp<::android::IBinder>& _aidl_impl)
    : BpInterface<IPingResponder>(_aidl_impl){
}

::android::binder::Status BpPingResponder::Ping(const ::android::String16& input, ::android::String16* _aidl_return) {
  ::android::Parcel _aidl_data;
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeString16(input);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 0 /* Ping */, _aidl_data, &_aidl_reply);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IPingResponder::getDefaultImpl())) {
     return IPingResponder::getDefaultImpl()->Ping(input, _aidl_return);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_status.readFromParcel(_aidl_reply);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  if (!_aidl_status.isOk()) {
    return _aidl_status;
  }
  _aidl_ret_status = _aidl_reply.readString16(_aidl_return);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

::android::binder::Status BpPingResponder::NullablePing(const ::std::optional<::android::String16>& input, ::std::optional<::android::String16>* _aidl_return) {
  ::android::Parcel _aidl_data;
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeString16(input);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 1 /* NullablePing */, _aidl_data, &_aidl_reply);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IPingResponder::getDefaultImpl())) {
     return IPingResponder::getDefaultImpl()->NullablePing(input, _aidl_return);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_status.readFromParcel(_aidl_reply);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  if (!_aidl_status.isOk()) {
    return _aidl_status;
  }
  _aidl_ret_status = _aidl_reply.readString16(_aidl_return);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

::android::binder::Status BpPingResponder::Utf8Ping(const ::std::string& input, ::std::string* _aidl_return) {
  ::android::Parcel _aidl_data;
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeUtf8AsUtf16(input);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 2 /* Utf8Ping */, _aidl_data, &_aidl_reply);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IPingResponder::getDefaultImpl())) {
     return IPingResponder::getDefaultImpl()->Utf8Ping(input, _aidl_return);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_status.readFromParcel(_aidl_reply);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  if (!_aidl_status.isOk()) {
    return _aidl_status;
  }
  _aidl_ret_status = _aidl_reply.readUtf8FromUtf16(_aidl_return);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

::android::binder::Status BpPingResponder::NullableUtf8Ping(const ::std::optional<::std::string>& input, ::std::optional<::std::string>* _aidl_return) {
  ::android::Parcel _aidl_data;
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeUtf8AsUtf16(input);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 3 /* NullableUtf8Ping */, _aidl_data, &_aidl_reply);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IPingResponder::getDefaultImpl())) {
     return IPingResponder::getDefaultImpl()->NullableUtf8Ping(input, _aidl_return);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_status.readFromParcel(_aidl_reply);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  if (!_aidl_status.isOk()) {
    return _aidl_status;
  }
  _aidl_ret_status = _aidl_reply.readUtf8FromUtf16(_aidl_return);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

}  // namespace os

}  // namespace android
#include <android/os/BnPingResponder.h>
#include <binder/Parcel.h>
#include <binder/Stability.h>

namespace android {

namespace os {

BnPingResponder::BnPingResponder()
{
  ::android::internal::Stability::markCompilationUnit(this);
}

::android::status_t BnPingResponder::onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) {
  ::android::status_t _aidl_ret_status = ::android::OK;
  switch (_aidl_code) {
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 0 /* Ping */:
  {
    ::android::String16 in_input;
    ::android::String16 _aidl_return;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readString16(&in_input);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    ::android::binder::Status _aidl_status(Ping(in_input, &_aidl_return));
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
      break;
    }
    _aidl_ret_status = _aidl_reply->writeString16(_aidl_return);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
  }
  break;
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 1 /* NullablePing */:
  {
    ::std::optional<::android::String16> in_input;
    ::std::optional<::android::String16> _aidl_return;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readString16(&in_input);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    ::android::binder::Status _aidl_status(NullablePing(in_input, &_aidl_return));
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
      break;
    }
    _aidl_ret_status = _aidl_reply->writeString16(_aidl_return);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
  }
  break;
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 2 /* Utf8Ping */:
  {
    ::std::string in_input;
    ::std::string _aidl_return;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readUtf8FromUtf16(&in_input);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    ::android::binder::Status _aidl_status(Utf8Ping(in_input, &_aidl_return));
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
      break;
    }
    _aidl_ret_status = _aidl_reply->writeUtf8AsUtf16(_aidl_return);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
  }
  break;
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 3 /* NullableUtf8Ping */:
  {
    ::std::optional<::std::string> in_input;
    ::std::optional<::std::string> _aidl_return;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readUtf8FromUtf16(&in_input);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    ::android::binder::Status _aidl_status(NullableUtf8Ping(in_input, &_aidl_return));
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
      break;
    }
    _aidl_ret_status = _aidl_reply->writeUtf8AsUtf16(_aidl_return);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
  }
  break;
  default:
  {
    _aidl_ret_status = ::android::BBinder::onTransact(_aidl_code, _aidl_data, _aidl_reply, _aidl_flags);
  }
  break;
  }
  if (_aidl_ret_status == ::android::UNEXPECTED_NULL) {
    _aidl_ret_status = ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_NULL_POINTER).writeToParcel(_aidl_reply);
  }
  return _aidl_ret_status;
}

}  // namespace os

}  // namespace android
)";

const char kExpectedIHeaderOutput[] =
    R"(#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <optional>
#include <string>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

namespace android {

namespace os {

class IPingResponder : public ::android::IInterface {
public:
  DECLARE_META_INTERFACE(PingResponder)
  virtual ::android::binder::Status Ping(const ::android::String16& input, ::android::String16* _aidl_return) = 0;
  virtual ::android::binder::Status NullablePing(const ::std::optional<::android::String16>& input, ::std::optional<::android::String16>* _aidl_return) = 0;
  virtual ::android::binder::Status Utf8Ping(const ::std::string& input, ::std::string* _aidl_return) = 0;
  virtual ::android::binder::Status NullableUtf8Ping(const ::std::optional<::std::string>& input, ::std::optional<::std::string>* _aidl_return) = 0;
};  // class IPingResponder

class IPingResponderDefault : public IPingResponder {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status Ping(const ::android::String16&, ::android::String16*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status NullablePing(const ::std::optional<::android::String16>&, ::std::optional<::android::String16>*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status Utf8Ping(const ::std::string&, ::std::string*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status NullableUtf8Ping(const ::std::optional<::std::string>&, ::std::optional<::std::string>*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IPingResponderDefault

}  // namespace os

}  // namespace android
)";

const char kExpectedBpHeaderOutput[] =
    R"(#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <android/os/IPingResponder.h>

namespace android {

namespace os {

class BpPingResponder : public ::android::BpInterface<IPingResponder> {
public:
  explicit BpPingResponder(const ::android::sp<::android::IBinder>& _aidl_impl);
  virtual ~BpPingResponder() = default;
  ::android::binder::Status Ping(const ::android::String16& input, ::android::String16* _aidl_return) override;
  ::android::binder::Status NullablePing(const ::std::optional<::android::String16>& input, ::std::optional<::android::String16>* _aidl_return) override;
  ::android::binder::Status Utf8Ping(const ::std::string& input, ::std::string* _aidl_return) override;
  ::android::binder::Status NullableUtf8Ping(const ::std::optional<::std::string>& input, ::std::optional<::std::string>* _aidl_return) override;
};  // class BpPingResponder

}  // namespace os

}  // namespace android
)";

const char kExpectedBnHeaderOutput[] =
    R"(#pragma once

#include <binder/IInterface.h>
#include <android/os/IPingResponder.h>

namespace android {

namespace os {

class BnPingResponder : public ::android::BnInterface<IPingResponder> {
public:
  explicit BnPingResponder();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnPingResponder

}  // namespace os

}  // namespace android
)";

const char kExpectedCppOutputWithVersionAndHash[] =
    R"(#include <android/os/IPingResponder.h>
#include <android/os/BpPingResponder.h>

namespace android {

namespace os {

DO_NOT_DIRECTLY_USE_ME_IMPLEMENT_META_INTERFACE(PingResponder, "android.os.IPingResponder")

}  // namespace os

}  // namespace android
#include <android/os/BpPingResponder.h>
#include <binder/Parcel.h>
#include <android-base/macros.h>

namespace android {

namespace os {

BpPingResponder::BpPingResponder(const ::android::sp<::android::IBinder>& _aidl_impl)
    : BpInterface<IPingResponder>(_aidl_impl){
}

::android::binder::Status BpPingResponder::Ping(const ::android::String16& input, ::android::String16* _aidl_return) {
  ::android::Parcel _aidl_data;
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeString16(input);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 0 /* Ping */, _aidl_data, &_aidl_reply);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IPingResponder::getDefaultImpl())) {
     return IPingResponder::getDefaultImpl()->Ping(input, _aidl_return);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_status.readFromParcel(_aidl_reply);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  if (!_aidl_status.isOk()) {
    return _aidl_status;
  }
  _aidl_ret_status = _aidl_reply.readString16(_aidl_return);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

::android::binder::Status BpPingResponder::NullablePing(const ::std::optional<::android::String16>& input, ::std::optional<::android::String16>* _aidl_return) {
  ::android::Parcel _aidl_data;
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeString16(input);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 1 /* NullablePing */, _aidl_data, &_aidl_reply);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IPingResponder::getDefaultImpl())) {
     return IPingResponder::getDefaultImpl()->NullablePing(input, _aidl_return);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_status.readFromParcel(_aidl_reply);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  if (!_aidl_status.isOk()) {
    return _aidl_status;
  }
  _aidl_ret_status = _aidl_reply.readString16(_aidl_return);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

::android::binder::Status BpPingResponder::Utf8Ping(const ::std::string& input, ::std::string* _aidl_return) {
  ::android::Parcel _aidl_data;
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeUtf8AsUtf16(input);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 2 /* Utf8Ping */, _aidl_data, &_aidl_reply);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IPingResponder::getDefaultImpl())) {
     return IPingResponder::getDefaultImpl()->Utf8Ping(input, _aidl_return);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_status.readFromParcel(_aidl_reply);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  if (!_aidl_status.isOk()) {
    return _aidl_status;
  }
  _aidl_ret_status = _aidl_reply.readUtf8FromUtf16(_aidl_return);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

::android::binder::Status BpPingResponder::NullableUtf8Ping(const ::std::optional<::std::string>& input, ::std::optional<::std::string>* _aidl_return) {
  ::android::Parcel _aidl_data;
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_data.writeUtf8AsUtf16(input);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 3 /* NullableUtf8Ping */, _aidl_data, &_aidl_reply);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IPingResponder::getDefaultImpl())) {
     return IPingResponder::getDefaultImpl()->NullableUtf8Ping(input, _aidl_return);
  }
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = _aidl_status.readFromParcel(_aidl_reply);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  if (!_aidl_status.isOk()) {
    return _aidl_status;
  }
  _aidl_ret_status = _aidl_reply.readUtf8FromUtf16(_aidl_return);
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

int32_t BpPingResponder::getInterfaceVersion() {
  if (cached_version_ == -1) {
    ::android::Parcel data;
    ::android::Parcel reply;
    data.writeInterfaceToken(getInterfaceDescriptor());
    ::android::status_t err = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 16777214 /* getInterfaceVersion */, data, &reply);
    if (err == ::android::OK) {
      ::android::binder::Status _aidl_status;
      err = _aidl_status.readFromParcel(reply);
      if (err == ::android::OK && _aidl_status.isOk()) {
        cached_version_ = reply.readInt32();
      }
    }
  }
  return cached_version_;
}

std::string BpPingResponder::getInterfaceHash() {
  std::lock_guard<std::mutex> lockGuard(cached_hash_mutex_);
  if (cached_hash_ == "-1") {
    ::android::Parcel data;
    ::android::Parcel reply;
    data.writeInterfaceToken(getInterfaceDescriptor());
    ::android::status_t err = remote()->transact(::android::IBinder::FIRST_CALL_TRANSACTION + 16777213 /* getInterfaceHash */, data, &reply);
    if (err == ::android::OK) {
      ::android::binder::Status _aidl_status;
      err = _aidl_status.readFromParcel(reply);
      if (err == ::android::OK && _aidl_status.isOk()) {
        reply.readUtf8FromUtf16(&cached_hash_);
      }
    }
  }
  return cached_hash_;
}

}  // namespace os

}  // namespace android
#include <android/os/BnPingResponder.h>
#include <binder/Parcel.h>
#include <binder/Stability.h>

namespace android {

namespace os {

BnPingResponder::BnPingResponder()
{
  ::android::internal::Stability::markCompilationUnit(this);
}

::android::status_t BnPingResponder::onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) {
  ::android::status_t _aidl_ret_status = ::android::OK;
  switch (_aidl_code) {
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 0 /* Ping */:
  {
    ::android::String16 in_input;
    ::android::String16 _aidl_return;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readString16(&in_input);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    ::android::binder::Status _aidl_status(Ping(in_input, &_aidl_return));
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
      break;
    }
    _aidl_ret_status = _aidl_reply->writeString16(_aidl_return);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
  }
  break;
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 1 /* NullablePing */:
  {
    ::std::optional<::android::String16> in_input;
    ::std::optional<::android::String16> _aidl_return;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readString16(&in_input);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    ::android::binder::Status _aidl_status(NullablePing(in_input, &_aidl_return));
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
      break;
    }
    _aidl_ret_status = _aidl_reply->writeString16(_aidl_return);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
  }
  break;
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 2 /* Utf8Ping */:
  {
    ::std::string in_input;
    ::std::string _aidl_return;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readUtf8FromUtf16(&in_input);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    ::android::binder::Status _aidl_status(Utf8Ping(in_input, &_aidl_return));
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
      break;
    }
    _aidl_ret_status = _aidl_reply->writeUtf8AsUtf16(_aidl_return);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
  }
  break;
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 3 /* NullableUtf8Ping */:
  {
    ::std::optional<::std::string> in_input;
    ::std::optional<::std::string> _aidl_return;
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    _aidl_ret_status = _aidl_data.readUtf8FromUtf16(&in_input);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    ::android::binder::Status _aidl_status(NullableUtf8Ping(in_input, &_aidl_return));
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
      break;
    }
    _aidl_ret_status = _aidl_reply->writeUtf8AsUtf16(_aidl_return);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
  }
  break;
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 16777214 /* getInterfaceVersion */:
  {
    _aidl_data.checkInterface(this);
    _aidl_reply->writeNoException();
    _aidl_reply->writeInt32(IPingResponder::VERSION);
  }
  break;
  case ::android::IBinder::FIRST_CALL_TRANSACTION + 16777213 /* getInterfaceHash */:
  {
    _aidl_data.checkInterface(this);
    _aidl_reply->writeNoException();
    _aidl_reply->writeUtf8AsUtf16(IPingResponder::HASH);
  }
  break;
  default:
  {
    _aidl_ret_status = ::android::BBinder::onTransact(_aidl_code, _aidl_data, _aidl_reply, _aidl_flags);
  }
  break;
  }
  if (_aidl_ret_status == ::android::UNEXPECTED_NULL) {
    _aidl_ret_status = ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_NULL_POINTER).writeToParcel(_aidl_reply);
  }
  return _aidl_ret_status;
}

int32_t BnPingResponder::getInterfaceVersion() {
  return IPingResponder::VERSION;
}

std::string BnPingResponder::getInterfaceHash() {
  return IPingResponder::HASH;
}

}  // namespace os

}  // namespace android
)";

const char kExpectedIHeaderOutputWithVersionAndHash[] =
    R"(#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <cstdint>
#include <optional>
#include <string>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

namespace android {

namespace os {

class IPingResponder : public ::android::IInterface {
public:
  DECLARE_META_INTERFACE(PingResponder)
  const int32_t VERSION = 10;
  const std::string HASH = "abcdefg";
  virtual ::android::binder::Status Ping(const ::android::String16& input, ::android::String16* _aidl_return) = 0;
  virtual ::android::binder::Status NullablePing(const ::std::optional<::android::String16>& input, ::std::optional<::android::String16>* _aidl_return) = 0;
  virtual ::android::binder::Status Utf8Ping(const ::std::string& input, ::std::string* _aidl_return) = 0;
  virtual ::android::binder::Status NullableUtf8Ping(const ::std::optional<::std::string>& input, ::std::optional<::std::string>* _aidl_return) = 0;
  virtual int32_t getInterfaceVersion() = 0;
  virtual std::string getInterfaceHash() = 0;
};  // class IPingResponder

class IPingResponderDefault : public IPingResponder {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status Ping(const ::android::String16&, ::android::String16*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status NullablePing(const ::std::optional<::android::String16>&, ::std::optional<::android::String16>*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status Utf8Ping(const ::std::string&, ::std::string*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status NullableUtf8Ping(const ::std::optional<::std::string>&, ::std::optional<::std::string>*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  int32_t getInterfaceVersion() override {
    return 0;
  }
  std::string getInterfaceHash() override {
    return "";
  }
};  // class IPingResponderDefault

}  // namespace os

}  // namespace android
)";

const char kExpectedBpHeaderOutputWithVersionAndHash[] =
R"(#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <android/os/IPingResponder.h>

namespace android {

namespace os {

class BpPingResponder : public ::android::BpInterface<IPingResponder> {
public:
  explicit BpPingResponder(const ::android::sp<::android::IBinder>& _aidl_impl);
  virtual ~BpPingResponder() = default;
  ::android::binder::Status Ping(const ::android::String16& input, ::android::String16* _aidl_return) override;
  ::android::binder::Status NullablePing(const ::std::optional<::android::String16>& input, ::std::optional<::android::String16>* _aidl_return) override;
  ::android::binder::Status Utf8Ping(const ::std::string& input, ::std::string* _aidl_return) override;
  ::android::binder::Status NullableUtf8Ping(const ::std::optional<::std::string>& input, ::std::optional<::std::string>* _aidl_return) override;
  int32_t getInterfaceVersion() override;
  std::string getInterfaceHash() override;
private:
  int32_t cached_version_ = -1;
  std::string cached_hash_ = "-1";
  std::mutex cached_hash_mutex_;
};  // class BpPingResponder

}  // namespace os

}  // namespace android
)";

const char kExpectedBnHeaderOutputWithVersionAndHash[] =
    R"(#pragma once

#include <binder/IInterface.h>
#include <android/os/IPingResponder.h>

namespace android {

namespace os {

class BnPingResponder : public ::android::BnInterface<IPingResponder> {
public:
  explicit BnPingResponder();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
  int32_t getInterfaceVersion() final override;
  std::string getInterfaceHash();
};  // class BnPingResponder

}  // namespace os

}  // namespace android
)";

const char kRustOutputDirectory[] = "some/path/to";
const char kRustOutputPath[] = "some/path/to/android/os/IPingResponder.rs";

const char kExpectedRustDepsOutput[] =
R"(some/path/to/android/os/IPingResponder.rs : \
  android/os/IPingResponder.aidl \
  ./bar/Unused.aidl

android/os/IPingResponder.aidl :
./bar/Unused.aidl :
)";

const char kExpectedRustOutput[] =
    R"(#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinder;
use binder::declare_binder_interface;
declare_binder_interface! {
  IPingResponder["android.os.IPingResponder"] {
    native: BnPingResponder(on_transact),
    proxy: BpPingResponder {
    },
  }
}
pub trait IPingResponder: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.os.IPingResponder" }
  fn Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn NullablePing(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn Utf8Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn NullableUtf8Ping(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getDefaultImpl() -> IPingResponderDefault where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: IPingResponderDefault) -> IPingResponderDefault where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub mod transactions {
  #[allow(unused_imports)] use binder::IBinder;
  pub const Ping: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 0;
  pub const NullablePing: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 1;
  pub const Utf8Ping: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 2;
  pub const NullableUtf8Ping: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 3;
}
pub type IPingResponderDefault = Option<std::sync::Arc<dyn IPingResponder + Sync>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<IPingResponderDefault> = std::sync::Mutex::new(None);
}
pub(crate) mod mangled { pub use super::IPingResponder as _7_android_2_os_14_IPingResponder; }
impl IPingResponder for BpPingResponder {
  fn Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> {
    let _aidl_reply = self.binder.transact(transactions::Ping, 0, |_aidl_data| {
      _aidl_data.write(_arg_input)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.Ping(_arg_input);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn NullablePing(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> {
    let _aidl_reply = self.binder.transact(transactions::NullablePing, 0, |_aidl_data| {
      _aidl_data.write(&_arg_input)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.NullablePing(_arg_input);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: Option<String> = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn Utf8Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> {
    let _aidl_reply = self.binder.transact(transactions::Utf8Ping, 0, |_aidl_data| {
      _aidl_data.write(_arg_input)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.Utf8Ping(_arg_input);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn NullableUtf8Ping(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> {
    let _aidl_reply = self.binder.transact(transactions::NullableUtf8Ping, 0, |_aidl_data| {
      _aidl_data.write(&_arg_input)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.NullableUtf8Ping(_arg_input);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: Option<String> = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
}
impl IPingResponder for binder::Binder<BnPingResponder> {
  fn Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> { self.0.Ping(_arg_input) }
  fn NullablePing(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> { self.0.NullablePing(_arg_input) }
  fn Utf8Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> { self.0.Utf8Ping(_arg_input) }
  fn NullableUtf8Ping(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> { self.0.NullableUtf8Ping(_arg_input) }
}
fn on_transact(_aidl_service: &dyn IPingResponder, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::Parcel, _aidl_reply: &mut binder::parcel::Parcel) -> binder::Result<()> {
  match _aidl_code {
    transactions::Ping => {
      let _arg_input: String = _aidl_data.read()?;
      let _aidl_return = _aidl_service.Ping(&_arg_input);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::NullablePing => {
      let _arg_input: Option<String> = _aidl_data.read()?;
      let _aidl_return = _aidl_service.NullablePing(_arg_input.as_deref());
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::Utf8Ping => {
      let _arg_input: String = _aidl_data.read()?;
      let _aidl_return = _aidl_service.Utf8Ping(&_arg_input);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::NullableUtf8Ping => {
      let _arg_input: Option<String> = _aidl_data.read()?;
      let _aidl_return = _aidl_service.NullableUtf8Ping(_arg_input.as_deref());
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    _ => Err(binder::StatusCode::UNKNOWN_TRANSACTION)
  }
}
)";

const char kExpectedRustOutputWithVersionAndHash[] =
    R"(#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinder;
use binder::declare_binder_interface;
declare_binder_interface! {
  IPingResponder["android.os.IPingResponder"] {
    native: BnPingResponder(on_transact),
    proxy: BpPingResponder {
      cached_version: std::sync::atomic::AtomicI32 = std::sync::atomic::AtomicI32::new(-1),
      cached_hash: std::sync::Mutex<Option<String>> = std::sync::Mutex::new(None)
    },
  }
}
pub trait IPingResponder: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.os.IPingResponder" }
  fn Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn NullablePing(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn Utf8Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn NullableUtf8Ping(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getInterfaceVersion(&self) -> binder::public_api::Result<i32> {
    Ok(VERSION)
  }
  fn getInterfaceHash(&self) -> binder::public_api::Result<String> {
    Ok(HASH.into())
  }
  fn getDefaultImpl() -> IPingResponderDefault where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: IPingResponderDefault) -> IPingResponderDefault where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub mod transactions {
  #[allow(unused_imports)] use binder::IBinder;
  pub const Ping: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 0;
  pub const NullablePing: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 1;
  pub const Utf8Ping: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 2;
  pub const NullableUtf8Ping: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 3;
  pub const getInterfaceVersion: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 16777214;
  pub const getInterfaceHash: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 16777213;
}
pub type IPingResponderDefault = Option<std::sync::Arc<dyn IPingResponder + Sync>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<IPingResponderDefault> = std::sync::Mutex::new(None);
}
pub(crate) mod mangled { pub use super::IPingResponder as _7_android_2_os_14_IPingResponder; }
pub const VERSION: i32 = 10;
pub const HASH: &str = "abcdefg";
impl IPingResponder for BpPingResponder {
  fn Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> {
    let _aidl_reply = self.binder.transact(transactions::Ping, 0, |_aidl_data| {
      _aidl_data.write(_arg_input)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.Ping(_arg_input);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn NullablePing(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> {
    let _aidl_reply = self.binder.transact(transactions::NullablePing, 0, |_aidl_data| {
      _aidl_data.write(&_arg_input)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.NullablePing(_arg_input);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: Option<String> = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn Utf8Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> {
    let _aidl_reply = self.binder.transact(transactions::Utf8Ping, 0, |_aidl_data| {
      _aidl_data.write(_arg_input)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.Utf8Ping(_arg_input);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn NullableUtf8Ping(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> {
    let _aidl_reply = self.binder.transact(transactions::NullableUtf8Ping, 0, |_aidl_data| {
      _aidl_data.write(&_arg_input)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.NullableUtf8Ping(_arg_input);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: Option<String> = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn getInterfaceVersion(&self) -> binder::public_api::Result<i32> {
    let _aidl_version = self.cached_version.load(std::sync::atomic::Ordering::Relaxed);
    if _aidl_version != -1 { return Ok(_aidl_version); }
    let _aidl_reply = self.binder.transact(transactions::getInterfaceVersion, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.getInterfaceVersion();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: i32 = _aidl_reply.read()?;
    self.cached_version.store(_aidl_return, std::sync::atomic::Ordering::Relaxed);
    Ok(_aidl_return)
  }
  fn getInterfaceHash(&self) -> binder::public_api::Result<String> {
    {
      let _aidl_hash_lock = self.cached_hash.lock().unwrap();
      if let Some(ref _aidl_hash) = *_aidl_hash_lock {
        return Ok(_aidl_hash.clone());
      }
    }
    let _aidl_reply = self.binder.transact(transactions::getInterfaceHash, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IPingResponder>::getDefaultImpl() {
        return _aidl_default_impl.getInterfaceHash();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    *self.cached_hash.lock().unwrap() = Some(_aidl_return.clone());
    Ok(_aidl_return)
  }
}
impl IPingResponder for binder::Binder<BnPingResponder> {
  fn Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> { self.0.Ping(_arg_input) }
  fn NullablePing(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> { self.0.NullablePing(_arg_input) }
  fn Utf8Ping(&self, _arg_input: &str) -> binder::public_api::Result<String> { self.0.Utf8Ping(_arg_input) }
  fn NullableUtf8Ping(&self, _arg_input: Option<&str>) -> binder::public_api::Result<Option<String>> { self.0.NullableUtf8Ping(_arg_input) }
  fn getInterfaceVersion(&self) -> binder::public_api::Result<i32> { self.0.getInterfaceVersion() }
  fn getInterfaceHash(&self) -> binder::public_api::Result<String> { self.0.getInterfaceHash() }
}
fn on_transact(_aidl_service: &dyn IPingResponder, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::Parcel, _aidl_reply: &mut binder::parcel::Parcel) -> binder::Result<()> {
  match _aidl_code {
    transactions::Ping => {
      let _arg_input: String = _aidl_data.read()?;
      let _aidl_return = _aidl_service.Ping(&_arg_input);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::NullablePing => {
      let _arg_input: Option<String> = _aidl_data.read()?;
      let _aidl_return = _aidl_service.NullablePing(_arg_input.as_deref());
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::Utf8Ping => {
      let _arg_input: String = _aidl_data.read()?;
      let _aidl_return = _aidl_service.Utf8Ping(&_arg_input);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::NullableUtf8Ping => {
      let _arg_input: Option<String> = _aidl_data.read()?;
      let _aidl_return = _aidl_service.NullableUtf8Ping(_arg_input.as_deref());
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getInterfaceVersion => {
      let _aidl_return = _aidl_service.getInterfaceVersion();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getInterfaceHash => {
      let _aidl_return = _aidl_service.getInterfaceHash();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    _ => Err(binder::StatusCode::UNKNOWN_TRANSACTION)
  }
}
)";

// clang-format on
}  // namespace ping_responder
}  // namespace test_data
}  // namespace aidl
}  // namespace android
