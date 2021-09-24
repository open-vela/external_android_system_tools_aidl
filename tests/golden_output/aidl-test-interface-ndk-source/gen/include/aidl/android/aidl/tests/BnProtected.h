#pragma once

#include "aidl/android/aidl/tests/IProtected.h"

#include <android/binder_ibinder.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
class BnProtected : public ::ndk::BnCInterface<IProtected> {
public:
  BnProtected();
  virtual ~BnProtected();
protected:
  ::ndk::SpAIBinder createBinder() override;
private:
};
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
