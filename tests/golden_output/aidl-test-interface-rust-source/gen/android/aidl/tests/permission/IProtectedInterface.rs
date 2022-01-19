#![forbid(unsafe_code)]
#![rustfmt::skip]
#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinderInternal;
use binder::declare_binder_interface;
declare_binder_interface! {
  IProtectedInterface["android.aidl.tests.permission.IProtectedInterface"] {
    native: BnProtectedInterface(on_transact),
    proxy: BpProtectedInterface {
    },
    async: IProtectedInterfaceAsync,
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
pub trait IProtectedInterfaceAsync<P>: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.permission.IProtectedInterface" }
  fn Method1<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>>;
  fn Method2<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>>;
}
#[::async_trait::async_trait]
pub trait IProtectedInterfaceAsyncServer: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.permission.IProtectedInterface" }
  async fn Method1(&self) -> binder::public_api::Result<()>;
  async fn Method2(&self) -> binder::public_api::Result<()>;
}
impl BnProtectedInterface {
  /// Create a new async binder service.
  pub fn new_async_binder<T, R>(inner: T, rt: R, features: binder::BinderFeatures) -> binder::Strong<dyn IProtectedInterface>
  where
    T: IProtectedInterfaceAsyncServer + binder::Interface + Send + Sync + 'static,
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
    impl<T, R> IProtectedInterface for Wrapper<T, R>
    where
      T: IProtectedInterfaceAsyncServer + Send + Sync + 'static,
      R: binder::BinderAsyncRuntime + Send + Sync + 'static,
    {
      fn Method1(&self) -> binder::public_api::Result<()> {
        self._rt.block_on(self._inner.Method1())
      }
      fn Method2(&self) -> binder::public_api::Result<()> {
        self._rt.block_on(self._inner.Method2())
      }
    }
    let wrapped = Wrapper { _inner: inner, _rt: rt };
    Self::new_binder(wrapped, features)
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
impl BpProtectedInterface {
  fn build_parcel_Method1(&self) -> binder::public_api::Result<binder::Parcel> {
    let mut aidl_data = self.binder.prepare_transact()?;
    Ok(aidl_data)
  }
  fn read_response_Method1(&self, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<()> {
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
  fn build_parcel_Method2(&self) -> binder::public_api::Result<binder::Parcel> {
    let mut aidl_data = self.binder.prepare_transact()?;
    Ok(aidl_data)
  }
  fn read_response_Method2(&self, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<()> {
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
impl IProtectedInterface for BpProtectedInterface {
  fn Method1(&self) -> binder::public_api::Result<()> {
    let _aidl_data = self.build_parcel_Method1()?;
    let _aidl_reply = self.binder.submit_transact(transactions::Method1, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
    self.read_response_Method1(_aidl_reply)
  }
  fn Method2(&self) -> binder::public_api::Result<()> {
    let _aidl_data = self.build_parcel_Method2()?;
    let _aidl_reply = self.binder.submit_transact(transactions::Method2, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
    self.read_response_Method2(_aidl_reply)
  }
}
impl<P: binder::BinderAsyncPool> IProtectedInterfaceAsync<P> for BpProtectedInterface {
  fn Method1<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>> {
    let _aidl_data = match self.build_parcel_Method1() {
      Ok(_aidl_data) => _aidl_data,
      Err(err) => return Box::pin(std::future::ready(Err(err))),
    };
    let binder = self.binder.clone();
    P::spawn(
      move || binder.submit_transact(transactions::Method1, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
      move |_aidl_reply| async move {
        self.read_response_Method1(_aidl_reply)
      }
    )
  }
  fn Method2<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<()>> {
    let _aidl_data = match self.build_parcel_Method2() {
      Ok(_aidl_data) => _aidl_data,
      Err(err) => return Box::pin(std::future::ready(Err(err))),
    };
    let binder = self.binder.clone();
    P::spawn(
      move || binder.submit_transact(transactions::Method2, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
      move |_aidl_reply| async move {
        self.read_response_Method2(_aidl_reply)
      }
    )
  }
}
impl IProtectedInterface for binder::Binder<BnProtectedInterface> {
  fn Method1(&self) -> binder::public_api::Result<()> { self.0.Method1() }
  fn Method2(&self) -> binder::public_api::Result<()> { self.0.Method2() }
}
fn on_transact(_aidl_service: &dyn IProtectedInterface, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::BorrowedParcel<'_>, _aidl_reply: &mut binder::parcel::BorrowedParcel<'_>) -> binder::Result<()> {
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
