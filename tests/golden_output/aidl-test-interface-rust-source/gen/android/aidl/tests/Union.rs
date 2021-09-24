#![forbid(unsafe_code)]
#[derive(Debug, Clone, PartialEq)]
pub enum Union {
  Ns(Vec<i32>),
  N(i32),
  M(i32),
  S(String),
  Ibinder(Option<binder::SpIBinder>),
  Ss(Vec<String>),
  Be(crate::mangled::_7_android_4_aidl_5_tests_8_ByteEnum),
}
pub const S1: &str = "a string constant in union";
pub(crate) mod mangled { pub use super::Union as _7_android_4_aidl_5_tests_5_Union; }
impl Default for Union {
  fn default() -> Self {
    Self::Ns(vec!{})
  }
}
impl binder::parcel::Serialize for Union {
  fn serialize(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    <Self as binder::parcel::SerializeOption>::serialize_option(Some(self), parcel)
  }
}
impl binder::parcel::SerializeArray for Union {}
impl binder::parcel::SerializeOption for Union {
  fn serialize_option(this: Option<&Self>, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    let this = if let Some(this) = this {
      parcel.write(&1i32)?;
      this
    } else {
      return parcel.write(&0i32);
    };
    match this {
      Self::Ns(v) => {
        parcel.write(&0i32)?;
        parcel.write(v)
      }
      Self::N(v) => {
        parcel.write(&1i32)?;
        parcel.write(v)
      }
      Self::M(v) => {
        parcel.write(&2i32)?;
        parcel.write(v)
      }
      Self::S(v) => {
        parcel.write(&3i32)?;
        parcel.write(v)
      }
      Self::Ibinder(v) => {
        parcel.write(&4i32)?;
        parcel.write(v)
      }
      Self::Ss(v) => {
        parcel.write(&5i32)?;
        parcel.write(v)
      }
      Self::Be(v) => {
        parcel.write(&6i32)?;
        parcel.write(v)
      }
    }
  }
}
binder::impl_deserialize_for_parcelable!(Union);
impl Union {
  fn deserialize_parcelable(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    let tag: i32 = parcel.read()?;
    match tag {
      0 => {
        let value: Vec<i32> = parcel.read()?;
        *self = Self::Ns(value);
        Ok(())
      }
      1 => {
        let value: i32 = parcel.read()?;
        *self = Self::N(value);
        Ok(())
      }
      2 => {
        let value: i32 = parcel.read()?;
        *self = Self::M(value);
        Ok(())
      }
      3 => {
        let value: String = parcel.read()?;
        *self = Self::S(value);
        Ok(())
      }
      4 => {
        let value: Option<binder::SpIBinder> = parcel.read()?;
        *self = Self::Ibinder(value);
        Ok(())
      }
      5 => {
        let value: Vec<String> = parcel.read()?;
        *self = Self::Ss(value);
        Ok(())
      }
      6 => {
        let value: crate::mangled::_7_android_4_aidl_5_tests_8_ByteEnum = parcel.read()?;
        *self = Self::Be(value);
        Ok(())
      }
      _ => {
        Err(binder::StatusCode::BAD_VALUE)
      }
    }
  }
}
