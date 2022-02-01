#pragma once

#include "aidl/android/aidl/tests/permission/IProtected.h"

#include <android/binder_ibinder.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
namespace permission {
class BnProtected : public ::ndk::BnCInterface<IProtected> {
public:
  BnProtected();
  virtual ~BnProtected();
protected:
  ::ndk::SpAIBinder createBinder() override;
private:
};
class IProtectedDelegator : public BnProtected {
public:
  explicit IProtectedDelegator(const std::shared_ptr<IProtected> &impl) : _impl(impl) {}

  ::ndk::ScopedAStatus PermissionProtected() override {
    return _impl->PermissionProtected();
  }
  ::ndk::ScopedAStatus MultiplePermissionsAll() override {
    return _impl->MultiplePermissionsAll();
  }
  ::ndk::ScopedAStatus MultiplePermissionsAny() override {
    return _impl->MultiplePermissionsAny();
  }
protected:
private:
  std::shared_ptr<IProtected> _impl;
};

}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
