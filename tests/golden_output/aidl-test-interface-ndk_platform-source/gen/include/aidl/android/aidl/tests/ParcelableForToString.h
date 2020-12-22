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
#include <aidl/android/aidl/tests/GenericStructuredParcelable.h>
#include <aidl/android/aidl/tests/IntEnum.h>
#include <aidl/android/aidl/tests/OtherParcelableForToString.h>
#include <aidl/android/aidl/tests/StructuredParcelable.h>
#include <aidl/android/aidl/tests/Union.h>
namespace aidl {
namespace android {
namespace aidl {
namespace tests {
class ParcelableForToString {
public:
  typedef std::false_type fixed_size;
  static const char* descriptor;

  int32_t intValue = 0;
  std::vector<int32_t> intArray;
  int64_t longValue = 0L;
  std::vector<int64_t> longArray;
  double doubleValue = 0.000000;
  std::vector<double> doubleArray;
  float floatValue = 0.000000f;
  std::vector<float> floatArray;
  int8_t byteValue = 0;
  std::vector<uint8_t> byteArray;
  bool booleanValue = false;
  std::vector<bool> booleanArray;
  std::string stringValue;
  std::vector<std::string> stringArray;
  std::vector<std::string> stringList;
  ::aidl::android::aidl::tests::OtherParcelableForToString parcelableValue;
  std::vector<::aidl::android::aidl::tests::OtherParcelableForToString> parcelableArray;
  ::aidl::android::aidl::tests::IntEnum enumValue;
  std::vector<::aidl::android::aidl::tests::IntEnum> enumArray;
  std::vector<std::string> nullArray;
  std::vector<std::string> nullList;
  ::aidl::android::aidl::tests::GenericStructuredParcelable<int32_t, ::aidl::android::aidl::tests::StructuredParcelable, ::aidl::android::aidl::tests::IntEnum> parcelableGeneric;
  ::aidl::android::aidl::tests::Union unionValue;

  binder_status_t readFromParcel(const AParcel* parcel);
  binder_status_t writeToParcel(AParcel* parcel) const;

