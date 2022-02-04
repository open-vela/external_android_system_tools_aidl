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
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
