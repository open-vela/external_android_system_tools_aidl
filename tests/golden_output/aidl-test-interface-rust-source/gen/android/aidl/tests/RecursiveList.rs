#![forbid(unsafe_code)]
#[derive(Debug)]
pub struct RecursiveList {
  pub value: i32,
  pub next: Option<Box<crate::mangled::_7_android_4_aidl_5_tests_13_RecursiveList>>,
}
pub(crate) mod mangled { pub use super::RecursiveList as _7_android_4_aidl_5_tests_13_RecursiveList; }
impl Default for RecursiveList {
  fn default() -> Self {
    Self {
      value: 0,
      next: Default::default(),
    }
  }
}
impl binder::parcel::Serialize for RecursiveList {
  fn serialize(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    <Self as binder::parcel::SerializeOption>::serialize_option(Some(self), parcel)
  }
}
impl binder::parcel::SerializeArray for RecursiveList {}
impl binder::parcel::SerializeOption for RecursiveList {
  fn serialize_option(this: Option<&Self>, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    let this = if let Some(this) = this {
      parcel.write(&1i32)?;
      this
    } else {
      return parcel.write(&0i32);
    };
    parcel.sized_write(|subparcel| {
      subparcel.write(&this.value)?;
      subparcel.write(&this.next)?;
      Ok(())
    })
  }
}
binder::impl_deserialize_for_parcelable!(RecursiveList);
impl RecursiveList {
  fn deserialize_parcelable(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    parcel.sized_read(|subparcel| {
      if subparcel.has_more_data() {
        self.value = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.next = subparcel.read()?;
      }
      Ok(())
    })
  }
}
