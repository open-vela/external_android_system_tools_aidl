#pragma once

#include "aidl/android/aidl/tests/permission/IProtected.h"

#include <android/binder_ibinder.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
namespace permission {
class BpProtected : public ::ndk::BpCInterface<IProtected> {
public:
  explicit BpProtected(const ::ndk::SpAIBinder& binder);
  virtual ~BpProtected();

  ::ndk::ScopedAStatus PermissionProtected() override;
  ::ndk::ScopedAStatus MultiplePermissionsAll() override;
  ::ndk::ScopedAStatus MultiplePermissionsAny() override;
};
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
