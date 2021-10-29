#![forbid(unsafe_code)]
#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinderInternal;
use binder::declare_binder_interface;
declare_binder_interface! {
  IProtectedInterface["android.aidl.tests.permission.IProtectedInterface"] {
    native: BnProtectedInterface(on_transact),
    proxy: BpProtectedInterface {
    },
  }
}
pub trait IProtectedInterface: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.permission.IProtectedInterface" }
  fn Method1(&self) -> binder::public_api::Result<()>;
  fn Method2(&self) -> binder::public_api::Result<()>;
  fn getDefaultImpl() -> IProtectedInterfaceDefaultRef where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: IProtectedInterfaceDefaultRef) -> IProtectedInterfaceDefaultRef where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub trait IProtectedInterfaceDefault: Send + Sync {
  fn Method1(&self) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn Method2(&self) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
}
pub mod transactions {
  pub const Method1: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 0;
  pub const Method2: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 1;
}
pub type IProtectedInterfaceDefaultRef = Option<std::sync::Arc<dyn IProtectedInterfaceDefault>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<IProtectedInterfaceDefaultRef> = std::sync::Mutex::new(None);
}
impl IProtectedInterface for BpProtectedInterface {
  fn Method1(&self) -> binder::public_api::Result<()> {
    let _aidl_reply = self.binder.transact(transactions::Method1, binder::FLAG_PRIVATE_LOCAL, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IProtectedInterface>::getDefaultImpl() {
        return _aidl_default_impl.Method1();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
  fn Method2(&self) -> binder::public_api::Result<()> {
    let _aidl_reply = self.binder.transact(transactions::Method2, binder::FLAG_PRIVATE_LOCAL, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IProtectedInterface>::getDefaultImpl() {
        return _aidl_default_impl.Method2();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
}
impl IProtectedInterface for binder::Binder<BnProtectedInterface> {
  fn Method1(&self) -> binder::public_api::Result<()> { self.0.Method1() }
  fn Method2(&self) -> binder::public_api::Result<()> { self.0.Method2() }
}
fn on_transact(_aidl_service: &dyn IProtectedInterface, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::Parcel, _aidl_reply: &mut binder::parcel::Parcel) -> binder::Result<()> {
  match _aidl_code {
    transactions::Method1 => {
      let _aidl_return = _aidl_service.Method1();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::Method2 => {
      let _aidl_return = _aidl_service.Method2();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    _ => Err(binder::StatusCode::UNKNOWN_TRANSACTION)
  }
}
pub(crate) mod mangled {
 pub use super::IProtectedInterface as _7_android_4_aidl_5_tests_10_permission_19_IProtectedInterface;
}
