#![forbid(unsafe_code)]
#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinderInternal;
use binder::declare_binder_interface;
declare_binder_interface! {
  IProtected["android.aidl.tests.IProtected"] {
    native: BnProtected(on_transact),
    proxy: BpProtected {
    },
  }
}
pub trait IProtected: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.IProtected" }
  fn PermissionProtected(&self) -> binder::public_api::Result<()>;
  fn MultiplePermissions(&self) -> binder::public_api::Result<()>;
  fn MultiplePermissions2(&self) -> binder::public_api::Result<()>;
  fn getDefaultImpl() -> IProtectedDefaultRef where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: IProtectedDefaultRef) -> IProtectedDefaultRef where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub trait IProtectedDefault: Send + Sync {
  fn PermissionProtected(&self) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn MultiplePermissions(&self) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn MultiplePermissions2(&self) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
}
pub mod transactions {
  pub const PermissionProtected: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 0;
  pub const MultiplePermissions: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 1;
  pub const MultiplePermissions2: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 2;
}
pub type IProtectedDefaultRef = Option<std::sync::Arc<dyn IProtectedDefault>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<IProtectedDefaultRef> = std::sync::Mutex::new(None);
}
pub(crate) mod mangled { pub use super::IProtected as _7_android_4_aidl_5_tests_10_IProtected; }
impl IProtected for BpProtected {
  fn PermissionProtected(&self) -> binder::public_api::Result<()> {
    let _aidl_reply = self.binder.transact(transactions::PermissionProtected, binder::FLAG_PRIVATE_LOCAL, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IProtected>::getDefaultImpl() {
        return _aidl_default_impl.PermissionProtected();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
  fn MultiplePermissions(&self) -> binder::public_api::Result<()> {
    let _aidl_reply = self.binder.transact(transactions::MultiplePermissions, binder::FLAG_PRIVATE_LOCAL, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IProtected>::getDefaultImpl() {
        return _aidl_default_impl.MultiplePermissions();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
  fn MultiplePermissions2(&self) -> binder::public_api::Result<()> {
    let _aidl_reply = self.binder.transact(transactions::MultiplePermissions2, binder::FLAG_PRIVATE_LOCAL, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IProtected>::getDefaultImpl() {
        return _aidl_default_impl.MultiplePermissions2();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
}
impl IProtected for binder::Binder<BnProtected> {
  fn PermissionProtected(&self) -> binder::public_api::Result<()> { self.0.PermissionProtected() }
  fn MultiplePermissions(&self) -> binder::public_api::Result<()> { self.0.MultiplePermissions() }
  fn MultiplePermissions2(&self) -> binder::public_api::Result<()> { self.0.MultiplePermissions2() }
}
fn on_transact(_aidl_service: &dyn IProtected, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::Parcel, _aidl_reply: &mut binder::parcel::Parcel) -> binder::Result<()> {
  match _aidl_code {
    transactions::PermissionProtected => {
      let _aidl_return = _aidl_service.PermissionProtected();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::MultiplePermissions => {
      let _aidl_return = _aidl_service.MultiplePermissions();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::MultiplePermissions2 => {
      let _aidl_return = _aidl_service.MultiplePermissions2();
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
