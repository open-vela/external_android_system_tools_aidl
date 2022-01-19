#pragma once

#include "aidl/android/aidl/tests/permission/IProtectedInterface.h"

#include <android/binder_ibinder.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
namespace permission {
class BnProtectedInterface : public ::ndk::BnCInterface<IProtectedInterface> {
public:
  BnProtectedInterface();
  virtual ~BnProtectedInterface();
protected:
  ::ndk::SpAIBinder createBinder() override;
private:
};
class IProtectedInterfaceDelegator : public BnProtectedInterface {
public:
  explicit IProtectedInterfaceDelegator(const std::shared_ptr<IProtectedInterface> &impl) : _impl(impl) {}

  ::ndk::ScopedAStatus Method1() override {
    return _impl->Method1();
  }
  ::ndk::ScopedAStatus Method2() override {
    return _impl->Method2();
  }
protected:
private:
  std::shared_ptr<IProtectedInterface> _impl;
};

}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
