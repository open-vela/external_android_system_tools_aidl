#pragma once

#include <android/aidl/tests/nested/INestedService.h>
#include <android/aidl/tests/nested/ParcelableWithNested.h>
#include <android/binder_to_string.h>
#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/Status.h>
#include <tuple>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

namespace android {
namespace aidl {
namespace tests {
namespace nested {
class INestedService : public ::android::IInterface {
public:
  DECLARE_META_INTERFACE(NestedService)
  class Result : public ::android::Parcelable {
  public:
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

    ::android::aidl::tests::nested::ParcelableWithNested::Status status = ::android::aidl::tests::nested::ParcelableWithNested::Status(::android::aidl::tests::nested::ParcelableWithNested::Status::OK);
    ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) final;
    ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const final;
    static const ::android::String16& getParcelableDescriptor() {
      static const ::android::StaticString16 DESCIPTOR (u"android.aidl.tests.nested.INestedService.Result");
      return DESCIPTOR;
    }
    inline std::string toString() const {
      std::ostringstream os;
      os << "Result{";
      os << "status: " << ::android::internal::ToString(status);
      os << "}";
      return os.str();
    }
  };  // class Result
  virtual ::android::binder::Status flipStatus(const ::android::aidl::tests::nested::ParcelableWithNested& p, ::android::aidl::tests::nested::INestedService::Result* _aidl_return) = 0;
};  // class INestedService

class INestedServiceDefault : public INestedService {
public:
  ::android::IBinder* onAsBinder() override {
    return nullptr;
  }
  ::android::binder::Status flipStatus(const ::android::aidl::tests::nested::ParcelableWithNested&, ::android::aidl::tests::nested::INestedService::Result*) override {
    return ::android::binder::Status::fromStatusT(::android::UNKNOWN_TRANSACTION);
  }
};  // class INestedServiceDefault
}  // namespace nested
}  // namespace tests
}  // namespace aidl
}  // namespace android
