#![forbid(unsafe_code)]
#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinderInternal;
use binder::declare_binder_interface;
declare_binder_interface! {
  INestedService["android.aidl.tests.nested.INestedService"] {
    native: BnNestedService(on_transact),
    proxy: BpNestedService {
    },
  }
}
pub trait INestedService: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.aidl.tests.nested.INestedService" }
  fn flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result>;
  fn getDefaultImpl() -> INestedServiceDefaultRef where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: INestedServiceDefaultRef) -> INestedServiceDefaultRef where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub trait INestedServiceDefault: Send + Sync {
  fn flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
}
pub mod transactions {
  pub const flipStatus: binder::TransactionCode = binder::FIRST_CALL_TRANSACTION + 0;
}
pub type INestedServiceDefaultRef = Option<std::sync::Arc<dyn INestedServiceDefault>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<INestedServiceDefaultRef> = std::sync::Mutex::new(None);
}
impl INestedService for BpNestedService {
  fn flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result> {
    let _aidl_reply = self.binder.transact(transactions::flipStatus, binder::FLAG_PRIVATE_LOCAL, |_aidl_data| {
      _aidl_data.write(_arg_p)?;
      Ok(())
    });
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
}
impl INestedService for binder::Binder<BnNestedService> {
  fn flipStatus(&self, _arg_p: &crate::mangled::_7_android_4_aidl_5_tests_6_nested_20_ParcelableWithNested) -> binder::public_api::Result<crate::mangled::_7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result> { self.0.flipStatus(_arg_p) }
}
fn on_transact(_aidl_service: &dyn INestedService, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::Parcel, _aidl_reply: &mut binder::parcel::Parcel) -> binder::Result<()> {
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
    fn write_to_parcel(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
      parcel.sized_write(|subparcel| {
        subparcel.write(&self.status)?;
        Ok(())
      })
    }
    fn read_from_parcel(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
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
pub(crate) mod mangled {
 pub use super::INestedService as _7_android_4_aidl_5_tests_6_nested_14_INestedService;
 pub use super::Result::Result as _7_android_4_aidl_5_tests_6_nested_14_INestedService_6_Result;
}
