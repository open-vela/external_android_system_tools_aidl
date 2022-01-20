#pragma once

#include "aidl/android/aidl/tests/nested/INestedService.h"

#include <android/binder_ibinder.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
namespace nested {
class BnNestedService : public ::ndk::BnCInterface<INestedService> {
public:
  BnNestedService();
  virtual ~BnNestedService();
protected:
  ::ndk::SpAIBinder createBinder() override;
private:
};
}  // namespace nested
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
