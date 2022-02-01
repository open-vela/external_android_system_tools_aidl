#pragma once

#include "aidl/android/aidl/tests/IDeprecated.h"

#include <android/binder_ibinder.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
class __attribute__((deprecated("test"))) BnDeprecated : public ::ndk::BnCInterface<IDeprecated> {
public:
  BnDeprecated();
  virtual ~BnDeprecated();
protected:
  ::ndk::SpAIBinder createBinder() override;
private:
};
class __attribute__((deprecated("test"))) IDeprecatedDelegator : public BnDeprecated {
public:
  explicit IDeprecatedDelegator(const std::shared_ptr<IDeprecated> &impl) : _impl(impl) {}

protected:
private:
  std::shared_ptr<IDeprecated> _impl;
};

}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
