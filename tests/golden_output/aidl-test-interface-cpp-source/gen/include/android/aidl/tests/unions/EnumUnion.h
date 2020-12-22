#pragma once

#include <android/aidl/tests/IntEnum.h>
#include <android/aidl/tests/LongEnum.h>
#include <binder/Parcel.h>
#include <binder/Status.h>
#include <cassert>
#include <codecvt>
#include <locale>
#include <sstream>
#include <type_traits>
#include <utility>
#include <variant>

#ifndef __BIONIC__
#define __assert2(a,b,c,d) ((void)0)
#endif

namespace android {

namespace aidl {

namespace tests {

namespace unions {

class EnumUnion : public ::android::Parcelable {
public:
  inline bool operator!=(const EnumUnion& rhs) const {
    return _value != rhs._value;
  }
  inline bool operator<(const EnumUnion& rhs) const {
    return _value < rhs._value;
  }
  inline bool operator<=(const EnumUnion& rhs) const {
    return _value <= rhs._value;
  }
  inline bool operator==(const EnumUnion& rhs) const {
    return _value == rhs._value;
  }
  inline bool operator>(const EnumUnion& rhs) const {
    return _value > rhs._value;
  }
  inline bool operator>=(const EnumUnion& rhs) const {
    return _value >= rhs._value;
  }

  enum Tag : int32_t {
    intEnum = 0,  // android.aidl.tests.IntEnum intEnum;
    longEnum,  // android.aidl.tests.LongEnum longEnum;
  };

  template<typename _Tp>
  static constexpr bool _not_self = !std::is_same_v<std::remove_cv_t<std::remove_reference_t<_Tp>>, EnumUnion>;

  EnumUnion() : _value(std::in_place_index<intEnum>, ::android::aidl::tests::IntEnum(::android::aidl::tests::IntEnum::FOO)) { }
  EnumUnion(const EnumUnion&) = default;
  EnumUnion(EnumUnion&&) = default;
  EnumUnion& operator=(const EnumUnion&) = default;
  EnumUnion& operator=(EnumUnion&&) = default;

  template <typename _Tp, typename = std::enable_if_t<_not_self<_Tp>>>
  // NOLINTNEXTLINE(google-explicit-constructor)
  constexpr EnumUnion(_Tp&& _arg)
      : _value(std::forward<_Tp>(_arg)) {}

  template <typename... _Tp>
  constexpr explicit EnumUnion(_Tp&&... _args)
      : _value(std::forward<_Tp>(_args)...) {}

  template <Tag _tag, typename... _Tp>
  static EnumUnion make(_Tp&&... _args) {
    return EnumUnion(std::in_place_index<_tag>, std::forward<_Tp>(_args)...);
  }

  template <Tag _tag, typename _Tp, typename... _Up>
  static EnumUnion make(std::initializer_list<_Tp> _il, _Up&&... _args) {
    return EnumUnion(std::in_place_index<_tag>, std::move(_il), std::forward<_Up>(_args)...);
  }

  Tag getTag() const {
    return static_cast<Tag>(_value.index());
  }

  template <Tag _tag>
  const auto& get() const {
    if (getTag() != _tag) { __assert2(__FILE__, __LINE__, __PRETTY_FUNCTION__, "bad access: a wrong tag"); }
    return std::get<_tag>(_value);
  }

  template <Tag _tag>
  auto& get() {
    if (getTag() != _tag) { __assert2(__FILE__, __LINE__, __PRETTY_FUNCTION__, "bad access: a wrong tag"); }
    return std::get<_tag>(_value);
  }

  template <Tag _tag, typename... _Tp>
  void set(_Tp&&... _args) {
    _value.emplace<_tag>(std::forward<_Tp>(_args)...);
  }

  ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) override final;
  ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const override final;
  static const std::string& getParcelableDescriptor() {
    static const std::string DESCIPTOR = "android.aidl.tests.unions.EnumUnion";
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
    os << "EnumUnion{";
    switch (getTag()) {
    case intEnum: os << "intEnum: " << android::aidl::tests::toString(get<intEnum>()); break;
    case longEnum: os << "longEnum: " << android::aidl::tests::toString(get<longEnum>()); break;
    }
    os << "}";
    return os.str();
  }
private:
  std::variant<::android::aidl::tests::IntEnum, ::android::aidl::tests::LongEnum> _value;
};  // class EnumUnion

}  // namespace unions

}  // namespace tests

}  // namespace aidl

}  // namespace android
