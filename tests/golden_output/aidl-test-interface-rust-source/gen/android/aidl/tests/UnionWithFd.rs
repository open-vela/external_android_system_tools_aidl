#![forbid(unsafe_code)]
#[derive(Debug)]
pub enum UnionWithFd {
  Num(i32),
  Pfd(Option<binder::parcel::ParcelFileDescriptor>),
}
impl Default for UnionWithFd {
  fn default() -> Self {
    Self::Num(0)
  }
}
impl binder::parcel::Parcelable for UnionWithFd {
  fn write_to_parcel(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    match self {
      Self::Num(v) => {
        parcel.write(&0i32)?;
        parcel.write(v)
      }
      Self::Pfd(v) => {
        parcel.write(&1i32)?;
        let __field_ref = v.as_ref().ok_or(binder::StatusCode::UNEXPECTED_NULL)?;
        parcel.write(__field_ref)
      }
    }
  }
  fn read_from_parcel(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    let tag: i32 = parcel.read()?;
    match tag {
      0 => {
        let value: i32 = parcel.read()?;
        *self = Self::Num(value);
        Ok(())
      }
      1 => {
        let value: Option<binder::parcel::ParcelFileDescriptor> = Some(parcel.read()?);
        *self = Self::Pfd(value);
        Ok(())
      }
      _ => {
        Err(binder::StatusCode::BAD_VALUE)
      }
    }
  }
}
binder::impl_serialize_for_parcelable!(UnionWithFd);
binder::impl_deserialize_for_parcelable!(UnionWithFd);
impl binder::parcel::ParcelableMetadata for UnionWithFd {
  fn get_descriptor() -> &'static str { "android.aidl.tests.UnionWithFd" }
}
pub(crate) mod mangled {
 pub use super::UnionWithFd as _7_android_4_aidl_5_tests_11_UnionWithFd;
}
