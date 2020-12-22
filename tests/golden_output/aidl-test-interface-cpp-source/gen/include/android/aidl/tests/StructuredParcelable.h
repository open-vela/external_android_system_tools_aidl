#pragma once

#include <android/aidl/tests/ByteEnum.h>
#include <android/aidl/tests/ConstantExpressionEnum.h>
#include <android/aidl/tests/IntEnum.h>
#include <android/aidl/tests/LongEnum.h>
#include <android/aidl/tests/Union.h>
#include <binder/IBinder.h>
#include <binder/Parcel.h>
#include <binder/Status.h>
#include <codecvt>
#include <cstdint>
#include <locale>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <utils/String16.h>
#include <vector>

namespace android {

namespace aidl {

namespace tests {

class StructuredParcelable : public ::android::Parcelable {
public:
  inline bool operator!=(const StructuredParcelable& rhs) const {
    return std::tie(shouldContainThreeFs, f, shouldBeJerry, shouldBeByteBar, shouldBeIntBar, shouldBeLongBar, shouldContainTwoByteFoos, shouldContainTwoIntFoos, shouldContainTwoLongFoos, stringDefaultsToFoo, byteDefaultsToFour, intDefaultsToFive, longDefaultsToNegativeSeven, booleanDefaultsToTrue, charDefaultsToC, floatDefaultsToPi, doubleWithDefault, arrayDefaultsTo123, arrayDefaultsToEmpty, boolDefault, byteDefault, intDefault, longDefault, floatDefault, doubleDefault, checkDoubleFromFloat, checkStringArray1, checkStringArray2, int32_min, int32_max, int64_max, hexInt32_neg_1, ibinder, int32_1, int64_1, hexInt32_pos_1, hexInt64_pos_1, const_exprs_1, const_exprs_2, const_exprs_3, const_exprs_4, const_exprs_5, const_exprs_6, const_exprs_7, const_exprs_8, const_exprs_9, const_exprs_10, addString1, addString2, shouldSetBit0AndBit2, u, shouldBeConstS1, defaultWithFoo) != std::tie(rhs.shouldContainThreeFs, rhs.f, rhs.shouldBeJerry, rhs.shouldBeByteBar, rhs.shouldBeIntBar, rhs.shouldBeLongBar, rhs.shouldContainTwoByteFoos, rhs.shouldContainTwoIntFoos, rhs.shouldContainTwoLongFoos, rhs.stringDefaultsToFoo, rhs.byteDefaultsToFour, rhs.intDefaultsToFive, rhs.longDefaultsToNegativeSeven, rhs.booleanDefaultsToTrue, rhs.charDefaultsToC, rhs.floatDefaultsToPi, rhs.doubleWithDefault, rhs.arrayDefaultsTo123, rhs.arrayDefaultsToEmpty, rhs.boolDefault, rhs.byteDefault, rhs.intDefault, rhs.longDefault, rhs.floatDefault, rhs.doubleDefault, rhs.checkDoubleFromFloat, rhs.checkStringArray1, rhs.checkStringArray2, rhs.int32_min, rhs.int32_max, rhs.int64_max, rhs.hexInt32_neg_1, rhs.ibinder, rhs.int32_1, rhs.int64_1, rhs.hexInt32_pos_1, rhs.hexInt64_pos_1, rhs.const_exprs_1, rhs.const_exprs_2, rhs.const_exprs_3, rhs.const_exprs_4, rhs.const_exprs_5, rhs.const_exprs_6, rhs.const_exprs_7, rhs.const_exprs_8, rhs.const_exprs_9, rhs.const_exprs_10, rhs.addString1, rhs.addString2, rhs.shouldSetBit0AndBit2, rhs.u, rhs.shouldBeConstS1, rhs.defaultWithFoo);
  }
  inline bool operator<(const StructuredParcelable& rhs) const {
    return std::tie(shouldContainThreeFs, f, shouldBeJerry, shouldBeByteBar, shouldBeIntBar, shouldBeLongBar, shouldContainTwoByteFoos, shouldContainTwoIntFoos, shouldContainTwoLongFoos, stringDefaultsToFoo, byteDefaultsToFour, intDefaultsToFive, longDefaultsToNegativeSeven, booleanDefaultsToTrue, charDefaultsToC, floatDefaultsToPi, doubleWithDefault, arrayDefaultsTo123, arrayDefaultsToEmpty, boolDefault, byteDefault, intDefault, longDefault, floatDefault, doubleDefault, checkDoubleFromFloat, checkStringArray1, checkStringArray2, int32_min, int32_max, int64_max, hexInt32_neg_1, ibinder, int32_1, int64_1, hexInt32_pos_1, hexInt64_pos_1, const_exprs_1, const_exprs_2, const_exprs_3, const_exprs_4, const_exprs_5, const_exprs_6, const_exprs_7, const_exprs_8, const_exprs_9, const_exprs_10, addString1, addString2, shouldSetBit0AndBit2, u, shouldBeConstS1, defaultWithFoo) < std::tie(rhs.shouldContainThreeFs, rhs.f, rhs.shouldBeJerry, rhs.shouldBeByteBar, rhs.shouldBeIntBar, rhs.shouldBeLongBar, rhs.shouldContainTwoByteFoos, rhs.shouldContainTwoIntFoos, rhs.shouldContainTwoLongFoos, rhs.stringDefaultsToFoo, rhs.byteDefaultsToFour, rhs.intDefaultsToFive, rhs.longDefaultsToNegativeSeven, rhs.booleanDefaultsToTrue, rhs.charDefaultsToC, rhs.floatDefaultsToPi, rhs.doubleWithDefault, rhs.arrayDefaultsTo123, rhs.arrayDefaultsToEmpty, rhs.boolDefault, rhs.byteDefault, rhs.intDefault, rhs.longDefault, rhs.floatDefault, rhs.doubleDefault, rhs.checkDoubleFromFloat, rhs.checkStringArray1, rhs.checkStringArray2, rhs.int32_min, rhs.int32_max, rhs.int64_max, rhs.hexInt32_neg_1, rhs.ibinder, rhs.int32_1, rhs.int64_1, rhs.hexInt32_pos_1, rhs.hexInt64_pos_1, rhs.const_exprs_1, rhs.const_exprs_2, rhs.const_exprs_3, rhs.const_exprs_4, rhs.const_exprs_5, rhs.const_exprs_6, rhs.const_exprs_7, rhs.const_exprs_8, rhs.const_exprs_9, rhs.const_exprs_10, rhs.addString1, rhs.addString2, rhs.shouldSetBit0AndBit2, rhs.u, rhs.shouldBeConstS1, rhs.defaultWithFoo);
  }
  inline bool operator<=(const StructuredParcelable& rhs) const {
    return std::tie(shouldContainThreeFs, f, shouldBeJerry, shouldBeByteBar, shouldBeIntBar, shouldBeLongBar, shouldContainTwoByteFoos, shouldContainTwoIntFoos, shouldContainTwoLongFoos, stringDefaultsToFoo, byteDefaultsToFour, intDefaultsToFive, longDefaultsToNegativeSeven, booleanDefaultsToTrue, charDefaultsToC, floatDefaultsToPi, doubleWithDefault, arrayDefaultsTo123, arrayDefaultsToEmpty, boolDefault, byteDefault, intDefault, longDefault, floatDefault, doubleDefault, checkDoubleFromFloat, checkStringArray1, checkStringArray2, int32_min, int32_max, int64_max, hexInt32_neg_1, ibinder, int32_1, int64_1, hexInt32_pos_1, hexInt64_pos_1, const_exprs_1, const_exprs_2, const_exprs_3, const_exprs_4, const_exprs_5, const_exprs_6, const_exprs_7, const_exprs_8, const_exprs_9, const_exprs_10, addString1, addString2, shouldSetBit0AndBit2, u, shouldBeConstS1, defaultWithFoo) <= std::tie(rhs.shouldContainThreeFs, rhs.f, rhs.shouldBeJerry, rhs.shouldBeByteBar, rhs.shouldBeIntBar, rhs.shouldBeLongBar, rhs.shouldContainTwoByteFoos, rhs.shouldContainTwoIntFoos, rhs.shouldContainTwoLongFoos, rhs.stringDefaultsToFoo, rhs.byteDefaultsToFour, rhs.intDefaultsToFive, rhs.longDefaultsToNegativeSeven, rhs.booleanDefaultsToTrue, rhs.charDefaultsToC, rhs.floatDefaultsToPi, rhs.doubleWithDefault, rhs.arrayDefaultsTo123, rhs.arrayDefaultsToEmpty, rhs.boolDefault, rhs.byteDefault, rhs.intDefault, rhs.longDefault, rhs.floatDefault, rhs.doubleDefault, rhs.checkDoubleFromFloat, rhs.checkStringArray1, rhs.checkStringArray2, rhs.int32_min, rhs.int32_max, rhs.int64_max, rhs.hexInt32_neg_1, rhs.ibinder, rhs.int32_1, rhs.int64_1, rhs.hexInt32_pos_1, rhs.hexInt64_pos_1, rhs.const_exprs_1, rhs.const_exprs_2, rhs.const_exprs_3, rhs.const_exprs_4, rhs.const_exprs_5, rhs.const_exprs_6, rhs.const_exprs_7, rhs.const_exprs_8, rhs.const_exprs_9, rhs.const_exprs_10, rhs.addString1, rhs.addString2, rhs.shouldSetBit0AndBit2, rhs.u, rhs.shouldBeConstS1, rhs.defaultWithFoo);
  }
  inline bool operator==(const StructuredParcelable& rhs) const {
    return std::tie(shouldContainThreeFs, f, shouldBeJerry, shouldBeByteBar, shouldBeIntBar, shouldBeLongBar, shouldContainTwoByteFoos, shouldContainTwoIntFoos, shouldContainTwoLongFoos, stringDefaultsToFoo, byteDefaultsToFour, intDefaultsToFive, longDefaultsToNegativeSeven, booleanDefaultsToTrue, charDefaultsToC, floatDefaultsToPi, doubleWithDefault, arrayDefaultsTo123, arrayDefaultsToEmpty, boolDefault, byteDefault, intDefault, longDefault, floatDefault, doubleDefault, checkDoubleFromFloat, checkStringArray1, checkStringArray2, int32_min, int32_max, int64_max, hexInt32_neg_1, ibinder, int32_1, int64_1, hexInt32_pos_1, hexInt64_pos_1, const_exprs_1, const_exprs_2, const_exprs_3, const_exprs_4, const_exprs_5, const_exprs_6, const_exprs_7, const_exprs_8, const_exprs_9, const_exprs_10, addString1, addString2, shouldSetBit0AndBit2, u, shouldBeConstS1, defaultWithFoo) == std::tie(rhs.shouldContainThreeFs, rhs.f, rhs.shouldBeJerry, rhs.shouldBeByteBar, rhs.shouldBeIntBar, rhs.shouldBeLongBar, rhs.shouldContainTwoByteFoos, rhs.shouldContainTwoIntFoos, rhs.shouldContainTwoLongFoos, rhs.stringDefaultsToFoo, rhs.byteDefaultsToFour, rhs.intDefaultsToFive, rhs.longDefaultsToNegativeSeven, rhs.booleanDefaultsToTrue, rhs.charDefaultsToC, rhs.floatDefaultsToPi, rhs.doubleWithDefault, rhs.arrayDefaultsTo123, rhs.arrayDefaultsToEmpty, rhs.boolDefault, rhs.byteDefault, rhs.intDefault, rhs.longDefault, rhs.floatDefault, rhs.doubleDefault, rhs.checkDoubleFromFloat, rhs.checkStringArray1, rhs.checkStringArray2, rhs.int32_min, rhs.int32_max, rhs.int64_max, rhs.hexInt32_neg_1, rhs.ibinder, rhs.int32_1, rhs.int64_1, rhs.hexInt32_pos_1, rhs.hexInt64_pos_1, rhs.const_exprs_1, rhs.const_exprs_2, rhs.const_exprs_3, rhs.const_exprs_4, rhs.const_exprs_5, rhs.const_exprs_6, rhs.const_exprs_7, rhs.const_exprs_8, rhs.const_exprs_9, rhs.const_exprs_10, rhs.addString1, rhs.addString2, rhs.shouldSetBit0AndBit2, rhs.u, rhs.shouldBeConstS1, rhs.defaultWithFoo);
  }
  inline bool operator>(const StructuredParcelable& rhs) const {
    return std::tie(shouldContainThreeFs, f, shouldBeJerry, shouldBeByteBar, shouldBeIntBar, shouldBeLongBar, shouldContainTwoByteFoos, shouldContainTwoIntFoos, shouldContainTwoLongFoos, stringDefaultsToFoo, byteDefaultsToFour, intDefaultsToFive, longDefaultsToNegativeSeven, booleanDefaultsToTrue, charDefaultsToC, floatDefaultsToPi, doubleWithDefault, arrayDefaultsTo123, arrayDefaultsToEmpty, boolDefault, byteDefault, intDefault, longDefault, floatDefault, doubleDefault, checkDoubleFromFloat, checkStringArray1, checkStringArray2, int32_min, int32_max, int64_max, hexInt32_neg_1, ibinder, int32_1, int64_1, hexInt32_pos_1, hexInt64_pos_1, const_exprs_1, const_exprs_2, const_exprs_3, const_exprs_4, const_exprs_5, const_exprs_6, const_exprs_7, const_exprs_8, const_exprs_9, const_exprs_10, addString1, addString2, shouldSetBit0AndBit2, u, shouldBeConstS1, defaultWithFoo) > std::tie(rhs.shouldContainThreeFs, rhs.f, rhs.shouldBeJerry, rhs.shouldBeByteBar, rhs.shouldBeIntBar, rhs.shouldBeLongBar, rhs.shouldContainTwoByteFoos, rhs.shouldContainTwoIntFoos, rhs.shouldContainTwoLongFoos, rhs.stringDefaultsToFoo, rhs.byteDefaultsToFour, rhs.intDefaultsToFive, rhs.longDefaultsToNegativeSeven, rhs.booleanDefaultsToTrue, rhs.charDefaultsToC, rhs.floatDefaultsToPi, rhs.doubleWithDefault, rhs.arrayDefaultsTo123, rhs.arrayDefaultsToEmpty, rhs.boolDefault, rhs.byteDefault, rhs.intDefault, rhs.longDefault, rhs.floatDefault, rhs.doubleDefault, rhs.checkDoubleFromFloat, rhs.checkStringArray1, rhs.checkStringArray2, rhs.int32_min, rhs.int32_max, rhs.int64_max, rhs.hexInt32_neg_1, rhs.ibinder, rhs.int32_1, rhs.int64_1, rhs.hexInt32_pos_1, rhs.hexInt64_pos_1, rhs.const_exprs_1, rhs.const_exprs_2, rhs.const_exprs_3, rhs.const_exprs_4, rhs.const_exprs_5, rhs.const_exprs_6, rhs.const_exprs_7, rhs.const_exprs_8, rhs.const_exprs_9, rhs.const_exprs_10, rhs.addString1, rhs.addString2, rhs.shouldSetBit0AndBit2, rhs.u, rhs.shouldBeConstS1, rhs.defaultWithFoo);
  }
  inline bool operator>=(const StructuredParcelable& rhs) const {
    return std::tie(shouldContainThreeFs, f, shouldBeJerry, shouldBeByteBar, shouldBeIntBar, shouldBeLongBar, shouldContainTwoByteFoos, shouldContainTwoIntFoos, shouldContainTwoLongFoos, stringDefaultsToFoo, byteDefaultsToFour, intDefaultsToFive, longDefaultsToNegativeSeven, booleanDefaultsToTrue, charDefaultsToC, floatDefaultsToPi, doubleWithDefault, arrayDefaultsTo123, arrayDefaultsToEmpty, boolDefault, byteDefault, intDefault, longDefault, floatDefault, doubleDefault, checkDoubleFromFloat, checkStringArray1, checkStringArray2, int32_min, int32_max, int64_max, hexInt32_neg_1, ibinder, int32_1, int64_1, hexInt32_pos_1, hexInt64_pos_1, const_exprs_1, const_exprs_2, const_exprs_3, const_exprs_4, const_exprs_5, const_exprs_6, const_exprs_7, const_exprs_8, const_exprs_9, const_exprs_10, addString1, addString2, shouldSetBit0AndBit2, u, shouldBeConstS1, defaultWithFoo) >= std::tie(rhs.shouldContainThreeFs, rhs.f, rhs.shouldBeJerry, rhs.shouldBeByteBar, rhs.shouldBeIntBar, rhs.shouldBeLongBar, rhs.shouldContainTwoByteFoos, rhs.shouldContainTwoIntFoos, rhs.shouldContainTwoLongFoos, rhs.stringDefaultsToFoo, rhs.byteDefaultsToFour, rhs.intDefaultsToFive, rhs.longDefaultsToNegativeSeven, rhs.booleanDefaultsToTrue, rhs.charDefaultsToC, rhs.floatDefaultsToPi, rhs.doubleWithDefault, rhs.arrayDefaultsTo123, rhs.arrayDefaultsToEmpty, rhs.boolDefault, rhs.byteDefault, rhs.intDefault, rhs.longDefault, rhs.floatDefault, rhs.doubleDefault, rhs.checkDoubleFromFloat, rhs.checkStringArray1, rhs.checkStringArray2, rhs.int32_min, rhs.int32_max, rhs.int64_max, rhs.hexInt32_neg_1, rhs.ibinder, rhs.int32_1, rhs.int64_1, rhs.hexInt32_pos_1, rhs.hexInt64_pos_1, rhs.const_exprs_1, rhs.const_exprs_2, rhs.const_exprs_3, rhs.const_exprs_4, rhs.const_exprs_5, rhs.const_exprs_6, rhs.const_exprs_7, rhs.const_exprs_8, rhs.const_exprs_9, rhs.const_exprs_10, rhs.addString1, rhs.addString2, rhs.shouldSetBit0AndBit2, rhs.u, rhs.shouldBeConstS1, rhs.defaultWithFoo);
  }

