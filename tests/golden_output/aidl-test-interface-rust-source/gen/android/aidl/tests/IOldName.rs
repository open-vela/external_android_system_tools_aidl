#![forbid(unsafe_code)]
#![rustfmt::skip]
#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinderInternal;
use binder::declare_binder_interface;
declare_binder_interface! {
  IOldName["android.aidl.tests.IOldName"] {
    native: BnOldName(on_transact),
    proxy: BpOldName {
    },
    async: IOldNameAsync,
  }
}
pub trait IOldName: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.IOldName" }
  fn RealName(&self) -> binder::public_api::Result<String>;
  fn getDefaultImpl() -> IOldNameDefaultRef where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: IOldNameDefaultRef) -> IOldNameDefaultRef where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub trait IOldNameAsync<P>: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.IOldName" }
  fn RealName<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<String>>;
}
#[::async_trait::async_trait]
pub trait IOldNameAsyncServer: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.IOldName" }
  async fn RealName(&self) -> binder::public_api::Result<String>;
}
impl BnOldName {
  /// Create a new async binder service.
  pub fn new_async_binder<T, R>(inner: T, rt: R, features: binder::BinderFeatures) -> binder::Strong<dyn IOldName>
  where
    T: IOldNameAsyncServer + binder::Interface + Send + Sync + 'static,
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
    impl<T, R> IOldName for Wrapper<T, R>
    where
      T: IOldNameAsyncServer + Send + Sync + 'static,
      R: binder::BinderAsyncRuntime + Send + Sync + 'static,
    {
      fn RealName(&self) -> binder::public_api::Result<String> {
        self._rt.block_on(self._inner.RealName())
      }
    }
    let wrapped = Wrapper { _inner: inner, _rt: rt };
    Self::new_binder(wrapped, features)
  }
}
pub trait IOldNameDefault: Send + Sync {
  fn RealName(&self) -> binder::public_api::Result<String> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
}
pub mod transactions {
  pub const RealName: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 0;
}
pub type IOldNameDefaultRef = Option<std::sync::Arc<dyn IOldNameDefault>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<IOldNameDefaultRef> = std::sync::Mutex::new(None);
}
impl BpOldName {
  fn build_parcel_RealName(&self) -> binder::public_api::Result<binder::Parcel> {
    let mut aidl_data = self.binder.prepare_transact()?;
    Ok(aidl_data)
  }
  fn read_response_RealName(&self, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<String> {
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
impl IOldName for BpOldName {
  fn RealName(&self) -> binder::public_api::Result<String> {
    let _aidl_data = self.build_parcel_RealName()?;
    let _aidl_reply = self.binder.submit_transact(transactions::RealName, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
    self.read_response_RealName(_aidl_reply)
  }
}
impl<P: binder::BinderAsyncPool> IOldNameAsync<P> for BpOldName {
  fn RealName<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<String>> {
    let _aidl_data = match self.build_parcel_RealName() {
      Ok(_aidl_data) => _aidl_data,
      Err(err) => return Box::pin(std::future::ready(Err(err))),
    };
    let binder = self.binder.clone();
    P::spawn(
      move || binder.submit_transact(transactions::RealName, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
      move |_aidl_reply| async move {
        self.read_response_RealName(_aidl_reply)
      }
    )
  }
}
impl IOldName for binder::Binder<BnOldName> {
  fn RealName(&self) -> binder::public_api::Result<String> { self.0.RealName() }
}
fn on_transact(_aidl_service: &dyn IOldName, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::BorrowedParcel<'_>, _aidl_reply: &mut binder::parcel::BorrowedParcel<'_>) -> binder::Result<()> {
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
pub(crate) mod mangled {
 pub use super::IOldName as _7_android_4_aidl_5_tests_8_IOldName;
}
