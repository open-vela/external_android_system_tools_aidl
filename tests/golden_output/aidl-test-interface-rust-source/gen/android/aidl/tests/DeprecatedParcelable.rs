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
impl binder::parcel::Serialize for DeprecatedParcelable {
  fn serialize(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    <Self as binder::parcel::SerializeOption>::serialize_option(Some(self), parcel)
  }
}
impl binder::parcel::SerializeArray for DeprecatedParcelable {}
impl binder::parcel::SerializeOption for DeprecatedParcelable {
  fn serialize_option(this: Option<&Self>, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    let this = if let Some(this) = this {
      parcel.write(&1i32)?;
      this
    } else {
      return parcel.write(&0i32);
    };
    parcel.sized_write(|subparcel| {
      Ok(())
    })
  }
}
binder::impl_deserialize_for_parcelable!(DeprecatedParcelable);
impl DeprecatedParcelable {
  fn deserialize_parcelable(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    parcel.sized_read(|subparcel| {
      Ok(())
    })
  }
}
