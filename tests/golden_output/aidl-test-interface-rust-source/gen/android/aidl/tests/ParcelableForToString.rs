#[derive(Debug)]
pub struct ParcelableForToString {
  pub intValue: i32,
  pub intArray: Vec<i32>,
  pub longValue: i64,
  pub longArray: Vec<i64>,
  pub doubleValue: f64,
  pub doubleArray: Vec<f64>,
  pub floatValue: f32,
  pub floatArray: Vec<f32>,
  pub byteValue: i8,
  pub byteArray: Vec<u8>,
  pub booleanValue: bool,
  pub booleanArray: Vec<bool>,
  pub stringValue: String,
  pub stringArray: Vec<String>,
  pub stringList: Vec<String>,
  pub parcelableValue: crate::mangled::_7_android_4_aidl_5_tests_26_OtherParcelableForToString,
  pub parcelableArray: Vec<crate::mangled::_7_android_4_aidl_5_tests_26_OtherParcelableForToString>,
  pub enumValue: crate::mangled::_7_android_4_aidl_5_tests_7_IntEnum,
  pub enumArray: Vec<crate::mangled::_7_android_4_aidl_5_tests_7_IntEnum>,
  pub nullArray: Vec<String>,
  pub nullList: Vec<String>,
  pub parcelableGeneric: i32,
  pub unionValue: crate::mangled::_7_android_4_aidl_5_tests_5_Union,
}
pub(crate) mod mangled { pub use super::ParcelableForToString as _7_android_4_aidl_5_tests_21_ParcelableForToString; }
impl Default for ParcelableForToString {
  fn default() -> Self {
    Self {
      intValue: 0,
      intArray: Default::default(),
      longValue: 0,
      longArray: Default::default(),
      doubleValue: 0.000000f64,
      doubleArray: Default::default(),
      floatValue: 0.000000f32,
      floatArray: Default::default(),
      byteValue: 0,
      byteArray: Default::default(),
      booleanValue: false,
      booleanArray: Default::default(),
      stringValue: Default::default(),
      stringArray: Default::default(),
      stringList: Default::default(),
      parcelableValue: Default::default(),
      parcelableArray: Default::default(),
      enumValue: crate::mangled::_7_android_4_aidl_5_tests_7_IntEnum::FOO,
      enumArray: Default::default(),
      nullArray: Default::default(),
      nullList: Default::default(),
      parcelableGeneric: Default::default(),
      unionValue: Default::default(),
    }
  }
}
impl binder::parcel::Serialize for ParcelableForToString {
  fn serialize(&self, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    <Self as binder::parcel::SerializeOption>::serialize_option(Some(self), parcel)
  }
}
impl binder::parcel::SerializeArray for ParcelableForToString {}
impl binder::parcel::SerializeOption for ParcelableForToString {
  fn serialize_option(this: Option<&Self>, parcel: &mut binder::parcel::Parcel) -> binder::Result<()> {
    let this = if let Some(this) = this {
      parcel.write(&1i32)?;
      this
    } else {
      return parcel.write(&0i32);
    };
    parcel.sized_write(|subparcel| {
      subparcel.write(&this.intValue)?;
      subparcel.write(&this.intArray)?;
      subparcel.write(&this.longValue)?;
      subparcel.write(&this.longArray)?;
      subparcel.write(&this.doubleValue)?;
      subparcel.write(&this.doubleArray)?;
      subparcel.write(&this.floatValue)?;
      subparcel.write(&this.floatArray)?;
      subparcel.write(&this.byteValue)?;
      subparcel.write(&this.byteArray)?;
      subparcel.write(&this.booleanValue)?;
      subparcel.write(&this.booleanArray)?;
      subparcel.write(&this.stringValue)?;
      subparcel.write(&this.stringArray)?;
      subparcel.write(&this.stringList)?;
      subparcel.write(&this.parcelableValue)?;
      subparcel.write(&this.parcelableArray)?;
      subparcel.write(&this.enumValue)?;
      subparcel.write(&this.enumArray)?;
      subparcel.write(&this.nullArray)?;
      subparcel.write(&this.nullList)?;
      subparcel.write(&this.parcelableGeneric)?;
      subparcel.write(&this.unionValue)?;
      Ok(())
    })
  }
}
binder::impl_deserialize_for_parcelable!(ParcelableForToString);
impl ParcelableForToString {
  fn deserialize_parcelable(&mut self, parcel: &binder::parcel::Parcel) -> binder::Result<()> {
    parcel.sized_read(|subparcel| {
      if subparcel.has_more_data() {
        self.intValue = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.intArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.longValue = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.longArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.doubleValue = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.doubleArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.floatValue = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.floatArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.byteValue = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.byteArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.booleanValue = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.booleanArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.stringValue = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.stringArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.stringList = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.parcelableValue = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.parcelableArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.enumValue = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.enumArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.nullArray = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.nullList = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.parcelableGeneric = subparcel.read()?;
      }
      if subparcel.has_more_data() {
        self.unionValue = subparcel.read()?;
      }
      Ok(())
    })
  }
}
