#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinder;
use binder::declare_binder_interface;
declare_binder_interface! {
  IOldName["android.aidl.tests.IOldName"] {
    native: BnOldName(on_transact),
    proxy: BpOldName {
    },
  }
}
pub trait IOldName: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.IOldName" }
  fn RealName(&self) -> binder::public_api::Result<String> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getDefaultImpl() -> IOldNameDefault where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: IOldNameDefault) -> IOldNameDefault where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub mod transactions {
  #[allow(unused_imports)] use binder::IBinder;
  pub const RealName: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 0;
}
pub type IOldNameDefault = Option<std::sync::Arc<dyn IOldName + Sync>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<IOldNameDefault> = std::sync::Mutex::new(None);
}
pub(crate) mod mangled { pub use super::IOldName as _7_android_4_aidl_5_tests_8_IOldName; }
impl IOldName for BpOldName {
  fn RealName(&self) -> binder::public_api::Result<String> {
    let _aidl_reply = self.binder.transact(transactions::RealName, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IOldName>::getDefaultImpl() {
        return _aidl_default_impl.RealName();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
}
impl IOldName for binder::Binder<BnOldName> {
  fn RealName(&self) -> binder::public_api::Result<String> { self.0.RealName() }
}
fn on_transact(_aidl_service: &dyn IOldName, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::Parcel, _aidl_reply: &mut binder::parcel::Parcel) -> binder::Result<()> {
  match _aidl_code {
    transactions::RealName => {
      let _aidl_return = _aidl_service.RealName();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    _ => Err(binder::StatusCode::UNKNOWN_TRANSACTION)
  }
}
