#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include <android/binder_interface_utils.h>
#include <android/binder_parcelable_utils.h>
#include <android/binder_to_string.h>
#include <aidl/android/aidl/tests/FixedSize.h>
#include <aidl/android/aidl/tests/LongEnum.h>
#ifdef BINDER_STABILITY_SUPPORT
#include <android/binder_stability.h>
#endif  // BINDER_STABILITY_SUPPORT

#ifndef __BIONIC__
#define __assert2(a,b,c,d) ((void)0)
#endif

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
class FixedSize {
public:
  typedef std::false_type fixed_size;
  static const char* descriptor;

  class FixedUnion {
  public:
    typedef std::true_type fixed_size;
    static const char* descriptor;

    enum Tag : uint8_t {
      booleanValue = 0,  // boolean booleanValue;
      byteValue,  // byte byteValue;
      charValue,  // char charValue;
      intValue,  // int intValue;
      longValue,  // long longValue;
      floatValue,  // float floatValue;
      doubleValue,  // double doubleValue;
      enumValue,  // android.aidl.tests.LongEnum enumValue;
    };

    template <Tag _Tag>
    using _at = typename std::tuple_element<static_cast<size_t>(_Tag), std::tuple<bool, int8_t, char16_t, int32_t, int64_t, float, double, ::aidl::android::aidl::tests::LongEnum>>::type;
    template <Tag _Tag, typename _Type>
    static FixedUnion make(_Type&& _arg) {
      FixedUnion _inst;
      _inst.set<_Tag>(std::forward<_Type>(_arg));
      return _inst;
    }
    constexpr Tag getTag() const {
      return _tag;
    }
    template <Tag _Tag>
    const _at<_Tag>& get() const {
      if (_Tag != _tag) { __assert2(__FILE__, __LINE__, __PRETTY_FUNCTION__, "bad access: a wrong tag"); }
      return *(_at<_Tag>*)(&_value);
    }
    template <Tag _Tag>
    _at<_Tag>& get() {
      if (_Tag != _tag) { __assert2(__FILE__, __LINE__, __PRETTY_FUNCTION__, "bad access: a wrong tag"); }
      return *(_at<_Tag>*)(&_value);
    }
    template <Tag _Tag, typename _Type>
    void set(_Type&& _arg) {
      _tag = _Tag;
      get<_Tag>() = std::forward<_Type>(_arg);
    }
    binder_status_t readFromParcel(const AParcel* _parcel);
    binder_status_t writeToParcel(AParcel* _parcel) const;

    static int _cmp(const FixedUnion& _lhs, const FixedUnion& _rhs) {
      return _cmp_value(_lhs.getTag(), _rhs.getTag()) || _cmp_value_at<enumValue>(_lhs, _rhs);
    }
    template <Tag _Tag>
    static int _cmp_value_at(const FixedUnion& _lhs, const FixedUnion& _rhs) {
      if constexpr (_Tag == booleanValue) {
        return _cmp_value(_lhs.get<_Tag>(), _rhs.get<_Tag>());
      } else {
        return (_lhs.getTag() == _Tag)
          ? _cmp_value(_lhs.get<_Tag>(), _rhs.get<_Tag>())
          : _cmp_value_at<static_cast<Tag>(static_cast<size_t>(_Tag)-1)>(_lhs, _rhs);
      }
    }
    template <typename _Type>
    static int _cmp_value(const _Type& _lhs, const _Type& _rhs) {
      return (_lhs == _rhs) ? 0 : (_lhs < _rhs) ? -1 : 1;
    }
    inline bool operator!=(const FixedUnion&_rhs) const {
      return _cmp(*this, _rhs) != 0;
    }
    inline bool operator<(const FixedUnion&_rhs) const {
      return _cmp(*this, _rhs) < 0;
    }
    inline bool operator<=(const FixedUnion&_rhs) const {
      return _cmp(*this, _rhs) <= 0;
    }
    inline bool operator==(const FixedUnion&_rhs) const {
      return _cmp(*this, _rhs) == 0;
    }
    inline bool operator>(const FixedUnion&_rhs) const {
      return _cmp(*this, _rhs) > 0;
    }
    inline bool operator>=(const FixedUnion&_rhs) const {
      return _cmp(*this, _rhs) >= 0;
    }
    static const ::ndk::parcelable_stability_t _aidl_stability = ::ndk::STABILITY_LOCAL;
    inline std::string toString() const {
      std::ostringstream os;
      os << "FixedUnion{";
      switch (getTag()) {
      case booleanValue: os << "booleanValue: " << ::android::internal::ToString(get<booleanValue>()); break;
      case byteValue: os << "byteValue: " << ::android::internal::ToString(get<byteValue>()); break;
      case charValue: os << "charValue: " << ::android::internal::ToString(get<charValue>()); break;
      case intValue: os << "intValue: " << ::android::internal::ToString(get<intValue>()); break;
      case longValue: os << "longValue: " << ::android::internal::ToString(get<longValue>()); break;
      case floatValue: os << "floatValue: " << ::android::internal::ToString(get<floatValue>()); break;
      case doubleValue: os << "doubleValue: " << ::android::internal::ToString(get<doubleValue>()); break;
      case enumValue: os << "enumValue: " << ::android::internal::ToString(get<enumValue>()); break;
      }
      os << "}";
      return os.str();
    }
  private:
    Tag _tag __attribute__((aligned (1))) = booleanValue;
    union _value_t {
      _value_t() {}
      ~_value_t() {}
      bool booleanValue __attribute__((aligned (1))) = bool(false);
      int8_t byteValue __attribute__((aligned (1)));
      char16_t charValue __attribute__((aligned (2)));
      int32_t intValue __attribute__((aligned (4)));
      int64_t longValue __attribute__((aligned (8)));
      float floatValue __attribute__((aligned (4)));
      double doubleValue __attribute__((aligned (8)));
      ::aidl::android::aidl::tests::LongEnum enumValue __attribute__((aligned (8)));
    } _value;
  };
  class FixedParcelable {
  public:
    typedef std::true_type fixed_size;
    static const char* descriptor;

