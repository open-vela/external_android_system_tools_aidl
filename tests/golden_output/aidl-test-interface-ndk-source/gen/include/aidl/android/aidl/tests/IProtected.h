#pragma once

#include <android/binder_interface_utils.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#ifdef BINDER_STABILITY_SUPPORT
#include <android/binder_stability.h>
#endif  // BINDER_STABILITY_SUPPORT

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
class IProtected : public ::ndk::ICInterface {
public:
  static const char* descriptor;
  IProtected();
  virtual ~IProtected();

  static constexpr uint32_t TRANSACTION_PermissionProtected = FIRST_CALL_TRANSACTION + 0;
  static constexpr uint32_t TRANSACTION_MultiplePermissions = FIRST_CALL_TRANSACTION + 1;
  static constexpr uint32_t TRANSACTION_MultiplePermissions2 = FIRST_CALL_TRANSACTION + 2;

  static std::shared_ptr<IProtected> fromBinder(const ::ndk::SpAIBinder& binder);
  static binder_status_t writeToParcel(AParcel* parcel, const std::shared_ptr<IProtected>& instance);
  static binder_status_t readFromParcel(const AParcel* parcel, std::shared_ptr<IProtected>* instance);
  static bool setDefaultImpl(const std::shared_ptr<IProtected>& impl);
  static const std::shared_ptr<IProtected>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus PermissionProtected() = 0;
  virtual ::ndk::ScopedAStatus MultiplePermissions() = 0;
  virtual ::ndk::ScopedAStatus MultiplePermissions2() = 0;
private:
  static std::shared_ptr<IProtected> default_impl;
};
class IProtectedDefault : public IProtected {
public:
  ::ndk::ScopedAStatus PermissionProtected() override;
  ::ndk::ScopedAStatus MultiplePermissions() override;
  ::ndk::ScopedAStatus MultiplePermissions2() override;
  ::ndk::SpAIBinder asBinder() override;
  bool isRemote() override;
};
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
