#pragma once

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Status.h>
#include <utils/StrongPointer.h>

namespace android {
namespace aidl {
namespace tests {
namespace permission {
class IProtectedInterface : public ::android::IInterface {
public:
  DECLARE_META_INTERFACE(ProtectedInterface)
  virtual ::android::binder::Status Method1() = 0;
  virtual ::android::binder::Status Method2() = 0;
};  // class IProtectedInterface

class IProtectedInterfaceDefault : public IProtectedInterface {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status Method1() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
  ::android::binder::Status Method2() override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class IProtectedInterfaceDefault
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
