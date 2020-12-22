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
#include <aidl/android/aidl/tests/ByteEnum.h>
#include <aidl/android/aidl/tests/ConstantExpressionEnum.h>
#include <aidl/android/aidl/tests/IntEnum.h>
#include <aidl/android/aidl/tests/LongEnum.h>
#include <aidl/android/aidl/tests/Union.h>
namespace aidl {
namespace android {
namespace aidl {
namespace tests {
class StructuredParcelable {
public:
  typedef std::false_type fixed_size;
  static const char* descriptor;

  std::vector<int32_t> shouldContainThreeFs;
  int32_t f = 0;
  std::string shouldBeJerry;
  ::aidl::android::aidl::tests::ByteEnum shouldBeByteBar;
  ::aidl::android::aidl::tests::IntEnum shouldBeIntBar;
  ::aidl::android::aidl::tests::LongEnum shouldBeLongBar;
  std::vector<::aidl::android::aidl::tests::ByteEnum> shouldContainTwoByteFoos;
  std::vector<::aidl::android::aidl::tests::IntEnum> shouldContainTwoIntFoos;
  std::vector<::aidl::android::aidl::tests::LongEnum> shouldContainTwoLongFoos;
  std::string stringDefaultsToFoo = "foo";
  int8_t byteDefaultsToFour = 4;
  int32_t intDefaultsToFive = 5;
  int64_t longDefaultsToNegativeSeven = -7L;
  bool booleanDefaultsToTrue = true;
  char16_t charDefaultsToC = 'C';
  float floatDefaultsToPi = 3.140000f;
  double doubleWithDefault = -314000000000000000.000000;
  std::vector<int32_t> arrayDefaultsTo123 = {1, 2, 3};
  std::vector<int32_t> arrayDefaultsToEmpty = {};
  bool boolDefault = false;
  int8_t byteDefault = 0;
  int32_t intDefault = 0;
  int64_t longDefault = 0L;
  float floatDefault = 0.000000f;
  double doubleDefault = 0.000000;
  double checkDoubleFromFloat = 3.140000;
  std::vector<std::string> checkStringArray1 = {"a", "b"};
  std::vector<std::string> checkStringArray2 = {"a", "b"};
  int32_t int32_min = -2147483648;
  int32_t int32_max = 2147483647;
  int64_t int64_max = 9223372036854775807L;
  int32_t hexInt32_neg_1 = -1;
  ::ndk::SpAIBinder ibinder;
  std::vector<int32_t> int32_1 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::vector<int64_t> int64_1 = {1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L};
  int32_t hexInt32_pos_1 = 1;
  int32_t hexInt64_pos_1 = 1;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_1;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_2;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_3;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_4;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_5;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_6;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_7;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_8;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_9;
  ::aidl::android::aidl::tests::ConstantExpressionEnum const_exprs_10;
  std::string addString1 = "hello world!";
  std::string addString2 = "The quick brown fox jumps over the lazy dog.";
  int32_t shouldSetBit0AndBit2 = 0;
  std::optional<::aidl::android::aidl::tests::Union> u;
  std::optional<::aidl::android::aidl::tests::Union> shouldBeConstS1;
  ::aidl::android::aidl::tests::IntEnum defaultWithFoo = ::aidl::android::aidl::tests::IntEnum::FOO;

  binder_status_t readFromParcel(const AParcel* parcel);
  binder_status_t writeToParcel(AParcel* parcel) const;

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

  static const ::ndk::parcelable_stability_t _aidl_stability = ::ndk::STABILITY_LOCAL;
  enum : int32_t { BIT0 = 1 };
  enum : int32_t { BIT1 = 2 };
  enum : int32_t { BIT2 = 4 };
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
};
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
