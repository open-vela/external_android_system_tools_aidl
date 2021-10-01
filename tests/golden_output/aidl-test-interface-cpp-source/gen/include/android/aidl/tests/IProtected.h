#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <utils/StrongPointer.h>

namespace android {
namespace aidl {
namespace tests {
class IProtected : public ::android::IInterface {
public:
  DECLARE_META_INTERFACE(Protected)
  virtual ::android::binder::Status PermissionProtected() = 0;
  virtual ::android::binder::Status MultiplePermissions() = 0;
  virtual ::android::binder::Status MultiplePermissions2() = 0;
};  // class IProtected

class IProtectedDefault : public IProtected {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status PermissionProtected() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status MultiplePermissions() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status MultiplePermissions2() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IProtectedDefault
}  // namespace tests
}  // namespace aidl
}  // namespace android
