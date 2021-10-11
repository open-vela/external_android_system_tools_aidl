#![forbid(unsafe_code)]
#[derive(Debug, Clone, PartialEq)]
pub enum UnionInUnion {
  First(crate::mangled::_7_android_4_aidl_5_tests_6_unions_9_EnumUnion),
  Second(i32),
}
pub(crate) mod mangled { pub use super::UnionInUnion as _7_android_4_aidl_5_tests_6_unions_12_UnionInUnion; }
impl Default for UnionInUnion {
  fn default() -> Self {
    Self::First(Default::default())
  }
}
impl binder::parcel::Serialize for UnionInUnion {
  fn serialize(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    <Self as binder::parcel::SerializeOption>::serialize_option(Some(self), parcel)
  }
}
impl binder::parcel::SerializeArray for UnionInUnion {}
impl binder::parcel::SerializeOption for UnionInUnion {
  fn serialize_option(this: Option<&Self>, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    let this = if let Some(this) = this {
      parcel.write(&1i32)?;
      this
    } else {
      return parcel.write(&0i32);
    };
    match this {
      Self::First(v) => {
        parcel.write(&0i32)?;
        parcel.write(v)
      }
      Self::Second(v) => {
        parcel.write(&1i32)?;
        parcel.write(v)
      }
    }
  }
}
binder::impl_deserialize_for_parcelable!(UnionInUnion);
impl UnionInUnion {
  fn deserialize_parcelable(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    let tag: i32 = parcel.read()?;
    match tag {
      0 => {
        let value: crate::mangled::_7_android_4_aidl_5_tests_6_unions_9_EnumUnion = parcel.read()?;
        *self = Self::First(value);
        Ok(())
      }
      1 => {
        let value: i32 = parcel.read()?;
        *self = Self::Second(value);
        Ok(())
      }
      _ => {
        Err(binder::StatusCode::BAD_VALUE)
      }
    }
  }
}
