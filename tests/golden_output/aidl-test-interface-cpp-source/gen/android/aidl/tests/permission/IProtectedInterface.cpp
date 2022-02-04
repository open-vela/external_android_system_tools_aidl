#include <android/aidl/tests/permission/IProtectedInterface.h>
#include <android/aidl/tests/permission/BpProtectedInterface.h>
namespace android {
namespace aidl {
namespace tests {
namespace permission {
DO_NOT_DIRECTLY_USE_ME_IMPLEMENT_META_INTERFACE(ProtectedInterface, "android.aidl.tests.permission.IProtectedInterface")
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
#include <android/aidl/tests/permission/BpProtectedInterface.h>
#include <android/aidl/tests/permission/BnProtectedInterface.h>
#include <binder/Parcel.h>
#include <android-base/macros.h>

namespace android {
namespace aidl {
namespace tests {
namespace permission {

BpProtectedInterface::BpProtectedInterface(const ::android::sp<::android::IBinder>& _aidl_impl)
    : BpInterface<IProtectedInterface>(_aidl_impl){
}

::android::binder::Status BpProtectedInterface::Method1() {
  ::android::Parcel _aidl_data;
  _aidl_data.markForBinder(remoteStrong());
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(BnProtectedInterface::TRANSACTION_Method1, _aidl_data, &_aidl_reply, 0);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IProtectedInterface::getDefaultImpl())) {
     return IProtectedInterface::getDefaultImpl()->Method1();
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
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

::android::binder::Status BpProtectedInterface::Method2() {
  ::android::Parcel _aidl_data;
  _aidl_data.markForBinder(remoteStrong());
  ::android::Parcel _aidl_reply;
  ::android::status_t _aidl_ret_status = ::android::OK;
  ::android::binder::Status _aidl_status;
  _aidl_ret_status = _aidl_data.writeInterfaceToken(getInterfaceDescriptor());
  if (((_aidl_ret_status) != (::android::OK))) {
    goto _aidl_error;
  }
  _aidl_ret_status = remote()->transact(BnProtectedInterface::TRANSACTION_Method2, _aidl_data, &_aidl_reply, 0);
  if (UNLIKELY(_aidl_ret_status == ::android::UNKNOWN_TRANSACTION && IProtectedInterface::getDefaultImpl())) {
     return IProtectedInterface::getDefaultImpl()->Method2();
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
  _aidl_error:
  _aidl_status.setFromStatusT(_aidl_ret_status);
  return _aidl_status;
}

}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
#include <android/aidl/tests/permission/BnProtectedInterface.h>
#include <binder/Parcel.h>
#include <binder/Stability.h>

namespace android {
namespace aidl {
namespace tests {
namespace permission {

BnProtectedInterface::BnProtectedInterface()
{
  ::android::internal::Stability::markCompilationUnit(this);
}

::android::status_t BnProtectedInterface::onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) {
  ::android::status_t _aidl_ret_status = ::android::OK;
  switch (_aidl_code) {
  case BnProtectedInterface::TRANSACTION_Method1:
  {
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    ::android::binder::Status _aidl_status(Method1());
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
      break;
    }
  }
  break;
  case BnProtectedInterface::TRANSACTION_Method2:
  {
    if (!(_aidl_data.checkInterface(this))) {
      _aidl_ret_status = ::android::BAD_TYPE;
      break;
    }
    ::android::binder::Status _aidl_status(Method2());
    _aidl_ret_status = _aidl_status.writeToParcel(_aidl_reply);
    if (((_aidl_ret_status) != (::android::OK))) {
      break;
    }
    if (!_aidl_status.isOk()) {
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
    _aidl_ret_status = ::android::binder::Status::fromExceptionCode(::android::binder::Status::EX_NULL_POINTER).writeOverParcel(_aidl_reply);
  }
  return _aidl_ret_status;
}

}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
