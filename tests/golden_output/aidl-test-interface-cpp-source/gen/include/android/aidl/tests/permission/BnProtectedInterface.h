#pragma once

#include <binder/IInterface.h>
#include <android/aidl/tests/permission/IProtectedInterface.h>

namespace android {
namespace aidl {
namespace tests {
namespace permission {
class BnProtectedInterface : public ::android::BnInterface<IProtectedInterface> {
public:
  static constexpr uint32_t TRANSACTION_Method1 = ::android::IBinder::FIRST_CALL_TRANSACTION + 0;
  static constexpr uint32_t TRANSACTION_Method2 = ::android::IBinder::FIRST_CALL_TRANSACTION + 1;
  explicit BnProtectedInterface();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnProtectedInterface

class IProtectedInterfaceDelegator : public BnProtectedInterface {
public:
  explicit IProtectedInterfaceDelegator(::android::sp<IProtectedInterface> &impl) : _aidl_delegate(impl) {}

  ::android::binder::Status Method1() override {
    return _aidl_delegate->Method1();
  }
  ::android::binder::Status Method2() override {
    return _aidl_delegate->Method2();
  }
private:
  ::android::sp<IProtectedInterface> _aidl_delegate;
};  // class IProtectedInterfaceDelegator
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
