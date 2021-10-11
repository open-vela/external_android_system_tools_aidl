#![forbid(unsafe_code)]
#[derive(Debug)]
pub struct OtherParcelableForToString {
  pub field: String,
}
pub(crate) mod mangled { pub use super::OtherParcelableForToString as _7_android_4_aidl_5_tests_26_OtherParcelableForToString; }
impl Default for OtherParcelableForToString {
  fn default() -> Self {
    Self {
      field: Default::default(),
    }
  }
}
impl binder::parcel::Parcelable for OtherParcelableForToString {
  fn write_to_parcel(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    parcel.sized_write(|subparcel| {
      subparcel.write(&self.field)?;
      Ok(())
    })
  }
  fn read_from_parcel(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    parcel.sized_read(|subparcel| {
      if subparcel.has_more_data() {
        self.field = subparcel.read()?;
      }
      Ok(())
    })
  }
}
binder::impl_serialize_for_parcelable!(OtherParcelableForToString);
binder::impl_deserialize_for_parcelable!(OtherParcelableForToString);
