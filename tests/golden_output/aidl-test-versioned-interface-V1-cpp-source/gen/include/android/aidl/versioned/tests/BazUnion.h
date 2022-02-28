#pragma once

#include <android/binder_to_string.h>
#include <binder/Parcel.h>
#include <binder/Status.h>
#include <cassert>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <utils/String16.h>
#include <variant>

#ifndef __BIONIC__
#define __assert2(a,b,c,d) ((void)0)
#endif

namespace android {
namespace aidl {
namespace versioned {
namespace tests {
class BazUnion : public ::android::Parcelable {
public:
  enum Tag : int32_t {
    intNum = 0,  // int intNum;
  };

  template<typename _Tp>
  static constexpr bool _not_self = !std::is_same_v<std::remove_cv_t<std::remove_reference_t<_Tp>>, BazUnion>;

  BazUnion() : _value(std::in_place_index<intNum>, int32_t(0)) { }

  template <typename _Tp, typename = std::enable_if_t<_not_self<_Tp>>>
  // NOLINTNEXTLINE(google-explicit-constructor)
  constexpr BazUnion(_Tp&& _arg)
      : _value(std::forward<_Tp>(_arg)) {}

  template <size_t _Np, typename... _Tp>
  constexpr explicit BazUnion(std::in_place_index_t<_Np>, _Tp&&... _args)
      : _value(std::in_place_index<_Np>, std::forward<_Tp>(_args)...) {}

  template <Tag _tag, typename... _Tp>
  static BazUnion make(_Tp&&... _args) {
    return BazUnion(std::in_place_index<_tag>, std::forward<_Tp>(_args)...);
  }

  template <Tag _tag, typename _Tp, typename... _Up>
  static BazUnion make(std::initializer_list<_Tp> _il, _Up&&... _args) {
    return BazUnion(std::in_place_index<_tag>, std::move(_il), std::forward<_Up>(_args)...);
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

  inline bool operator!=(const BazUnion& rhs) const {
    return _value != rhs._value;
  }
  inline bool operator<(const BazUnion& rhs) const {
    return _value < rhs._value;
  }
  inline bool operator<=(const BazUnion& rhs) const {
    return _value <= rhs._value;
  }
  inline bool operator==(const BazUnion& rhs) const {
    return _value == rhs._value;
  }
  inline bool operator>(const BazUnion& rhs) const {
    return _value > rhs._value;
  }
  inline bool operator>=(const BazUnion& rhs) const {
    return _value >= rhs._value;
  }

  ::android::status_t readFromParcel(const ::android::Parcel* _aidl_parcel) final;
  ::android::status_t writeToParcel(::android::Parcel* _aidl_parcel) const final;
  static const ::android::String16& getParcelableDescriptor() {
    static const ::android::StaticString16 DESCIPTOR (u"android.aidl.versioned.tests.BazUnion");
    return DESCIPTOR;
  }
  inline std::string toString() const {
    std::ostringstream os;
    os << "BazUnion{";
    switch (getTag()) {
    case intNum: os << "intNum: " << ::android::internal::ToString(get<intNum>()); break;
    }
    os << "}";
    return os.str();
  }
private:
  std::variant<int32_t> _value;
};  // class BazUnion
}  // namespace tests
}  // namespace versioned
}  // namespace aidl
}  // namespace android
