#pragma once

#include "aidl/android/aidl/tests/IProtected.h"

#include <android/binder_ibinder.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
class BpProtected : public ::ndk::BpCInterface<IProtected> {
public:
  explicit BpProtected(const ::ndk::SpAIBinder& binder);
  virtual ~BpProtected();

  ::ndk::ScopedAStatus PermissionProtected() override;
  ::ndk::ScopedAStatus MultiplePermissions() override;
  ::ndk::ScopedAStatus MultiplePermissions2() override;
};
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
