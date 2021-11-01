#pragma once

#include <binder/IInterface.h>
#include <android/aidl/tests/permission/IProtected.h>

namespace android {
namespace aidl {
namespace tests {
namespace permission {
class BnProtected : public ::android::BnInterface<IProtected> {
public:
  static constexpr uint32_t TRANSACTION_PermissionProtected = ::android::IBinder::FIRST_CALL_TRANSACTION + 0;
  static constexpr uint32_t TRANSACTION_MultiplePermissions = ::android::IBinder::FIRST_CALL_TRANSACTION + 1;
  static constexpr uint32_t TRANSACTION_MultiplePermissions2 = ::android::IBinder::FIRST_CALL_TRANSACTION + 2;
  explicit BnProtected();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnProtected

class IProtectedDelegator : public BnProtected {
public:
  explicit IProtectedDelegator(::android::sp<IProtected> &impl) : _aidl_delegate(impl) {}

  ::android::binder::Status PermissionProtected() override {
    return _aidl_delegate->PermissionProtected();
  }
  ::android::binder::Status MultiplePermissions() override {
    return _aidl_delegate->MultiplePermissions();
  }
  ::android::binder::Status MultiplePermissions2() override {
    return _aidl_delegate->MultiplePermissions2();
  }
private:
  ::android::sp<IProtected> _aidl_delegate;
};  // class IProtectedDelegator
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