  ::std::vector<int32_t> shouldContainThreeFs;
  int32_t f = int32_t(0);
  ::std::string shouldBeJerry;
  ::android::aidl::tests::ByteEnum shouldBeByteBar;
  ::android::aidl::tests::IntEnum shouldBeIntBar;
  ::android::aidl::tests::LongEnum shouldBeLongBar;
  ::std::vector<::android::aidl::tests::ByteEnum> shouldContainTwoByteFoos;
  ::std::vector<::android::aidl::tests::IntEnum> shouldContainTwoIntFoos;
  ::std::vector<::android::aidl::tests::LongEnum> shouldContainTwoLongFoos;
  ::android::String16 stringDefaultsToFoo = ::android::String16(::android::String16("foo"));
  int8_t byteDefaultsToFour = int8_t(4);
  int32_t intDefaultsToFive = int32_t(5);
  int64_t longDefaultsToNegativeSeven = int64_t(-7L);
  bool booleanDefaultsToTrue = bool(true);
  char16_t charDefaultsToC = char16_t('C');
  float floatDefaultsToPi = float(3.140000f);
  double doubleWithDefault = double(-314000000000000000.000000);
  ::std::vector<int32_t> arrayDefaultsTo123 = ::std::vector<int32_t>({1, 2, 3});
  ::std::vector<int32_t> arrayDefaultsToEmpty = ::std::vector<int32_t>({});
  bool boolDefault = bool(false);
  int8_t byteDefault = int8_t(0);
  int32_t intDefault = int32_t(0);
  int64_t longDefault = int64_t(0L);
  float floatDefault = float(0.000000f);
  double doubleDefault = double(0.000000);
  double checkDoubleFromFloat = double(3.140000);
  ::std::vector<::android::String16> checkStringArray1 = ::std::vector<::android::String16>({::android::String16("a"), ::android::String16("b")});
  ::std::vector<::std::string> checkStringArray2 = ::std::vector<::std::string>({"a", "b"});
  int32_t int32_min = int32_t(-2147483648);
  int32_t int32_max = int32_t(2147483647);
  int64_t int64_max = int64_t(9223372036854775807L);
  int32_t hexInt32_neg_1 = int32_t(-1);
  ::android::sp<::android::IBinder> ibinder;
  ::std::vector<int32_t> int32_1 = ::std::vector<int32_t>({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
  ::std::vector<int64_t> int64_1 = ::std::vector<int64_t>({1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L});
  int32_t hexInt32_pos_1 = int32_t(1);
  int32_t hexInt64_pos_1 = int32_t(1);
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_1;
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_2;
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_3;
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_4;
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_5;
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_6;
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_7;
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_8;
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_9;
  ::android::aidl::tests::ConstantExpressionEnum const_exprs_10;
  ::std::string addString1 = ::std::string("hello world!");
  ::std::string addString2 = ::std::string("The quick brown fox jumps over the lazy dog.");
  int32_t shouldSetBit0AndBit2 = int32_t(0);
  ::std::optional<::android::aidl::tests::Union> u;
  ::std::optional<::android::aidl::tests::Union> shouldBeConstS1;
  ::android::aidl::tests::IntEnum defaultWithFoo = ::android::aidl::tests::IntEnum(::android::aidl::tests::IntEnum::FOO);
  enum  : int32_t {
    BIT0 = 1,
    BIT1 = 2,
    BIT2 = 4,
  };
  ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) override final;
  ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const override final;
  static const std::string& getParcelableDescriptor() {
    static const std::string DESCIPTOR = "android.aidl.tests.StructuredParcelable";
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
    os << "StructuredParcelable{";
    os << "shouldContainThreeFs: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: shouldContainThreeFs) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", f: " << std::to_string(f);
    os << ", shouldBeJerry: " << (std::ostringstream() << shouldBeJerry).str();
    os << ", shouldBeByteBar: " << android::aidl::tests::toString(shouldBeByteBar);
    os << ", shouldBeIntBar: " << android::aidl::tests::toString(shouldBeIntBar);
    os << ", shouldBeLongBar: " << android::aidl::tests::toString(shouldBeLongBar);
    os << ", shouldContainTwoByteFoos: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: shouldContainTwoByteFoos) { (void)v; if (first) first = false; else o << ", "; o << android::aidl::tests::toString(v); }; o << "]"; return o.str(); }();
    os << ", shouldContainTwoIntFoos: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: shouldContainTwoIntFoos) { (void)v; if (first) first = false; else o << ", "; o << android::aidl::tests::toString(v); }; o << "]"; return o.str(); }();
    os << ", shouldContainTwoLongFoos: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: shouldContainTwoLongFoos) { (void)v; if (first) first = false; else o << ", "; o << android::aidl::tests::toString(v); }; o << "]"; return o.str(); }();
    os << ", stringDefaultsToFoo: " << (std::ostringstream() << stringDefaultsToFoo).str();
    os << ", byteDefaultsToFour: " << std::to_string(byteDefaultsToFour);
    os << ", intDefaultsToFive: " << std::to_string(intDefaultsToFive);
    os << ", longDefaultsToNegativeSeven: " << std::to_string(longDefaultsToNegativeSeven);
    os << ", booleanDefaultsToTrue: " << (booleanDefaultsToTrue?"true":"false");
    os << ", charDefaultsToC: " << std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>().to_bytes(charDefaultsToC);
    os << ", floatDefaultsToPi: " << std::to_string(floatDefaultsToPi);
    os << ", doubleWithDefault: " << std::to_string(doubleWithDefault);
    os << ", arrayDefaultsTo123: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: arrayDefaultsTo123) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", arrayDefaultsToEmpty: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: arrayDefaultsToEmpty) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", boolDefault: " << (boolDefault?"true":"false");
    os << ", byteDefault: " << std::to_string(byteDefault);
    os << ", intDefault: " << std::to_string(intDefault);
    os << ", longDefault: " << std::to_string(longDefault);
    os << ", floatDefault: " << std::to_string(floatDefault);
    os << ", doubleDefault: " << std::to_string(doubleDefault);
    os << ", checkDoubleFromFloat: " << std::to_string(checkDoubleFromFloat);
    os << ", checkStringArray1: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: checkStringArray1) { (void)v; if (first) first = false; else o << ", "; o << (std::ostringstream() << v).str(); }; o << "]"; return o.str(); }();
    os << ", checkStringArray2: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: checkStringArray2) { (void)v; if (first) first = false; else o << ", "; o << (std::ostringstream() << v).str(); }; o << "]"; return o.str(); }();
    os << ", int32_min: " << std::to_string(int32_min);
    os << ", int32_max: " << std::to_string(int32_max);
    os << ", int64_max: " << std::to_string(int64_max);
    os << ", hexInt32_neg_1: " << std::to_string(hexInt32_neg_1);
    os << ", ibinder: " << "";
    os << ", int32_1: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: int32_1) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", int64_1: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: int64_1) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }();
    os << ", hexInt32_pos_1: " << std::to_string(hexInt32_pos_1);
    os << ", hexInt64_pos_1: " << std::to_string(hexInt64_pos_1);
    os << ", const_exprs_1: " << android::aidl::tests::toString(const_exprs_1);
    os << ", const_exprs_2: " << android::aidl::tests::toString(const_exprs_2);
    os << ", const_exprs_3: " << android::aidl::tests::toString(const_exprs_3);
    os << ", const_exprs_4: " << android::aidl::tests::toString(const_exprs_4);
    os << ", const_exprs_5: " << android::aidl::tests::toString(const_exprs_5);
    os << ", const_exprs_6: " << android::aidl::tests::toString(const_exprs_6);
    os << ", const_exprs_7: " << android::aidl::tests::toString(const_exprs_7);
    os << ", const_exprs_8: " << android::aidl::tests::toString(const_exprs_8);
    os << ", const_exprs_9: " << android::aidl::tests::toString(const_exprs_9);
    os << ", const_exprs_10: " << android::aidl::tests::toString(const_exprs_10);
    os << ", addString1: " << (std::ostringstream() << addString1).str();
    os << ", addString2: " << (std::ostringstream() << addString2).str();
    os << ", shouldSetBit0AndBit2: " << std::to_string(shouldSetBit0AndBit2);
    os << ", u: " << ((u) ? _call_toString(*u): "(null)");
    os << ", shouldBeConstS1: " << ((shouldBeConstS1) ? _call_toString(*shouldBeConstS1): "(null)");
    os << ", defaultWithFoo: " << android::aidl::tests::toString(defaultWithFoo);
    os << "}";
    return os.str();
  }
};  // class StructuredParcelable

}  // namespace tests

}  // namespace aidl

}  // namespace android
