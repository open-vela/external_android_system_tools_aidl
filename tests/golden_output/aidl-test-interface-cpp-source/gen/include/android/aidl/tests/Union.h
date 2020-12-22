#pragma once

#include <android/aidl/tests/ByteEnum.h>
#include <binder/IBinder.h>
#include <binder/Parcel.h>
#include <binder/Status.h>
#include <cassert>
#include <codecvt>
#include <cstdint>
#include <locale>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <utils/String16.h>
#include <variant>
#include <vector>

#ifndef __BIONIC__
#define __assert2(a,b,c,d) ((void)0)
#endif

namespace android {

namespace aidl {

namespace tests {

class Union : public ::android::Parcelable {
public:
  inline bool operator!=(const Union& rhs) const {
    return _value != rhs._value;
  }
  inline bool operator<(const Union& rhs) const {
    return _value < rhs._value;
  }
  inline bool operator<=(const Union& rhs) const {
    return _value <= rhs._value;
  }
  inline bool operator==(const Union& rhs) const {
    return _value == rhs._value;
  }
  inline bool operator>(const Union& rhs) const {
    return _value > rhs._value;
  }
  inline bool operator>=(const Union& rhs) const {
    return _value >= rhs._value;
  }

  enum Tag : int32_t {
    ns = 0,  // int[] ns;
    n,  // int n;
    m,  // int m;
    s,  // String s;
    ibinder,  // IBinder ibinder;
    ss,  // List<String> ss;
    be,  // android.aidl.tests.ByteEnum be;
  };

  template<typename _Tp>
  static constexpr bool _not_self = !std::is_same_v<std::remove_cv_t<std::remove_reference_t<_Tp>>, Union>;

  Union() : _value(std::in_place_index<ns>, ::std::vector<int32_t>()) { }
  Union(const Union&) = default;
  Union(Union&&) = default;
  Union& operator=(const Union&) = default;
  Union& operator=(Union&&) = default;

  template <typename _Tp, typename = std::enable_if_t<_not_self<_Tp>>>
  // NOLINTNEXTLINE(google-explicit-constructor)
  constexpr Union(_Tp&& _arg)
      : _value(std::forward<_Tp>(_arg)) {}

  template <typename... _Tp>
  constexpr explicit Union(_Tp&&... _args)
      : _value(std::forward<_Tp>(_args)...) {}

  template <Tag _tag, typename... _Tp>
  static Union make(_Tp&&... _args) {
    return Union(std::in_place_index<_tag>, std::forward<_Tp>(_args)...);
  }

  template <Tag _tag, typename _Tp, typename... _Up>
  static Union make(std::initializer_list<_Tp> _il, _Up&&... _args) {
    return Union(std::in_place_index<_tag>, std::move(_il), std::forward<_Up>(_args)...);
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

  static const ::std::string& S1();
  ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) override final;
  ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const override final;
  static const std::string& getParcelableDescriptor() {
    static const std::string DESCIPTOR = "android.aidl.tests.Union";
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
    os << "Union{";
    switch (getTag()) {
    case ns: os << "ns: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: get<ns>()) { (void)v; if (first) first = false; else o << ", "; o << std::to_string(v); }; o << "]"; return o.str(); }(); break;
    case n: os << "n: " << std::to_string(get<n>()); break;
    case m: os << "m: " << std::to_string(get<m>()); break;
    case s: os << "s: " << (std::ostringstream() << get<s>()).str(); break;
    case ibinder: os << "ibinder: " << ""; break;
    case ss: os << "ss: " << [&](){ std::ostringstream o; o << "["; bool first = true; for (const auto& v: get<ss>()) { (void)v; if (first) first = false; else o << ", "; o << (std::ostringstream() << v).str(); }; o << "]"; return o.str(); }(); break;
    case be: os << "be: " << android::aidl::tests::toString(get<be>()); break;
    }
    os << "}";
    return os.str();
  }
private:
  std::variant<::std::vector<int32_t>, int32_t, int32_t, ::std::string, ::android::sp<::android::IBinder>, ::std::vector<::std::string>, ::android::aidl::tests::ByteEnum> _value;
};  // class Union

}  // namespace tests

}  // namespace aidl

}  // namespace android
