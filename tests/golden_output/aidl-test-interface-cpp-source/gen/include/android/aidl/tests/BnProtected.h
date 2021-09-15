#pragma once

#include <binder/IInterface.h>
#include <android/aidl/tests/IProtected.h>

namespace android {

namespace aidl {

namespace tests {

class BnProtected : public ::android::BnInterface<IProtected> {
public:
  static constexpr uint32_t TRANSACTION_PermissionProtected = ::android::IBinder::FIRST_CALL_TRANSACTION + 0;
  static constexpr uint32_t TRANSACTION_MultiplePermissions = ::android::IBinder::FIRST_CALL_TRANSACTION + 1;
  static constexpr uint32_t TRANSACTION_MultiplePermissions2 = ::android::IBinder::FIRST_CALL_TRANSACTION + 2;
  explicit BnProtected();
  ::android::status_t onTransact(uint32_t _aidl_code, const ::android::Parcel& _aidl_data, ::android::Parcel* _aidl_reply, uint32_t _aidl_flags) override;
};  // class BnProtected

}  // namespace tests

}  // namespace aidl

}  // namespace android
