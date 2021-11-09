#![forbid(unsafe_code)]
#![rustfmt::skip]
#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinderInternal;
use binder::declare_binder_interface;
declare_binder_interface! {
  INestedService["android.aidl.tests.nested.INestedService"] {
    native: BnNestedService(on_transact),
    proxy: BpNestedService {
    },
    async: INestedServiceAsync,
  }
}
pub trait INestedService: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.nested.INestedService" }
  fn flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result>;
  fn flipStatusWithCallback(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status, _arg_cb: &binder::Strong<dyn crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback>) -> binder::public_api::Result<()>;
  fn getDefaultImpl() -> INestedServiceDefaultRef where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: INestedServiceDefaultRef) -> INestedServiceDefaultRef where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub trait INestedServiceAsync<P>: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.nested.INestedService" }
  fn flipStatus<'a>(&'a self, _arg_p: &'a crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::BoxFuture<'a, binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result>>;
  fn flipStatusWithCallback<'a>(&'a self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status, _arg_cb: &'a binder::Strong<dyn crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback>) -> binder::BoxFuture<'a, binder::public_api::Result<()>>;
}
pub trait INestedServiceDefault: Send + Sync {
  fn flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn flipStatusWithCallback(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status, _arg_cb: &binder::Strong<dyn crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback>) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
}
pub mod transactions {
  pub const flipStatus: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 0;
  pub const flipStatusWithCallback: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 1;
}
pub type INestedServiceDefaultRef = Option<std::sync::Arc<dyn INestedServiceDefault>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<INestedServiceDefaultRef> = std::sync::Mutex::new(None);
}
impl BpNestedService {
  fn build_parcel_flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::public_api::Result<binder::Parcel> {
    let mut aidl_data = self.binder.prepare_transact()?;
    aidl_data.write(_arg_p)?;
    Ok(aidl_data)
  }
  fn read_response_flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result> {
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as INestedService>::getDefaultImpl() {
        return _aidl_default_impl.flipStatus(_arg_p);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn build_parcel_flipStatusWithCallback(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status, _arg_cb: &binder::Strong<dyn crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback>) -> binder::public_api::Result<binder::Parcel> {
    let mut aidl_data = self.binder.prepare_transact()?;
    aidl_data.write(&_arg_status)?;
    aidl_data.write(_arg_cb)?;
    Ok(aidl_data)
  }
  fn read_response_flipStatusWithCallback(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status, _arg_cb: &binder::Strong<dyn crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback>, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<()> {
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as INestedService>::getDefaultImpl() {
        return _aidl_default_impl.flipStatusWithCallback(_arg_status, _arg_cb);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
}
impl INestedService for BpNestedService {
  fn flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result> {
    let _aidl_data = self.build_parcel_flipStatus(_arg_p)?;
    let _aidl_reply = self.binder.submit_transact(transactions::flipStatus, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
    self.read_response_flipStatus(_arg_p, _aidl_reply)
  }
  fn flipStatusWithCallback(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status, _arg_cb: &binder::Strong<dyn crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback>) -> binder::public_api::Result<()> {
    let _aidl_data = self.build_parcel_flipStatusWithCallback(_arg_status, _arg_cb)?;
    let _aidl_reply = self.binder.submit_transact(transactions::flipStatusWithCallback, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
    self.read_response_flipStatusWithCallback(_arg_status, _arg_cb, _aidl_reply)
  }
}
impl<P: binder::BinderAsyncPool> INestedServiceAsync<P> for BpNestedService {
  fn flipStatus<'a>(&'a self, _arg_p: &'a crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::BoxFuture<'a, binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result>> {
    let _aidl_data = match self.build_parcel_flipStatus(_arg_p) {
      Ok(_aidl_data) => _aidl_data,
      Err(err) => return Box::pin(std::future::ready(Err(err))),
    };
    let binder = self.binder.clone();
    P::spawn(
      move || binder.submit_transact(transactions::flipStatus, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
      move |_aidl_reply| async move {
        self.read_response_flipStatus(_arg_p, _aidl_reply)
      }
    )
  }
  fn flipStatusWithCallback<'a>(&'a self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status, _arg_cb: &'a binder::Strong<dyn crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback>) -> binder::BoxFuture<'a, binder::public_api::Result<()>> {
    let _aidl_data = match self.build_parcel_flipStatusWithCallback(_arg_status, _arg_cb) {
      Ok(_aidl_data) => _aidl_data,
      Err(err) => return Box::pin(std::future::ready(Err(err))),
    };
    let binder = self.binder.clone();
    P::spawn(
      move || binder.submit_transact(transactions::flipStatusWithCallback, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
      move |_aidl_reply| async move {
        self.read_response_flipStatusWithCallback(_arg_status, _arg_cb, _aidl_reply)
      }
    )
  }
}
impl INestedService for binder::Binder<BnNestedService> {
  fn flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result> { self.0.flipStatus(_arg_p) }
  fn flipStatusWithCallback(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status, _arg_cb: &binder::Strong<dyn crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback>) -> binder::public_api::Result<()> { self.0.flipStatusWithCallback(_arg_status, _arg_cb) }
}
fn on_transact(_aidl_service: &dyn INestedService, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::BorrowedParcel<'_>, _aidl_reply: &mut binder::parcel::BorrowedParcel<'_>) -> binder::Result<()> {
  match _aidl_code {
    transactions::flipStatus => {
      let _arg_p: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested = _aidl_data.read()?;
      let _aidl_return = _aidl_service.flipStatus(&_arg_p);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::flipStatusWithCallback => {
      let _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status = _aidl_data.read()?;
      let _arg_cb: binder::Strong<dyn crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback> = _aidl_data.read()?;
      let _aidl_return = _aidl_service.flipStatusWithCallback(_arg_status, &_arg_cb);
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
pub mod Result {
  #[derive(Debug, PartialEq)]
  pub struct Result {
    pub status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status,
  }
  impl Default for Result {
    fn default() -> Self {
      Self {
        status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status::OK,
      }
    }
  }
  impl binder::parcel::Parcelable for Result {
    fn write_to_parcel(&self, parcel: &mut binder::parcel::BorrowedParcel) -> binder::Result<()> {
      parcel.sized_write(|subparcel| {
        subparcel.write(&self.status)?;
        Ok(())
      })
    }
    fn read_from_parcel(&mut self, parcel: &binder::parcel::BorrowedParcel) -> binder::Result<()> {
      parcel.sized_read(|subparcel| {
        if subparcel.has_more_data() {
          self.status = subparcel.read()?;
        }
        Ok(())
      })
    }
  }
  binder::impl_serialize_for_parcelable!(Result);
  binder::impl_deserialize_for_parcelable!(Result);
  impl binder::parcel::ParcelableMetadata for Result {
    fn get_descriptor() -> &'static str { "android.aidl.tests.nested.INestedService.Result" }
  }
}
pub mod ICallback {
  #![allow(non_upper_case_globals)]
  #![allow(non_snake_case)]
  #[allow(unused_imports)] use binder::IBinderInternal;
  use binder::declare_binder_interface;
  declare_binder_interface! {
    ICallback["android.aidl.tests.nested.INestedService.ICallback"] {
      native: BnCallback(on_transact),
      proxy: BpCallback {
      },
      async: ICallbackAsync,
    }
  }
  pub trait ICallback: binder::Interface + Send {
    fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.nested.INestedService.ICallback" }
    fn done(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status) -> binder::public_api::Result<()>;
    fn getDefaultImpl() -> ICallbackDefaultRef where Self: Sized {
      DEFAULT_IMPL.lock().unwrap().clone()
    }
    fn setDefaultImpl(d: ICallbackDefaultRef) -> ICallbackDefaultRef where Self: Sized {
      std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
    }
  }
  pub trait ICallbackAsync<P>: binder::Interface + Send {
    fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.nested.INestedService.ICallback" }
    fn done<'a>(&'a self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status) -> binder::BoxFuture<'a, binder::public_api::Result<()>>;
  }
  pub trait ICallbackDefault: Send + Sync {
    fn done(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status) -> binder::public_api::Result<()> {
      Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
    }
  }
  pub mod transactions {
    pub const done: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 0;
  }
  pub type ICallbackDefaultRef = Option<std::sync::Arc<dyn ICallbackDefault>>;
  use lazy_static::lazy_static;
  lazy_static! {
    static ref DEFAULT_IMPL: std::sync::Mutex<ICallbackDefaultRef> = std::sync::Mutex::new(None);
  }
  impl BpCallback {
    fn build_parcel_done(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status) -> binder::public_api::Result<binder::Parcel> {
      let mut aidl_data = self.binder.prepare_transact()?;
      aidl_data.write(&_arg_status)?;
      Ok(aidl_data)
    }
    fn read_response_done(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status, _aidl_reply: binder::Result<binder::Parcel>) -> binder::public_api::Result<()> {
      if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
        if let Some(_aidl_default_impl) = <Self as ICallback>::getDefaultImpl() {
          return _aidl_default_impl.done(_arg_status);
        }
      }
      let _aidl_reply = _aidl_reply?;
      let _aidl_status: binder::Status = _aidl_reply.read()?;
      if !_aidl_status.is_ok() { return Err(_aidl_status); }
      Ok(())
    }
  }
  impl ICallback for BpCallback {
    fn done(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status) -> binder::public_api::Result<()> {
      let _aidl_data = self.build_parcel_done(_arg_status)?;
      let _aidl_reply = self.binder.submit_transact(transactions::done, _aidl_data, binder::FLAG_PRIVATE_LOCAL);
      self.read_response_done(_arg_status, _aidl_reply)
    }
  }
  impl<P: binder::BinderAsyncPool> ICallbackAsync<P> for BpCallback {
    fn done<'a>(&'a self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status) -> binder::BoxFuture<'a, binder::public_api::Result<()>> {
      let _aidl_data = match self.build_parcel_done(_arg_status) {
        Ok(_aidl_data) => _aidl_data,
        Err(err) => return Box::pin(std::future::ready(Err(err))),
      };
      let binder = self.binder.clone();
      P::spawn(
        move || binder.submit_transact(transactions::done, _aidl_data, binder::FLAG_PRIVATE_LOCAL),
        move |_aidl_reply| async move {
          self.read_response_done(_arg_status, _aidl_reply)
        }
      )
    }
  }
  impl ICallback for binder::Binder<BnCallback> {
    fn done(&self, _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status) -> binder::public_api::Result<()> { self.0.done(_arg_status) }
  }
  fn on_transact(_aidl_service: &dyn ICallback, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::BorrowedParcel<'_>, _aidl_reply: &mut binder::parcel::BorrowedParcel<'_>) -> binder::Result<()> {
    match _aidl_code {
      transactions::done => {
        let _arg_status: crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested_6_Status = _aidl_data.read()?;
        let _aidl_return = _aidl_service.done(_arg_status);
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
}
pub(crate) mod mangled {
 pub use super::INestedService as _7_android_4_aidl_5_tests_6_nested_14_INestedService;
 pub use super::Result::Result as _7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result;
 pub use super::ICallback::ICallback as _7_android_4_aidl_5_tests_6_nested_14_INestedService_9_ICallback;
}