  inline bool operator!=(const ParcelableForToString& rhs) const {
    return std::tie(intValue, intArray, longValue, longArray, doubleValue, doubleArray, floatValue, floatArray, byteValue, byteArray, booleanValue, booleanArray, stringValue, stringArray, stringList, parcelableValue, parcelableArray, enumValue, enumArray, nullArray, nullList, parcelableGeneric, unionValue) != std::tie(rhs.intValue, rhs.intArray, rhs.longValue, rhs.longArray, rhs.doubleValue, rhs.doubleArray, rhs.floatValue, rhs.floatArray, rhs.byteValue, rhs.byteArray, rhs.booleanValue, rhs.booleanArray, rhs.stringValue, rhs.stringArray, rhs.stringList, rhs.parcelableValue, rhs.parcelableArray, rhs.enumValue, rhs.enumArray, rhs.nullArray, rhs.nullList, rhs.parcelableGeneric, rhs.unionValue);
  }
  inline bool operator<(const ParcelableForToString& rhs) const {
    return std::tie(intValue, intArray, longValue, longArray, doubleValue, doubleArray, floatValue, floatArray, byteValue, byteArray, booleanValue, booleanArray, stringValue, stringArray, stringList, parcelableValue, parcelableArray, enumValue, enumArray, nullArray, nullList, parcelableGeneric, unionValue) < std::tie(rhs.intValue, rhs.intArray, rhs.longValue, rhs.longArray, rhs.doubleValue, rhs.doubleArray, rhs.floatValue, rhs.floatArray, rhs.byteValue, rhs.byteArray, rhs.booleanValue, rhs.booleanArray, rhs.stringValue, rhs.stringArray, rhs.stringList, rhs.parcelableValue, rhs.parcelableArray, rhs.enumValue, rhs.enumArray, rhs.nullArray, rhs.nullList, rhs.parcelableGeneric, rhs.unionValue);
  }
  inline bool operator<=(const ParcelableForToString& rhs) const {
    return std::tie(intValue, intArray, longValue, longArray, doubleValue, doubleArray, floatValue, floatArray, byteValue, byteArray, booleanValue, booleanArray, stringValue, stringArray, stringList, parcelableValue, parcelableArray, enumValue, enumArray, nullArray, nullList, parcelableGeneric, unionValue) <= std::tie(rhs.intValue, rhs.intArray, rhs.longValue, rhs.longArray, rhs.doubleValue, rhs.doubleArray, rhs.floatValue, rhs.floatArray, rhs.byteValue, rhs.byteArray, rhs.booleanValue, rhs.booleanArray, rhs.stringValue, rhs.stringArray, rhs.stringList, rhs.parcelableValue, rhs.parcelableArray, rhs.enumValue, rhs.enumArray, rhs.nullArray, rhs.nullList, rhs.parcelableGeneric, rhs.unionValue);
  }
  inline bool operator==(const ParcelableForToString& rhs) const {
    return std::tie(intValue, intArray, longValue, longArray, doubleValue, doubleArray, floatValue, floatArray, byteValue, byteArray, booleanValue, booleanArray, stringValue, stringArray, stringList, parcelableValue, parcelableArray, enumValue, enumArray, nullArray, nullList, parcelableGeneric, unionValue) == std::tie(rhs.intValue, rhs.intArray, rhs.longValue, rhs.longArray, rhs.doubleValue, rhs.doubleArray, rhs.floatValue, rhs.floatArray, rhs.byteValue, rhs.byteArray, rhs.booleanValue, rhs.booleanArray, rhs.stringValue, rhs.stringArray, rhs.stringList, rhs.parcelableValue, rhs.parcelableArray, rhs.enumValue, rhs.enumArray, rhs.nullArray, rhs.nullList, rhs.parcelableGeneric, rhs.unionValue);
  }
  inline bool operator>(const ParcelableForToString& rhs) const {
    return std::tie(intValue, intArray, longValue, longArray, doubleValue, doubleArray, floatValue, floatArray, byteValue, byteArray, booleanValue, booleanArray, stringValue, stringArray, stringList, parcelableValue, parcelableArray, enumValue, enumArray, nullArray, nullList, parcelableGeneric, unionValue) > std::tie(rhs.intValue, rhs.intArray, rhs.longValue, rhs.longArray, rhs.doubleValue, rhs.doubleArray, rhs.floatValue, rhs.floatArray, rhs.byteValue, rhs.byteArray, rhs.booleanValue, rhs.booleanArray, rhs.stringValue, rhs.stringArray, rhs.stringList, rhs.parcelableValue, rhs.parcelableArray, rhs.enumValue, rhs.enumArray, rhs.nullArray, rhs.nullList, rhs.parcelableGeneric, rhs.unionValue);
  }
  inline bool operator>=(const ParcelableForToString& rhs) const {
    return std::tie(intValue, intArray, longValue, longArray, doubleValue, doubleArray, floatValue, floatArray, byteValue, byteArray, booleanValue, booleanArray, stringValue, stringArray, stringList, parcelableValue, parcelableArray, enumValue, enumArray, nullArray, nullList, parcelableGeneric, unionValue) >= std::tie(rhs.intValue, rhs.intArray, rhs.longValue, rhs.longArray, rhs.doubleValue, rhs.doubleArray, rhs.floatValue, rhs.floatArray, rhs.byteValue, rhs.byteArray, rhs.booleanValue, rhs.booleanArray, rhs.stringValue, rhs.stringArray, rhs.stringList, rhs.parcelableValue, rhs.parcelableArray, rhs.enumValue, rhs.enumArray, rhs.nullArray, rhs.nullList, rhs.parcelableGeneric, rhs.unionValue);
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
    os << "ParcelableForToString{";
    os << "intValue: " << std::to_string(intValue);
    os << ", intArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: intArray) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", longValue: " << std::to_string(longValue);
    os << ", longArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: longArray) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", doubleValue: " << std::to_string(doubleValue);
    os << ", doubleArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: doubleArray) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", floatValue: " << std::to_string(floatValue);
    os << ", floatArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: floatArray) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", byteValue: " << std::to_string(byteValue);
    os << ", byteArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: byteArray) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", booleanValue: " << (booleanValue?"true":"false");
    os << ", booleanArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: booleanArray) { (void)v; if (first) first = false; else o << ", "; o << (v?"true":"false"); }; o << "]"; return o.str(); }();
    os << ", stringValue: " << (std::ostringstream() << stringValue).str();
    os << ", stringArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: stringArray) { (void)v; if (first) first = false; else o << ", "; o << (std::ostringstream() << v).str(); }; o << "]"; return o.str(); }();
    os << ", stringList: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: stringList) { (void)v; if (first) first = false; else o << ", "; o << (std::ostringstream() << v).str(); }; o << "]"; return o.str(); }();
    os << ", parcelableValue: " << _call_toString(parcelableValue);
    os << ", parcelableArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: parcelableArray) { (void)v; if (first) first = false; else o << ", "; o << _call_toString(v); }; o << "]"; return o.str(); }();
    os << ", enumValue: " << android::aidl::tests::toString(enumValue);
    os << ", enumArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: enumArray) { (void)v; if (first) first = false; else o << ", "; o << android::aidl::tests::toString(v); }; o << "]"; return o.str(); }();
    os << ", nullArray: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: nullArray) { (void)v; if (first) first = false; else o << ", "; o << (std::ostringstream() << v).str(); }; o << "]"; return o.str(); }();
    os << ", nullList: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: nullList) { (void)v; if (first) first = false; else o << ", "; o << (std::ostringstream() << v).str(); }; o << "]"; return o.str(); }();
    os << ", parcelableGeneric: " << _call_toString(parcelableGeneric);
    os << ", unionValue: " << _call_toString(unionValue);
    os << "}";
    return os.str();
  }
};
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
