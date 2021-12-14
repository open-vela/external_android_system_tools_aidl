#![forbid(unsafe_code)]
#![rustfmt::skip]
#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinderInternal;
use binder::declare_binder_interface;
declare_binder_interface! {
  IProtected["android.aidl.tests.permission.IProtected"] {
    native: BnProtected(on_transact),
    proxy: BpProtected {
    },
    async: IProtectedAsync,
  }
}
pub trait IProtected: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.permission.IProtected" }
  fn PermissionProtected(&self) -> binder::public_api::Result<()>;
  fn MultiplePermissionsAll(&self) -> binder::public_api::Result<()>;
  fn MultiplePermissionsAny(&self) -> binder::public_api::Result<()>;
  fn getDefaultImpl() -> IProtectedDefaultRef where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: IProtectedDefaultRef) -> IProtectedDefaultRef where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub trait IProtectedAsync<P>: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.permission.IProtected" }
  fn PermissionProtected<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>>;
  fn MultiplePermissionsAll<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>>;
  fn MultiplePermissionsAny<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>>;
}
#[::async_trait::async_trait]
pub trait IProtectedAsyncServer: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.permission.IProtected" }
  async fn PermissionProtected(&self) -> binder::public_api::Result<()>;
  async fn MultiplePermissionsAll(&self) -> binder::public_api::Result<()>;
  async fn MultiplePermissionsAny(&self) -> binder::public_api::Result<()>;
}
impl BnProtected {
  /// Create a new async binder service.
  pub fn new_async_binder<T, R>(inner: T, rt: R, features: binder::BinderFeatures) -> binder::Strong<dyn IProtected>
  where
    T: IProtectedAsyncServer + binder::Interface + Send + Sync + 'static,
    R: binder::BinderAsyncRuntime + Send + Sync + 'static,
  {
    struct Wrapper<T, R> {
      _inner: T,
      _rt: R,
    }
    impl<T, R> binder::Interface for Wrapper<T, R> where T: binder::Interface, R: Send + Sync {
      fn as_binder(&self) -> binder::SpIBinder { self._inner.as_binder() }
      fn dump(&self, _file: &std::fs::File, _args: &[&std::ffi::CStr]) -> binder::Result<()> { self._inner.dump(_file, _args) }
    }
    impl<T, R> IProtected for Wrapper<T, R>
    where
      T: IProtectedAsyncServer + Send + Sync + 'static,
      R: binder::BinderAsyncRuntime + Send + Sync + 'static,
    {
      fn PermissionProtected(&self) -> binder::public_api::Result<()> {
        self._rt.block_on(self._inner.PermissionProtected())
      }
      fn MultiplePermissionsAll(&self) -> binder::public_api::Result<()> {
        self._rt.block_on(self._inner.MultiplePermissionsAll())
      }
      fn MultiplePermissionsAny(&self) -> binder::public_api::Result<()> {
        self._rt.block_on(self._inner.MultiplePermissionsAny())
      }
    }
    let wrapped = Wrapper { _inner: inner, _rt: rt };
    Self::new_binder(wrapped, features)
  }
}
pub trait IProtectedDefault: Send + Sync {
  fn PermissionProtected(&self) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn MultiplePermissionsAll(&self) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn MultiplePermissionsAny(&self) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
}
pub mod transactions {
  pub const PermissionProtected: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 0;
  pub const MultiplePermissionsAll: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 1;
  pub const MultiplePermissionsAny: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 2;
}
pub type IProtectedDefaultRef = Option<std::sync::Arc<dyn IProtectedDefault>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<IProtectedDefaultRef> = std::sync::Mutex::new(None);
}
impl BpProtected {
  fn build_parcel_PermissionProtected(&self) -> binder::public_api::Result<binder::Parcel> {
    let mut aidl_data = self.binder.prepare_transact()?;
    Ok(aidl_data)
  }
  fn read_response_PermissionProtected(&self, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<()> {
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
  fn build_parcel_MultiplePermissionsAll(&self) -> binder::public_api::Result<binder::Parcel> {
    let mut aidl_data = self.binder.prepare_transact()?;
    Ok(aidl_data)
  }
  fn read_response_MultiplePermissionsAll(&self, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<()> {
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IProtected>::getDefaultImpl() {
        return _aidl_default_impl.MultiplePermissionsAll();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
  fn build_parcel_MultiplePermissionsAny(&self) -> binder::public_api::Result<binder::Parcel> {
    let mut aidl_data = self.binder.prepare_transact()?;
    Ok(aidl_data)
  }
  fn read_response_MultiplePermissionsAny(&self, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<()> {
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IProtected>::getDefaultImpl() {
        return _aidl_default_impl.MultiplePermissionsAny();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
}
impl IProtected for BpProtected {
  fn PermissionProtected(&self) -> binder::public_api::Result<()> {
    let _aidl_data = self.build_parcel_PermissionProtected()?;
    let _aidl_reply = self.binder.submit_transact(transactions::PermissionProtected, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
    self.read_response_PermissionProtected(_aidl_reply)
  }
  fn MultiplePermissionsAll(&self) -> binder::public_api::Result<()> {
    let _aidl_data = self.build_parcel_MultiplePermissionsAll()?;
    let _aidl_reply = self.binder.submit_transact(transactions::MultiplePermissionsAll, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
    self.read_response_MultiplePermissionsAll(_aidl_reply)
  }
  fn MultiplePermissionsAny(&self) -> binder::public_api::Result<()> {
    let _aidl_data = self.build_parcel_MultiplePermissionsAny()?;
    let _aidl_reply = self.binder.submit_transact(transactions::MultiplePermissionsAny, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
    self.read_response_MultiplePermissionsAny(_aidl_reply)
  }
}
impl<P: binder::BinderAsyncPool> IProtectedAsync<P> for BpProtected {
  fn PermissionProtected<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>> {
    let _aidl_data = match self.build_parcel_PermissionProtected() {
      Ok(_aidl_data) => _aidl_data,
      Err(err) => return Box::pin(std::future::ready(Err(err))),
    };
    let binder = self.binder.clone();
    P::spawn(
      move || binder.submit_transact(transactions::PermissionProtected, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
      move |_aidl_reply| async move {
        self.read_response_PermissionProtected(_aidl_reply)
      }
    )
  }
  fn MultiplePermissionsAll<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>> {
    let _aidl_data = match self.build_parcel_MultiplePermissionsAll() {
      Ok(_aidl_data) => _aidl_data,
      Err(err) => return Box::pin(std::future::ready(Err(err))),
    };
    let binder = self.binder.clone();
    P::spawn(
      move || binder.submit_transact(transactions::MultiplePermissionsAll, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
      move |_aidl_reply| async move {
        self.read_response_MultiplePermissionsAll(_aidl_reply)
      }
    )
  }
  fn MultiplePermissionsAny<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>> {
    let _aidl_data = match self.build_parcel_MultiplePermissionsAny() {
      Ok(_aidl_data) => _aidl_data,
      Err(err) => return Box::pin(std::future::ready(Err(err))),
    };
    let binder = self.binder.clone();
    P::spawn(
      move || binder.submit_transact(transactions::MultiplePermissionsAny, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
      move |_aidl_reply| async move {
        self.read_response_MultiplePermissionsAny(_aidl_reply)
      }
    )
  }
}
impl IProtected for binder::Binder<BnProtected> {
  fn PermissionProtected(&self) -> binder::public_api::Result<()> { self.0.PermissionProtected() }
  fn MultiplePermissionsAll(&self) -> binder::public_api::Result<()> { self.0.MultiplePermissionsAll() }
  fn MultiplePermissionsAny(&self) -> binder::public_api::Result<()> { self.0.MultiplePermissionsAny() }
}
fn on_transact(_aidl_service: &dyn IProtected, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::BorrowedParcel<'_>, _aidl_reply: &mut binder::parcel::BorrowedParcel<'_>) -> binder::Result<()> {
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
    transactions::MultiplePermissionsAll => {
      let _aidl_return = _aidl_service.MultiplePermissionsAll();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::MultiplePermissionsAny => {
      let _aidl_return = _aidl_service.MultiplePermissionsAny();
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
 pub use super::IProtected as _7_android_4_aidl_5_tests_10_permission_10_IProtected;
}
