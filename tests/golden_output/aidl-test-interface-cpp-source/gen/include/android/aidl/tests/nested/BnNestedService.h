#pragma once

#include <binder/IInterface.h>
#include <android/aidl/tests/nested/INestedService.h>

namespace android {
namespace aidl {
namespace tests {
namespace nested {
class BnNestedService : public ::android::BnInterface<INestedService> {
public:
  static constexpr uint32_t TRANSACTION_flipStatus = ::android::IBinder::FIRST_CALL_TRANSACTION + 0;
  explicit BnNestedService();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnNestedService

class INestedServiceDelegator : public BnNestedService {
public:
  explicit INestedServiceDelegator(::android::sp<INestedService> &impl) : _aidl_delegate(impl) {}

  ::android::binder::Status flipStatus(const ::android::aidl::tests::nested::ParcelableWithNested& p, ::android::aidl::tests::nested::INestedService::Result* _aidl_return) override {
    return _aidl_delegate->flipStatus(p, _aidl_return);
  }
private:
  ::android::sp<INestedService> _aidl_delegate;
};  // class INestedServiceDelegator
}  // namespace nested
}  // namespace tests
}  // namespace aidl
}  // namespace android
