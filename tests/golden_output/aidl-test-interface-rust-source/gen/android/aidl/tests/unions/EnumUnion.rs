#![forbid(unsafe_code)]
#[derive(Debug, Clone, PartialEq)]
pub enum EnumUnion {
  IntEnum(crate::mangled::_7_android_4_aidl_5_tests_7_IntEnum),
  LongEnum(crate::mangled::_7_android_4_aidl_5_tests_8_LongEnum),
}
impl Default for EnumUnion {
  fn default() -> Self {
    Self::IntEnum(crate::mangled::_7_android_4_aidl_5_tests_7_IntEnum::FOO)
  }
}
impl binder::parcel::Parcelable for EnumUnion {
  fn write_to_parcel(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    match self {
      Self::IntEnum(v) => {
        parcel.write(&0i32)?;
        parcel.write(v)
      }
      Self::LongEnum(v) => {
        parcel.write(&1i32)?;
        parcel.write(v)
      }
    }
  }
  fn read_from_parcel(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    let tag: i32 = parcel.read()?;
    match tag {
      0 => {
        let value: crate::mangled::_7_android_4_aidl_5_tests_7_IntEnum = parcel.read()?;
        *self = Self::IntEnum(value);
        Ok(())
      }
      1 => {
        let value: crate::mangled::_7_android_4_aidl_5_tests_8_LongEnum = parcel.read()?;
        *self = Self::LongEnum(value);
        Ok(())
      }
      _ => {
        Err(binder::StatusCode::BAD_VALUE)
      }
    }
  }
}
binder::impl_serialize_for_parcelable!(EnumUnion);
binder::impl_deserialize_for_parcelable!(EnumUnion);
impl binder::parcel::ParcelableMetadata for EnumUnion {
  fn get_descriptor() -> &'static str { "android.aidl.tests.unions.EnumUnion" }
}
pub(crate) mod mangled {
 pub use super::EnumUnion as _7_android_4_aidl_5_tests_6_unions_9_EnumUnion;
}
