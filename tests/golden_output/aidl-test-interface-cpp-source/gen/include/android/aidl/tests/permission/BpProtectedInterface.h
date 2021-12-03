#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Errors.h>
#include <android/aidl/tests/permission/IProtectedInterface.h>

namespace android {
namespace aidl {
namespace tests {
namespace permission {
class BpProtectedInterface : public ::android::BpInterface<IProtectedInterface> {
public:
  explicit BpProtectedInterface(const ::android::sp<::android::IBinder>& _aidl_impl);
  virtual ~BpProtectedInterface() = default;
  ::android::binder::Status Method1() override;
  ::android::binder::Status Method2() override;
};  // class BpProtectedInterface
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
