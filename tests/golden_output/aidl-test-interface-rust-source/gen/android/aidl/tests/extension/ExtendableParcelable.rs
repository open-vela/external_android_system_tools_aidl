#![forbid(unsafe_code)]
#[derive(Debug)]
pub struct ExtendableParcelable {
  pub a: i32,
  pub b: String,
  pub ext: binder::parcel::ParcelableHolder,
  pub c: i64,
  pub ext2: binder::parcel::ParcelableHolder,
}
pub(crate) mod mangled { pub use super::ExtendableParcelable as _7_android_4_aidl_5_tests_9_extension_20_ExtendableParcelable; }
impl Default for ExtendableParcelable {
  fn default() -> Self {
    Self {
      a: 0,
      b: Default::default(),
      ext: binder::parcel::ParcelableHolder::new(binder::Stability::Local),
      c: 0,
      ext2: binder::parcel::ParcelableHolder::new(binder::Stability::Local),
    }
  }
}
impl binder::parcel::Parcelable for ExtendableParcelable {
  fn write_to_parcel(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    parcel.sized_write(|subparcel| {
      subparcel.write(&self.a)?;
      subparcel.write(&self.b)?;
      subparcel.write(&self.ext)?;
      subparcel.write(&self.c)?;
      subparcel.write(&self.ext2)?;
      Ok(())
    })
  }
  fn read_from_parcel(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    parcel.sized_read(|subparcel| {
      if subparcel.has_more_data() {
        self.a = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.b = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.ext = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.c = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.ext2 = subparcel.read()?;
      }
      Ok(())
    })
  }
}
binder::impl_serialize_for_parcelable!(ExtendableParcelable);
binder::impl_deserialize_for_parcelable!(ExtendableParcelable);
impl binder::parcel::ParcelableMetadata for ExtendableParcelable {
  fn get_descriptor() -> &'static str { "android.aidl.tests.extension.ExtendableParcelable" }
}
