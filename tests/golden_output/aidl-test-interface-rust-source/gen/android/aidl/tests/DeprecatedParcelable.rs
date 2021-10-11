#![forbid(unsafe_code)]
#[derive(Debug)]
#[deprecated = "test"]
pub struct DeprecatedParcelable {
}
pub(crate) mod mangled { pub use super::DeprecatedParcelable as _7_android_4_aidl_5_tests_20_DeprecatedParcelable; }
impl Default for DeprecatedParcelable {
  fn default() -> Self {
    Self {
    }
  }
}
impl binder::parcel::Parcelable for DeprecatedParcelable {
  fn write_to_parcel(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    parcel.sized_write(|subparcel| {
      Ok(())
    })
  }
  fn read_from_parcel(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    parcel.sized_read(|subparcel| {
      Ok(())
    })
  }
}
binder::impl_serialize_for_parcelable!(DeprecatedParcelable);
binder::impl_deserialize_for_parcelable!(DeprecatedParcelable);
impl binder::parcel::ParcelableMetadata for DeprecatedParcelable {
  fn get_descriptor() -> &'static str { "android.aidl.tests.DeprecatedParcelable" }
}
