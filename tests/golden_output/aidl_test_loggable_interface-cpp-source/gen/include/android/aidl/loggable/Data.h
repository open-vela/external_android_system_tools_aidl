#pragma once

#include <android/aidl/loggable/Enum.h>
#include <android/aidl/loggable/Union.h>
#include <binder/Parcel.h>
#include <binder/Status.h>
#include <codecvt>
#include <cstdint>
#include <locale>
#include <sstream>
#include <string>
#include <tuple>
#include <utils/String16.h>

namespace android {

namespace aidl {

namespace loggable {

class Data : public ::android::Parcelable {
public:
  inline bool operator!=(const Data& rhs) const {
    return std::tie(num, str, nestedUnion, nestedEnum) != std::tie(rhs.num, rhs.str, rhs.nestedUnion, rhs.nestedEnum);
  }
  inline bool operator<(const Data& rhs) const {
    return std::tie(num, str, nestedUnion, nestedEnum) < std::tie(rhs.num, rhs.str, rhs.nestedUnion, rhs.nestedEnum);
  }
  inline bool operator<=(const Data& rhs) const {
    return std::tie(num, str, nestedUnion, nestedEnum) <= std::tie(rhs.num, rhs.str, rhs.nestedUnion, rhs.nestedEnum);
  }
  inline bool operator==(const Data& rhs) const {
    return std::tie(num, str, nestedUnion, nestedEnum) == std::tie(rhs.num, rhs.str, rhs.nestedUnion, rhs.nestedEnum);
  }
  inline bool operator>(const Data& rhs) const {
    return std::tie(num, str, nestedUnion, nestedEnum) > std::tie(rhs.num, rhs.str, rhs.nestedUnion, rhs.nestedEnum);
  }
  inline bool operator>=(const Data& rhs) const {
    return std::tie(num, str, nestedUnion, nestedEnum) >= std::tie(rhs.num, rhs.str, rhs.nestedUnion, rhs.nestedEnum);
  }

  int32_t num = int32_t(0);
  ::std::string str;
  ::android::aidl::loggable::Union nestedUnion;
  ::android::aidl::loggable::Enum nestedEnum;
  ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) override final;
  ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const override final;
  static const ::android::String16& getParcelableDescriptor() {
    static const ::android::StaticString16 DESCIPTOR (u"android.aidl.loggable.Data");
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
    os << "Data{";
    os << "num: " << std::to_string(num);
    os << ", str: " << (std::ostringstream() << str).str();
    os << ", nestedUnion: " << _call_toString(nestedUnion);
    os << ", nestedEnum: " << android::aidl::loggable::toString(nestedEnum);
    os << "}";
    return os.str();
  }
};  // class Data

}  // namespace loggable

}  // namespace aidl

}  // namespace android
