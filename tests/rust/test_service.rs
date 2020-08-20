/*
 * Copyright (C) 2020, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//! Test Rust service for the AIDL compiler.

use aidl_test_interface::aidl::android::aidl::tests::ITestService::{
    self, BnTestService, BpTestService,
};
use aidl_test_interface::aidl::android::aidl::tests::{
    ByteEnum::ByteEnum, ConstantExpressionEnum::ConstantExpressionEnum, INamedCallback, INewName,
    IOldName, IntEnum::IntEnum, LongEnum::LongEnum, StructuredParcelable,
};
use aidl_test_interface::binder::{self, Interface, ParcelFileDescriptor, SpIBinder};
use aidl_test_versioned_interface_V1::aidl::android::aidl::versioned::tests::IFooInterface::{
    self, BnFooInterface, BpFooInterface,
};
use std::collections::HashMap;
use std::sync::Mutex;

fn dup_fd(fd: &ParcelFileDescriptor) -> ParcelFileDescriptor {
    ParcelFileDescriptor::new(fd.as_ref().try_clone().unwrap())
}

struct NamedCallback(String);

impl Interface for NamedCallback {}

impl INamedCallback::INamedCallback for NamedCallback {
    fn GetName(&self) -> binder::Result<String> {
        Ok(self.0.clone())
    }
}

struct OldName;

impl Interface for OldName {}

impl IOldName::IOldName for OldName {
    fn RealName(&self) -> binder::Result<String> {
        Ok("OldName".into())
    }
}

#[derive(Debug, Default)]
struct NewName;

impl Interface for NewName {}

impl INewName::INewName for NewName {
    fn RealName(&self) -> binder::Result<String> {
        Ok("NewName".into())
    }
}

#[derive(Default)]
struct TestService {
    service_map: Mutex<HashMap<String, Box<dyn INamedCallback::INamedCallback>>>,
}

impl Interface for TestService {}

macro_rules! impl_repeat {
    ($repeat_name:ident, $type:ty) => {
        fn $repeat_name(&self, token: $type) -> binder::Result<$type> {
            Ok(token)
        }
    };
}

macro_rules! impl_reverse {
    ($reverse_name:ident, $type:ty) => {
        fn $reverse_name(
            &self,
            input: &[$type],
            repeated: &mut Vec<$type>,
        ) -> binder::Result<Vec<$type>> {
            repeated.clear();
            repeated.extend_from_slice(input);
            Ok(input.iter().rev().cloned().collect())
        }
    };
}

macro_rules! impl_repeat_reverse {
    ($repeat_name:ident, $reverse_name:ident, $type:ty) => {
        impl_repeat! {$repeat_name, $type}
        impl_reverse! {$reverse_name, $type}
    };
}

macro_rules! impl_repeat_nullable {
    ($repeat_nullable_name:ident, $type:ty) => {
        fn $repeat_nullable_name(
            &self,
            input: Option<&[$type]>,
        ) -> binder::Result<Option<Vec<$type>>> {
            Ok(input.map(<[$type]>::to_vec))
        }
    };
}

impl ITestService::ITestService for TestService {
    impl_repeat! {RepeatByte, i8}
    impl_reverse! {ReverseByte, u8}
    impl_repeat_reverse! {RepeatBoolean, ReverseBoolean, bool}
    impl_repeat_reverse! {RepeatChar, ReverseChar, u16}
    impl_repeat_reverse! {RepeatInt, ReverseInt, i32}
    impl_repeat_reverse! {RepeatLong, ReverseLong, i64}
    impl_repeat_reverse! {RepeatFloat, ReverseFloat, f32}
    impl_repeat_reverse! {RepeatDouble, ReverseDouble, f64}
    impl_repeat_reverse! {RepeatByteEnum, ReverseByteEnum, ByteEnum}
    impl_repeat_reverse! {RepeatIntEnum, ReverseIntEnum, IntEnum}
    impl_repeat_reverse! {RepeatLongEnum, ReverseLongEnum, LongEnum}
    impl_reverse! {ReverseString, String}
    impl_reverse! {ReverseStringList, String}
    impl_reverse! {ReverseUtf8CppString, String}

    fn RepeatString(&self, input: &str) -> binder::Result<String> {
        Ok(input.into())
    }

    fn RepeatUtf8CppString(&self, input: &str) -> binder::Result<String> {
        Ok(input.into())
    }

    fn GetOtherTestService(
        &self,
        name: &str,
    ) -> binder::Result<Box<dyn INamedCallback::INamedCallback>> {
        let mut service_map = self.service_map.lock().unwrap();
        let other_service = service_map.entry(name.into()).or_insert_with(|| {
            let named_callback = NamedCallback(name.into());
            Box::new(INamedCallback::BnNamedCallback::new_binder(named_callback))
        });
        Ok(other_service.to_owned())
    }

    fn VerifyName(
        &self,
        service: &dyn INamedCallback::INamedCallback,
        name: &str,
    ) -> binder::Result<bool> {
        service.GetName().map(|found_name| found_name == name)
    }

    fn RepeatParcelFileDescriptor(
        &self,
        read: &ParcelFileDescriptor,
    ) -> binder::Result<ParcelFileDescriptor> {
        Ok(dup_fd(read))
    }

    fn ReverseParcelFileDescriptorArray(
        &self,
        input: &[ParcelFileDescriptor],
        repeated: &mut Vec<Option<ParcelFileDescriptor>>,
    ) -> binder::Result<Vec<ParcelFileDescriptor>> {
        repeated.clear();
        repeated.extend(input.iter().map(dup_fd).map(Some));
        Ok(input.iter().rev().map(dup_fd).collect())
    }

    fn ThrowServiceException(&self, code: i32) -> binder::Result<()> {
        Err(binder::Status::new_service_specific_error(code, None))
    }

    impl_repeat_nullable! {RepeatNullableIntArray, i32}
    impl_repeat_nullable! {RepeatNullableByteEnumArray, ByteEnum}
    impl_repeat_nullable! {RepeatNullableIntEnumArray, IntEnum}
    impl_repeat_nullable! {RepeatNullableLongEnumArray, LongEnum}
    impl_repeat_nullable! {RepeatNullableStringList, Option<String>}

    fn RepeatNullableString(&self, input: Option<&str>) -> binder::Result<Option<String>> {
        Ok(input.map(String::from))
    }

    fn RepeatNullableUtf8CppString(&self, input: Option<&str>) -> binder::Result<Option<String>> {
        Ok(input.map(String::from))
    }

    fn TakesAnIBinder(&self, _: &SpIBinder) -> binder::Result<()> {
        Ok(())
    }

    fn TakesANullableIBinder(&self, _: Option<&SpIBinder>) -> binder::Result<()> {
        Ok(())
    }

    fn ReverseNullableUtf8CppString(
        &self,
        input: Option<&[Option<String>]>,
        repeated: &mut Option<Vec<Option<String>>>,
    ) -> binder::Result<Option<Vec<Option<String>>>> {
        if let Some(input) = input {
            *repeated = Some(input.to_vec());
            Ok(Some(input.iter().rev().cloned().collect()))
        } else {
            // We don't touch `repeated` here, since
            // the C++ test service doesn't either
            Ok(None)
        }
    }

    fn ReverseUtf8CppStringList(
        &self,
        input: Option<&[Option<String>]>,
        repeated: &mut Option<Vec<Option<String>>>,
    ) -> binder::Result<Option<Vec<Option<String>>>> {
        self.ReverseNullableUtf8CppString(input, repeated)
    }

    fn GetCallback(
        &self,
        return_null: bool,
    ) -> binder::Result<Option<Box<dyn INamedCallback::INamedCallback>>> {
        if return_null {
            Ok(None)
        } else {
            self.GetOtherTestService("ABT: always be testing").map(Some)
        }
    }

    fn FillOutStructuredParcelable(
        &self,
        parcelable: &mut StructuredParcelable::StructuredParcelable,
    ) -> binder::Result<()> {
        parcelable.shouldBeJerry = "Jerry".into();
        parcelable.shouldContainThreeFs = vec![parcelable.f, parcelable.f, parcelable.f];
        parcelable.shouldBeByteBar = ByteEnum::BAR;
        parcelable.shouldBeIntBar = IntEnum::BAR;
        parcelable.shouldBeLongBar = LongEnum::BAR;
        parcelable.shouldContainTwoByteFoos = vec![ByteEnum::FOO, ByteEnum::FOO];
        parcelable.shouldContainTwoIntFoos = vec![IntEnum::FOO, IntEnum::FOO];
        parcelable.shouldContainTwoLongFoos = vec![LongEnum::FOO, LongEnum::FOO];

        parcelable.const_exprs_1 = ConstantExpressionEnum::decInt32_1;
        parcelable.const_exprs_2 = ConstantExpressionEnum::decInt32_2;
        parcelable.const_exprs_3 = ConstantExpressionEnum::decInt64_1;
        parcelable.const_exprs_4 = ConstantExpressionEnum::decInt64_2;
        parcelable.const_exprs_5 = ConstantExpressionEnum::decInt64_3;
        parcelable.const_exprs_6 = ConstantExpressionEnum::decInt64_4;
        parcelable.const_exprs_7 = ConstantExpressionEnum::hexInt32_1;
        parcelable.const_exprs_8 = ConstantExpressionEnum::hexInt32_2;
        parcelable.const_exprs_9 = ConstantExpressionEnum::hexInt32_3;
        parcelable.const_exprs_10 = ConstantExpressionEnum::hexInt64_1;

        Ok(())
    }

    fn GetOldNameInterface(&self) -> binder::Result<Box<dyn IOldName::IOldName>> {
        Ok(Box::new(IOldName::BnOldName::new_binder(OldName)))
    }

    fn GetNewNameInterface(&self) -> binder::Result<Box<dyn INewName::INewName>> {
        Ok(Box::new(INewName::BnNewName::new_binder(NewName)))
    }

    fn GetCppJavaTests(&self) -> binder::Result<Option<SpIBinder>> {
        Ok(None)
    }
}

struct FooInterface;

impl Interface for FooInterface {}

impl IFooInterface::IFooInterface for FooInterface {
    fn foo(&self) -> binder::Result<()> {
        Ok(())
    }
}

fn main() {
    binder::ProcessState::set_thread_pool_max_thread_count(0);
    binder::ProcessState::start_thread_pool();

    let service_name = <BpTestService as ITestService::ITestService>::get_descriptor();
    let service = BnTestService::new_binder(TestService::default());
    binder::add_service(service_name, service.as_binder()).expect("Could not register service");

    let versioned_service_name = <BpFooInterface as IFooInterface::IFooInterface>::get_descriptor();
    let versioned_service = BnFooInterface::new_binder(FooInterface);
    binder::add_service(versioned_service_name, versioned_service.as_binder())
        .expect("Could not register service");

    binder::ProcessState::join_thread_pool();
}
