#pragma once
#include <android/binder_interface_utils.h>
#include <android/binder_parcelable_utils.h>
#include <codecvt>
#include <locale>
#include <sstream>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#ifdef BINDER_STABILITY_SUPPORT
#include <android/binder_stability.h>
#endif  // BINDER_STABILITY_SUPPORT
#include <aidl/android/aidl/loggable/Enum.h>
#include <aidl/android/aidl/loggable/Union.h>
namespace aidl {
namespace android {
namespace aidl {
namespace loggable {
class Data {
public:
  typedef std::false_type fixed_size;
  static const char* descriptor;

  int32_t num = 0;
  std::string str;
  ::aidl::android::aidl::loggable::Union nestedUnion;
  ::aidl::android::aidl::loggable::Enum nestedEnum;

  binder_status_t readFromParcel(const AParcel* parcel);
  binder_status_t writeToParcel(AParcel* parcel) const;

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

  static const ::ndk::parcelable_stability_t _aidl_stability = ::ndk::STABILITY_LOCAL;
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
};
}  // namespace loggable
}  // namespace aidl
}  // namespace android
}  // namespace aidl
