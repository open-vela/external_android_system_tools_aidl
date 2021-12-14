#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <android/aidl/tests/permission/IProtected.h>

namespace android {
namespace aidl {
namespace tests {
namespace permission {
class BpProtected : public ::android::BpInterface<IProtected> {
public:
  explicit BpProtected(const ::android::sp<::android::IBinder>& _aidl_impl);
  virtual ~BpProtected() = default;
  ::android::binder::Status PermissionProtected() override;
  ::android::binder::Status MultiplePermissionsAll() override;
  ::android::binder::Status MultiplePermissionsAny() override;
};  // class BpProtected
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
