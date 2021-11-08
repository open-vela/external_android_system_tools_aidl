#include <android/binder_parcel_utils.h>
#include <aidl/android/aidl/tests/permission/BpProtectedInterface.h>
#include <aidl/android/aidl/tests/permission/BnProtectedInterface.h>
#include <aidl/android/aidl/tests/permission/IProtectedInterface.h>

namespace aidl {
namespace android {
namespace aidl {
namespace tests {
namespace permission {
static binder_status_t _aidl_android_aidl_tests_permission_IProtectedInterface_onTransact(AIBinder* _aidl_binder, transaction_code_t _aidl_code, const AParcel* _aidl_in, AParcel* _aidl_out) {
  (void)_aidl_in;
  (void)_aidl_out;
  binder_status_t _aidl_ret_status = STATUS_UNKNOWN_TRANSACTION;
  std::shared_ptr<BnProtectedInterface> _aidl_impl = std::static_pointer_cast<BnProtectedInterface>(::ndk::ICInterface::asInterface(_aidl_binder));
  switch (_aidl_code) {
    case (FIRST_CALL_TRANSACTION + 0 /*Method1*/): {

      ::ndk::ScopedAStatus _aidl_status = _aidl_impl->Method1();
      _aidl_ret_status = AParcel_writeStatusHeader(_aidl_out, _aidl_status.get());
      if (_aidl_ret_status != STATUS_OK) break;

      if (!AStatus_isOk(_aidl_status.get())) break;

      break;
    }
    case (FIRST_CALL_TRANSACTION + 1 /*Method2*/): {

      ::ndk::ScopedAStatus _aidl_status = _aidl_impl->Method2();
      _aidl_ret_status = AParcel_writeStatusHeader(_aidl_out, _aidl_status.get());
      if (_aidl_ret_status != STATUS_OK) break;

      if (!AStatus_isOk(_aidl_status.get())) break;

      break;
    }
  }
  return _aidl_ret_status;
}

static AIBinder_Class* _g_aidl_android_aidl_tests_permission_IProtectedInterface_clazz = ::ndk::ICInterface::defineClass(IProtectedInterface::descriptor, _aidl_android_aidl_tests_permission_IProtectedInterface_onTransact);

BpProtectedInterface::BpProtectedInterface(const ::ndk::SpAIBinder& binder) : BpCInterface(binder) {}
BpProtectedInterface::~BpProtectedInterface() {}

::ndk::ScopedAStatus BpProtectedInterface::Method1() {
  binder_status_t _aidl_ret_status = STATUS_OK;
  ::ndk::ScopedAStatus _aidl_status;
  ::ndk::ScopedAParcel _aidl_in;
  ::ndk::ScopedAParcel _aidl_out;

  _aidl_ret_status = AIBinder_prepareTransaction(asBinder().get(), _aidl_in.getR());
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  _aidl_ret_status = AIBinder_transact(
    asBinder().get(),
    (FIRST_CALL_TRANSACTION + 0 /*Method1*/),
    _aidl_in.getR(),
    _aidl_out.getR(),
    0
    #ifdef BINDER_STABILITY_SUPPORT
    | FLAG_PRIVATE_LOCAL
    #endif  // BINDER_STABILITY_SUPPORT
    );
  if (_aidl_ret_status == STATUS_UNKNOWN_TRANSACTION && IProtectedInterface::getDefaultImpl()) {
    _aidl_status = IProtectedInterface::getDefaultImpl()->Method1();
    goto _aidl_status_return;
  }
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  _aidl_ret_status = AParcel_readStatusHeader(_aidl_out.get(), _aidl_status.getR());
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  if (!AStatus_isOk(_aidl_status.get())) goto _aidl_status_return;
  _aidl_error:
  _aidl_status.set(AStatus_fromStatus(_aidl_ret_status));
  _aidl_status_return:
  return _aidl_status;
}
::ndk::ScopedAStatus BpProtectedInterface::Method2() {
  binder_status_t _aidl_ret_status = STATUS_OK;
  ::ndk::ScopedAStatus _aidl_status;
  ::ndk::ScopedAParcel _aidl_in;
  ::ndk::ScopedAParcel _aidl_out;

  _aidl_ret_status = AIBinder_prepareTransaction(asBinder().get(), _aidl_in.getR());
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  _aidl_ret_status = AIBinder_transact(
    asBinder().get(),
    (FIRST_CALL_TRANSACTION + 1 /*Method2*/),
    _aidl_in.getR(),
    _aidl_out.getR(),
    0
    #ifdef BINDER_STABILITY_SUPPORT
    | FLAG_PRIVATE_LOCAL
    #endif  // BINDER_STABILITY_SUPPORT
    );
  if (_aidl_ret_status == STATUS_UNKNOWN_TRANSACTION && IProtectedInterface::getDefaultImpl()) {
    _aidl_status = IProtectedInterface::getDefaultImpl()->Method2();
    goto _aidl_status_return;
  }
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  _aidl_ret_status = AParcel_readStatusHeader(_aidl_out.get(), _aidl_status.getR());
  if (_aidl_ret_status != STATUS_OK) goto _aidl_error;

  if (!AStatus_isOk(_aidl_status.get())) goto _aidl_status_return;
  _aidl_error:
  _aidl_status.set(AStatus_fromStatus(_aidl_ret_status));
  _aidl_status_return:
  return _aidl_status;
}
// Source for BnProtectedInterface
BnProtectedInterface::BnProtectedInterface() {}
BnProtectedInterface::~BnProtectedInterface() {}
::ndk::SpAIBinder BnProtectedInterface::createBinder() {
  AIBinder* binder = AIBinder_new(_g_aidl_android_aidl_tests_permission_IProtectedInterface_clazz, static_cast<void*>(this));
  #ifdef BINDER_STABILITY_SUPPORT
  AIBinder_markCompilationUnitStability(binder);
  #endif  // BINDER_STABILITY_SUPPORT
  return ::ndk::SpAIBinder(binder);
}
// Source for IProtectedInterface
const char* IProtectedInterface::descriptor = "android.aidl.tests.permission.IProtectedInterface";
IProtectedInterface::IProtectedInterface() {}
IProtectedInterface::~IProtectedInterface() {}


std::shared_ptr<IProtectedInterface> IProtectedInterface::fromBinder(const ::ndk::SpAIBinder& binder) {
  if (!AIBinder_associateClass(binder.get(), _g_aidl_android_aidl_tests_permission_IProtectedInterface_clazz)) { return nullptr; }
  std::shared_ptr<::ndk::ICInterface> interface = ::ndk::ICInterface::asInterface(binder.get());
  if (interface) {
    return std::static_pointer_cast<IProtectedInterface>(interface);
  }
  return ::ndk::SharedRefBase::make<BpProtectedInterface>(binder);
}

binder_status_t IProtectedInterface::writeToParcel(AParcel* parcel, const std::shared_ptr<IProtectedInterface>& instance) {
  return AParcel_writeStrongBinder(parcel, instance ? instance->asBinder().get() : nullptr);
}
binder_status_t IProtectedInterface::readFromParcel(const AParcel* parcel, std::shared_ptr<IProtectedInterface>* instance) {
  ::ndk::SpAIBinder binder;
  binder_status_t status = AParcel_readStrongBinder(parcel, binder.getR());
  if (status != STATUS_OK) return status;
  *instance = IProtectedInterface::fromBinder(binder);
  return STATUS_OK;
}
bool IProtectedInterface::setDefaultImpl(const std::shared_ptr<IProtectedInterface>& impl) {
  // Only one user of this interface can use this function
  // at a time. This is a heuristic to detect if two different
  // users in the same process use this function.
  assert(!IProtectedInterface::default_impl);
  if (impl) {
    IProtectedInterface::default_impl = impl;
    return true;
  }
  return false;
}
const std::shared_ptr<IProtectedInterface>& IProtectedInterface::getDefaultImpl() {
  return IProtectedInterface::default_impl;
}
std::shared_ptr<IProtectedInterface> IProtectedInterface::default_impl = nullptr;
::ndk::ScopedAStatus IProtectedInterfaceDefault::Method1() {
  ::ndk::ScopedAStatus _aidl_status;
  _aidl_status.set(AStatus_fromStatus(STATUS_UNKNOWN_TRANSACTION));
  return _aidl_status;
}
::ndk::ScopedAStatus IProtectedInterfaceDefault::Method2() {
  ::ndk::ScopedAStatus _aidl_status;
  _aidl_status.set(AStatus_fromStatus(STATUS_UNKNOWN_TRANSACTION));
  return _aidl_status;
}
::ndk::SpAIBinder IProtectedInterfaceDefault::asBinder() {
  return ::ndk::SpAIBinder();
}
bool IProtectedInterfaceDefault::isRemote() {
  return false;
}
}  // namespace permission
}  // namespace tests
}  // namespace aidl
}  // namespace android
}  // namespace aidl
