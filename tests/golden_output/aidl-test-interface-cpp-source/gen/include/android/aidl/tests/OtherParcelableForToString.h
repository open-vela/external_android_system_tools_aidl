#pragma once

#include <binder/Parcel.h>
#include <binder/Status.h>
#include <codecvt>
#include <locale>
#include <sstream>
#include <tuple>
#include <utils/String16.h>

namespace android {

namespace aidl {

namespace tests {

class OtherParcelableForToString : public ::android::Parcelable {
public:
  inline bool operator!=(const OtherParcelableForToString& rhs) const {
    return std::tie(field) != std::tie(rhs.field);
  }
  inline bool operator<(const OtherParcelableForToString& rhs) const {
    return std::tie(field) < std::tie(rhs.field);
  }
  inline bool operator<=(const OtherParcelableForToString& rhs) const {
    return std::tie(field) <= std::tie(rhs.field);
  }
  inline bool operator==(const OtherParcelableForToString& rhs) const {
    return std::tie(field) == std::tie(rhs.field);
  }
  inline bool operator>(const OtherParcelableForToString& rhs) const {
    return std::tie(field) > std::tie(rhs.field);
  }
  inline bool operator>=(const OtherParcelableForToString& rhs) const {
    return std::tie(field) >= std::tie(rhs.field);
  }

  ::android::String16 field;
  ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) override final;
  ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const override final;
  static const ::android::String16& getParcelableDescriptor() {
    static const ::android::StaticString16 DESCIPTOR (u"android.aidl.tests.OtherParcelableForToString");
    return DESCIPTOR;
  }
  template <typename _T> class _has_toString {
    template <typename _U> static std::true_type __has_toString(decltype(&_U::toString));
    template <typename _U> static std::false_type __has_toString(...);
    public: enum { value = decltype(__has_toString<_T>(nullptr))::value };
  };
  template <typename _T> inline static std::string _call_toString(const _T& t) {
    if constexpr (_has_toString<_T>::value) return t.toString();
    return "{no toString() implemented}";
  }
  inline std::string toString() const {
    std::ostringstream os;
    os << "OtherParcelableForToString{";
    os << "field: " << (std::ostringstream() << field).str();
    os << "}";
    return os.str();
  }
};  // class OtherParcelableForToString

}  // namespace tests

}  // namespace aidl

}  // namespace android
