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
}  // namespace nested
}  // namespace tests
}  // namespace aidl
}  // namespace android