    bool booleanValue __attribute__((aligned (1))) = false;
    int8_t byteValue __attribute__((aligned (1))) = 0;
    char16_t charValue __attribute__((aligned (2))) = '\0';
    int32_t intValue __attribute__((aligned (4))) = 0;
    int64_t longValue __attribute__((aligned (8))) = 0L;
    float floatValue __attribute__((aligned (4))) = 0.000000f;
    double doubleValue __attribute__((aligned (8))) = 0.000000;
    ::aidl::android::aidl::tests::LongEnum enumValue __attribute__((aligned (8))) = ::aidl::android::aidl::tests::LongEnum::FOO;
    ::aidl::android::aidl::tests::FixedSize::FixedUnion parcelableValue;

    binder_status_t readFromParcel(const AParcel* parcel);
    binder_status_t writeToParcel(AParcel* parcel) const;

    inline bool operator!=(const FixedParcelable& rhs) const {
      return std::tie(booleanValue, byteValue, charValue, intValue, longValue, floatValue, doubleValue, enumValue, parcelableValue) != std::tie(rhs.booleanValue, rhs.byteValue, rhs.charValue, rhs.intValue, rhs.longValue, rhs.floatValue, rhs.doubleValue, rhs.enumValue, rhs.parcelableValue);
    }
    inline bool operator<(const FixedParcelable& rhs) const {
      return std::tie(booleanValue, byteValue, charValue, intValue, longValue, floatValue, doubleValue, enumValue, parcelableValue) < std::tie(rhs.booleanValue, rhs.byteValue, rhs.charValue, rhs.intValue, rhs.longValue, rhs.floatValue, rhs.doubleValue, rhs.enumValue, rhs.parcelableValue);
    }
    inline bool operator<=(const FixedParcelable& rhs) const {
      return std::tie(booleanValue, byteValue, charValue, intValue, longValue, floatValue, doubleValue, enumValue, parcelableValue) <= std::tie(rhs.booleanValue, rhs.byteValue, rhs.charValue, rhs.intValue, rhs.longValue, rhs.floatValue, rhs.doubleValue, rhs.enumValue, rhs.parcelableValue);
    }
    inline bool operator==(const FixedParcelable& rhs) const {
      return std::tie(booleanValue, byteValue, charValue, intValue, longValue, floatValue, doubleValue, enumValue, parcelableValue) == std::tie(rhs.booleanValue, rhs.byteValue, rhs.charValue, rhs.intValue, rhs.longValue, rhs.floatValue, rhs.doubleValue, rhs.enumValue, rhs.parcelableValue);
    }
    inline bool operator>(const FixedParcelable& rhs) const {
      return std::tie(booleanValue, byteValue, charValue, intValue, longValue, floatValue, doubleValue, enumValue, parcelableValue) > std::tie(rhs.booleanValue, rhs.byteValue, rhs.charValue, rhs.intValue, rhs.longValue, rhs.floatValue, rhs.doubleValue, rhs.enumValue, rhs.parcelableValue);
    }
    inline bool operator>=(const FixedParcelable& rhs) const {
      return std::tie(booleanValue, byteValue, charValue, intValue, longValue, floatValue, doubleValue, enumValue, parcelableValue) >= std::tie(rhs.booleanValue, rhs.byteValue, rhs.charValue, rhs.intValue, rhs.longValue, rhs.floatValue, rhs.doubleValue, rhs.enumValue, rhs.parcelableValue);
    }

    static const ::ndk::parcelable_stability_t _aidl_stability = ::ndk::STABILITY_LOCAL;
    inline std::string toString() const {
      std::ostringstream os;
      os << "FixedParcelable{";
      os << "booleanValue: " << ::android::internal::ToString(booleanValue);
      os << ", byteValue: " << ::android::internal::ToString(byteValue);
      os << ", charValue: " << ::android::internal::ToString(charValue);
      os << ", intValue: " << ::android::internal::ToString(intValue);
      os << ", longValue: " << ::android::internal::ToString(longValue);
      os << ", floatValue: " << ::android::internal::ToString(floatValue);
      os << ", doubleValue: " << ::android::internal::ToString(doubleValue);
      os << ", enumValue: " << ::android::internal::ToString(enumValue);
      os << ", parcelableValue: " << ::android::internal::ToString(parcelableValue);
      os << "}";
      return os.str();
    }
  };

  binder_status_t readFromParcel(const AParcel* parcel);
  binder_status_t writeToParcel(AParcel* parcel) const;

  inline bool operator!=(const FixedSize&) const {
    return std::tie() != std::tie();
  }
  inline bool operator<(const FixedSize&) const {
    return std::tie() < std::tie();
  }
  inline bool operator<=(const FixedSize&) const {
    return std::tie() <= std::tie();
  }
  inline bool operator==(const FixedSize&) const {
    return std::tie() == std::tie();
  }
  inline bool operator>(const FixedSize&) const {
    return std::tie() > std::tie();
  }
  inline bool operator>=(const FixedSize&) const {
    return std::tie() >= std::tie();
  }

  static const ::ndk::parcelable_stability_t _aidl_stability = ::ndk::STABILITY_LOCAL;
  inline std::string toString() const {
    std::ostringstream os;
    os << "FixedSize{";
    os << "}";
    return os.str();
  }
};
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
