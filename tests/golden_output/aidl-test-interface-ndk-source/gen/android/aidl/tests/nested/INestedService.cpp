#include <android/binder_parcel_utils.h>
#include <aidl/android/aidl/tests/nested/BpNestedService.h>
#include <aidl/android/aidl/tests/nested/BnNestedService.h>
#include <aidl/android/aidl/tests/nested/INestedService.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
namespace nested {
static binder_status_t _aidl_onTransact(AIBinder* _aidl_binder, transaction_code_t _aidl_code, const AParcel* _aidl_in, AParcel* _aidl_out) {
  (void)_aidl_in;
  (void)_aidl_out;
  binder_status_t _aidl_ret_status = STATUS_UNKNOWN_TRANSACTION;
  std::shared_ptr<BnNestedService> _aidl_impl = std::static_pointer_cast<BnNestedService>(::ndk::ICInterface::asInterface(_aidl_binder));
  switch (_aidl_code) {
    case (FIRST_CALL_TRANSACTION + 0 /*flipStatus*/): {
      ::aidl::android::aidl::tests::nested::ParcelableWithNested in_p;
      ::aidl::android::aidl::tests::nested::INestedService::Result _aidl_return;

      _aidl_ret_status = ::ndk::AParcel_readParcelable(_aidl_in, &in_p);
      if (_aidl_ret_status != STATUS_OK) break;

      ::ndk::ScopedAStatus _aidl_status = _aidl_impl->flipStatus(in_p, &_aidl_return);
      _aidl_ret_status = AParcel_writeStatusHeader(_aidl_out, _aidl_status.get());
      if (_aidl_ret_status != STATUS_OK) break;

      if (!AStatus_isOk(_aidl_status.get())) break;

      _aidl_ret_status = ::ndk::AParcel_writeParcelable(_aidl_out, _aidl_return);
      if (_aidl_ret_status != STATUS_OK) break;

      break;
    }
  }
  return _aidl_ret_status;
}

static AIBinder_Class* _g_aidl_clazz = ::ndk::ICInterface::defineClass(INestedService::descriptor, _aidl_onTransact);

BpNestedService::BpNestedService(const ::ndk::SpAIBinder& binder) : BpCInterface(binder) {}
BpNestedService::~BpNestedService() {}

::ndk::ScopedAStatus BpNestedService::flipStatus(const ::aidl::android::aidl::tests::nested::ParcelableWithNested& in_p, ::aidl::android::aidl::tests::nested::INestedService::Result* _aidl_return) {
  binder_status_t _aidl_ret_status = STATUS_OK;
  ::ndk::ScopedAStatus _aidl_status;
  ::ndk::ScopedAParcel _aidl_in;
  ::ndk::ScopedAParcel _aidl_out;

  _aidl_ret_status = AIBinder_prepareTransaction(asBinder().get(), _aidl_in.getR());
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  _aidl_ret_status = ::ndk::AParcel_writeParcelable(_aidl_in.get(), in_p);
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  _aidl_ret_status = AIBinder_transact(
    asBinder().get(),
    (FIRST_CALL_TRANSACTION + 0 /*flipStatus*/),
    _aidl_in.getR(),
    _aidl_out.getR(),
    0
    #ifdef BINDER_STABILITY_SUPPORT
    | FLAG_PRIVATE_LOCAL
    #endif  // BINDER_STABILITY_SUPPORT
    );
  if (_aidl_ret_status == STATUS_UNKNOWN_TRANSACTION && INestedService::getDefaultImpl()) {
    _aidl_status = INestedService::getDefaultImpl()->flipStatus(in_p, _aidl_return);
    goto _aidl_status_return;
  }
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  _aidl_ret_status = AParcel_readStatusHeader(_aidl_out.get(), _aidl_status.getR());
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  if (!AStatus_isOk(_aidl_status.get())) goto _aidl_status_return;
  _aidl_ret_status = ::ndk::AParcel_readParcelable(_aidl_out.get(), _aidl_return);
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  _aidl_error:
  _aidl_status.set(AStatus_fromStatus(_aidl_ret_status));
  _aidl_status_return:
  return _aidl_status;
}
// Source for BnNestedService
BnNestedService::BnNestedService() {}
BnNestedService::~BnNestedService() {}
::ndk::SpAIBinder BnNestedService::createBinder() {
  AIBinder* binder = AIBinder_new(_g_aidl_clazz, static_cast<void*>(this));
  #ifdef BINDER_STABILITY_SUPPORT
  AIBinder_markCompilationUnitStability(binder);
  #endif  // BINDER_STABILITY_SUPPORT
  return ::ndk::SpAIBinder(binder);
}
// Source for INestedService
const char* INestedService::descriptor = "android.aidl.tests.nested.INestedService";
INestedService::INestedService() {}
INestedService::~INestedService() {}


std::shared_ptr<INestedService> INestedService::fromBinder(const ::ndk::SpAIBinder& binder) {
  if (!AIBinder_associateClass(binder.get(), _g_aidl_clazz)) { return nullptr; }
  std::shared_ptr<::ndk::ICInterface> interface = ::ndk::ICInterface::asInterface(binder.get());
  if (interface) {
    return std::static_pointer_cast<INestedService>(interface);
  }
  return ::ndk::SharedRefBase::make<BpNestedService>(binder);
}

binder_status_t INestedService::writeToParcel(AParcel* parcel, const std::shared_ptr<INestedService>& instance) {
  return AParcel_writeStrongBinder(parcel, instance ? instance->asBinder().get() : nullptr);
}
binder_status_t INestedService::readFromParcel(const AParcel* parcel, std::shared_ptr<INestedService>* instance) {
  ::ndk::SpAIBinder binder;
  binder_status_t status = AParcel_readStrongBinder(parcel, binder.getR());
  if (status != STATUS_OK) return status;
  *instance = INestedService::fromBinder(binder);
  return STATUS_OK;
}
bool INestedService::setDefaultImpl(const std::shared_ptr<INestedService>& impl) {
  // Only one user of this interface can use this function
  // at a time. This is a heuristic to detect if two different
  // users in the same process use this function.
  assert(!INestedService::default_impl);
  if (impl) {
    INestedService::default_impl = impl;
    return true;
  }
  return false;
}
const std::shared_ptr<INestedService>& INestedService::getDefaultImpl() {
  return INestedService::default_impl;
}
std::shared_ptr<INestedService> INestedService::default_impl = nullptr;
::ndk::ScopedAStatus INestedServiceDefault::flipStatus(const ::aidl::android::aidl::tests::nested::ParcelableWithNested& /*in_p*/, ::aidl::android::aidl::tests::nested::INestedService::Result* /*_aidl_return*/) {
  ::ndk::ScopedAStatus _aidl_status;
  _aidl_status.set(AStatus_fromStatus(STATUS_UNKNOWN_TRANSACTION));
  return _aidl_status;
}
::ndk::SpAIBinder INestedServiceDefault::asBinder() {
  return ::ndk::SpAIBinder();
}
bool INestedServiceDefault::isRemote() {
  return false;
}
}  // namespace nested
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
#include "aidl/android/aidl/tests/nested/INestedService.h"

