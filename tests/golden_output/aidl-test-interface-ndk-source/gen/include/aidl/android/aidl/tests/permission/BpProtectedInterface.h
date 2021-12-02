#pragma once

#include "aidl/android/aidl/tests/permission/IProtectedInterface.h"

#include <android/binder_ibinder.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
namespace permission {
class BpProtectedInterface : public ::ndk::BpCInterface<IProtectedInterface> {
public:
  explicit BpProtectedInterface(const ::ndk::SpAIBinder& binder);
  virtual ~BpProtectedInterface();

  ::ndk::ScopedAStatus Method1() override;
  ::ndk::ScopedAStatus Method2() override;
};
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
