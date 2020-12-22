#pragma once

#include <android/aidl/tests/GenericStructuredParcelable.h>
#include <android/aidl/tests/IntEnum.h>
#include <android/aidl/tests/OtherParcelableForToString.h>
#include <android/aidl/tests/StructuredParcelable.h>
#include <android/aidl/tests/Union.h>
#include <binder/Parcel.h>
#include <binder/Status.h>
#include <codecvt>
#include <cstdint>
#include <locale>
#include <sstream>
#include <tuple>
#include <utils/String16.h>
#include <vector>

namespace android {

namespace aidl {

namespace tests {

class ParcelableForToString : public ::android::Parcelable {
public:
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

  int32_t intValue = int32_t(0);
  ::std::vector<int32_t> intArray;
  int64_t longValue = int64_t(0L);
  ::std::vector<int64_t> longArray;
  double doubleValue = double(0.000000);
  ::std::vector<double> doubleArray;
  float floatValue = float(0.000000f);
  ::std::vector<float> floatArray;
  int8_t byteValue = int8_t(0);
  ::std::vector<uint8_t> byteArray;
  bool booleanValue = bool(false);
  ::std::vector<bool> booleanArray;
  ::android::String16 stringValue;
  ::std::vector<::android::String16> stringArray;
  ::std::vector<::android::String16> stringList;
  ::android::aidl::tests::OtherParcelableForToString parcelableValue;
  ::std::vector<::android::aidl::tests::OtherParcelableForToString> parcelableArray;
  ::android::aidl::tests::IntEnum enumValue;
  ::std::vector<::android::aidl::tests::IntEnum> enumArray;
  ::std::vector<::android::String16> nullArray;
  ::std::vector<::android::String16> nullList;
  ::android::aidl::tests::GenericStructuredParcelable<int32_t, ::android::aidl::tests::StructuredParcelable, ::android::aidl::tests::IntEnum> parcelableGeneric;
  ::android::aidl::tests::Union unionValue;
  ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) override final;
  ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const override final;
  static const std::string& getParcelableDescriptor() {
    static const std::string DESCIPTOR = "android.aidl.tests.ParcelableForToString";
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
};  // class ParcelableForToString

}  // namespace tests

}  // namespace aidl

}  // namespace android
