#pragma once

#include "aidl/android/aidl/versioned/tests/IFooInterface.h"

#include <android/binder_ibinder.h>

namespace aidl {
namespace android {
namespace aidl {
namespace versioned {
namespace tests {
class BnFooInterface : public ::ndk::BnCInterface<IFooInterface> {
public:
  BnFooInterface();
  virtual ~BnFooInterface();
  ::ndk::ScopedAStatus getInterfaceVersion(int32_t* _aidl_return) final;
  ::ndk::ScopedAStatus getInterfaceHash(std::string* _aidl_return) final;
protected:
  ::ndk::SpAIBinder createBinder() override;
private:
};
}  // namespace tests
}  // namespace versioned
}  // namespace aidl
}  // namespace android
}  // namespace aidl
