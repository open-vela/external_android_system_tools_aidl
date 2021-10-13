#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <android/binder_interface_utils.h>
#include <android/binder_parcelable_utils.h>
#include <android/binder_to_string.h>
#include <aidl/android/aidl/tests/nested/INestedService.h>
#include <aidl/android/aidl/tests/nested/ParcelableWithNested.h>
#ifdef BINDER_STABILITY_SUPPORT
#include <android/binder_stability.h>
#endif  // BINDER_STABILITY_SUPPORT

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
namespace nested {
class INestedService : public ::ndk::ICInterface {
public:
  static const char* descriptor;
  INestedService();
  virtual ~INestedService();

  class Result {
  public:
    typedef std::false_type fixed_size;
    static const char* descriptor;

    ::aidl::android::aidl::tests::nested::ParcelableWithNested::Status status = ::aidl::android::aidl::tests::nested::ParcelableWithNested::Status::OK;

    binder_status_t readFromParcel(const AParcel* parcel);
    binder_status_t writeToParcel(AParcel* parcel) const;

    inline bool operator!=(const Result& rhs) const {
      return std::tie(status) != std::tie(rhs.status);
    }
    inline bool operator<(const Result& rhs) const {
      return std::tie(status) < std::tie(rhs.status);
    }
    inline bool operator<=(const Result& rhs) const {
      return std::tie(status) <= std::tie(rhs.status);
    }
    inline bool operator==(const Result& rhs) const {
      return std::tie(status) == std::tie(rhs.status);
    }
    inline bool operator>(const Result& rhs) const {
      return std::tie(status) > std::tie(rhs.status);
    }
    inline bool operator>=(const Result& rhs) const {
      return std::tie(status) >= std::tie(rhs.status);
    }

    static const ::ndk::parcelable_stability_t _aidl_stability = ::ndk::STABILITY_LOCAL;
    inline std::string toString() const {
      std::ostringstream os;
      os << "Result{";
      os << "status: " << ::android::internal::ToString(status);
      os << "}";
      return os.str();
    }
  };
  static constexpr uint32_t TRANSACTION_flipStatus = FIRST_CALL_TRANSACTION + 0;

  static std::shared_ptr<INestedService> fromBinder(const ::ndk::SpAIBinder& binder);
  static binder_status_t writeToParcel(AParcel* parcel, const std::shared_ptr<INestedService>& instance);
  static binder_status_t readFromParcel(const AParcel* parcel, std::shared_ptr<INestedService>* instance);
  static bool setDefaultImpl(const std::shared_ptr<INestedService>& impl);
  static const std::shared_ptr<INestedService>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus flipStatus(const ::aidl::android::aidl::tests::nested::ParcelableWithNested& in_p, ::aidl::android::aidl::tests::nested::INestedService::Result* _aidl_return) = 0;
private:
  static std::shared_ptr<INestedService> default_impl;
};
class INestedServiceDefault : public INestedService {
public:
  ::ndk::ScopedAStatus flipStatus(const ::aidl::android::aidl::tests::nested::ParcelableWithNested& in_p, ::aidl::android::aidl::tests::nested::INestedService::Result* _aidl_return) override;
  ::ndk::SpAIBinder asBinder() override;
  bool isRemote() override;
};
}  // namespace nested
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
