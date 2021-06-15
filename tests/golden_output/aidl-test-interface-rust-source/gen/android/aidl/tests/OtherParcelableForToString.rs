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
impl binder::parcel::Serialize for OtherParcelableForToString {
  fn serialize(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    <Self as binder::parcel::SerializeOption>::serialize_option(Some(self), parcel)
  }
}
impl binder::parcel::SerializeArray for OtherParcelableForToString {}
impl binder::parcel::SerializeOption for OtherParcelableForToString {
  fn serialize_option(this: Option<&Self>, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    let this = if let Some(this) = this {
      parcel.write(&1i32)?;
      this
    } else {
      return parcel.write(&0i32);
    };
    parcel.sized_write(|subparcel| {
      subparcel.write(&this.field)?;
      Ok(())
    })
  }
}
binder::impl_deserialize_for_parcelable!(OtherParcelableForToString);
impl OtherParcelableForToString {
  fn deserialize_parcelable(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    let start_pos = parcel.get_data_position();
    let parcelable_size: i32 = parcel.read()?;
    if parcelable_size < 0 { return Err(binder::StatusCode::BAD_VALUE); }
    if start_pos.checked_add(parcelable_size).is_none() {
      return Err(binder::StatusCode::BAD_VALUE);
    }
    if (parcel.get_data_position() - start_pos) == parcelable_size {
      return Ok(());
    }
    self.field = parcel.read()?;
    unsafe {
      parcel.set_data_position(start_pos + parcelable_size)?;
    }
    Ok(())
  }
}
