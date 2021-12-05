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
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
