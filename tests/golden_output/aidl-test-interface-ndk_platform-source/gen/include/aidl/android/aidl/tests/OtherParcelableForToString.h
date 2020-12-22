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
namespace aidl {
namespace android {
namespace aidl {
namespace tests {
class OtherParcelableForToString {
public:
  typedef std::false_type fixed_size;
  static const char* descriptor;

  std::string field;

  binder_status_t readFromParcel(const AParcel* parcel);
  binder_status_t writeToParcel(AParcel* parcel) const;

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
    os << "OtherParcelableForToString{";
    os << "field: " << (std::ostringstream() << field).str();
    os << "}";
    return os.str();
  }
};
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