#include <android/binder_parcel_utils.h>
#include <aidl/android/aidl/tests/nested/BpNestedService.h>
#include <aidl/android/aidl/tests/nested/BnNestedService.h>
#include <aidl/android/aidl/tests/nested/INestedService.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
namespace nested {
const char* INestedService::Result::descriptor = "android.aidl.tests.nested.INestedService.Result";

binder_status_t INestedService::Result::readFromParcel(const AParcel* parcel) {
  int32_t _aidl_parcelable_size;
  int32_t _aidl_start_pos = AParcel_getDataPosition(parcel);
  binder_status_t _aidl_ret_status = AParcel_readInt32(parcel, &_aidl_parcelable_size);
  if (_aidl_start_pos > INT32_MAX - _aidl_parcelable_size) return STATUS_BAD_VALUE;
  if (_aidl_parcelable_size < 0) return STATUS_BAD_VALUE;
  if (_aidl_ret_status != STATUS_OK) return _aidl_ret_status;

  if (AParcel_getDataPosition(parcel) - _aidl_start_pos >= _aidl_parcelable_size) {
    AParcel_setDataPosition(parcel, _aidl_start_pos + _aidl_parcelable_size);
    return _aidl_ret_status;
  }
  _aidl_ret_status = AParcel_readByte(parcel, reinterpret_cast<int8_t*>(&status));
  if (_aidl_ret_status != STATUS_OK) return _aidl_ret_status;

  AParcel_setDataPosition(parcel, _aidl_start_pos + _aidl_parcelable_size);
  return _aidl_ret_status;
}
binder_status_t INestedService::Result::writeToParcel(AParcel* parcel) const {
  binder_status_t _aidl_ret_status;
  size_t _aidl_start_pos = AParcel_getDataPosition(parcel);
  _aidl_ret_status = AParcel_writeInt32(parcel, 0);
  if (_aidl_ret_status != STATUS_OK) return _aidl_ret_status;

  _aidl_ret_status = AParcel_writeByte(parcel, static_cast<int8_t>(status));
  if (_aidl_ret_status != STATUS_OK) return _aidl_ret_status;

  size_t _aidl_end_pos = AParcel_getDataPosition(parcel);
  AParcel_setDataPosition(parcel, _aidl_start_pos);
  AParcel_writeInt32(parcel, _aidl_end_pos - _aidl_start_pos);
  AParcel_setDataPosition(parcel, _aidl_end_pos);
  return _aidl_ret_status;
}

}  // namespace nested
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
