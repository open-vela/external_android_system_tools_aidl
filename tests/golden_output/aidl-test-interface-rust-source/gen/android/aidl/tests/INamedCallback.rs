#![forbid(unsafe_code)]
#![rustfmt::skip]
#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinderInternal;
use binder::declare_binder_interface;
declare_binder_interface! {
  INamedCallback["android.aidl.tests.INamedCallback"] {
    native: BnNamedCallback(on_transact),
    proxy: BpNamedCallback {
    },
    async: INamedCallbackAsync,
  }
}
pub trait INamedCallback: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.INamedCallback" }
  fn GetName(&self) -> binder::public_api::Result<String>;
  fn getDefaultImpl() -> INamedCallbackDefaultRef where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: INamedCallbackDefaultRef) -> INamedCallbackDefaultRef where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub trait INamedCallbackAsync<P>: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.INamedCallback" }
  fn GetName<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<String>>;
}
pub trait INamedCallbackDefault: Send + Sync {
  fn GetName(&self) -> binder::public_api::Result<String> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
}
pub mod transactions {
  pub const GetName: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 0;
}
pub type INamedCallbackDefaultRef = Option<std::sync::Arc<dyn INamedCallbackDefault>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<INamedCallbackDefaultRef> = std::sync::Mutex::new(None);
}
impl BpNamedCallback {
  fn build_parcel_GetName(&self) -> binder::public_api::Result<binder::Parcel> {
    let mut aidl_data = self.binder.prepare_transact()?;
    Ok(aidl_data)
  }
  fn read_response_GetName(&self, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<String> {
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as INamedCallback>::getDefaultImpl() {
        return _aidl_default_impl.GetName();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
}
impl INamedCallback for BpNamedCallback {
  fn GetName(&self) -> binder::public_api::Result<String> {
    let _aidl_data = self.build_parcel_GetName()?;
    let _aidl_reply = self.binder.submit_transact(transactions::GetName, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
    self.read_response_GetName(_aidl_reply)
  }
}
impl<P: binder::BinderAsyncPool> INamedCallbackAsync<P> for BpNamedCallback {
  fn GetName<'a>(&'a self) -> binder::BoxFuture<'a, binder::public_api::Result<String>> {
    let _aidl_data = match self.build_parcel_GetName() {
      Ok(_aidl_data) => _aidl_data,
      Err(err) => return Box::pin(std::future::ready(Err(err))),
    };
    let binder = self.binder.clone();
    P::spawn(
      move || binder.submit_transact(transactions::GetName, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
      move |_aidl_reply| async move {
        self.read_response_GetName(_aidl_reply)
      }
    )
  }
}
impl INamedCallback for binder::Binder<BnNamedCallback> {
  fn GetName(&self) -> binder::public_api::Result<String> { self.0.GetName() }
}
fn on_transact(_aidl_service: &dyn INamedCallback, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::BorrowedParcel<'_>, _aidl_reply: &mut binder::parcel::BorrowedParcel<'_>) -> binder::Result<()> {
  match _aidl_code {
    transactions::GetName => {
      let _aidl_return = _aidl_service.GetName();
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
 pub use super::INamedCallback as _7_android_4_aidl_5_tests_14_INamedCallback;
}
