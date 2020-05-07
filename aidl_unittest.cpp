/*
 * Copyright (C) 2015, The Android Open Source Project
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

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <android-base/stringprintf.h>
#include <gtest/gtest.h>

#include "aidl.h"
#include "aidl_checkapi.h"
#include "aidl_language.h"
#include "aidl_to_cpp.h"
#include "aidl_to_java.h"
#include "options.h"
#include "tests/fake_io_delegate.h"

using android::aidl::internals::parse_preprocessed_file;
using android::aidl::test::FakeIoDelegate;
using android::base::StringPrintf;
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;
using testing::TestParamInfo;
using testing::internal::CaptureStderr;
using testing::internal::GetCapturedStderr;

namespace android {
namespace aidl {
namespace {

const char kExpectedDepFileContents[] =
R"(place/for/output/p/IFoo.java : \
  p/IFoo.aidl

p/IFoo.aidl :
)";

const char kExpectedNinjaDepFileContents[] =
R"(place/for/output/p/IFoo.java : \
  p/IFoo.aidl
)";

const char kExpectedParcelableDeclarationDepFileContents[] =
    R"( : \
  p/Foo.aidl

p/Foo.aidl :
)";

const char kExpectedStructuredParcelableDepFileContents[] =
    R"(place/for/output/p/Foo.java : \
  p/Foo.aidl

p/Foo.aidl :
)";

const char kExpectedJavaParcelableOutputContests[] =
    R"(/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
@android.annotation.Hide
public class Rect implements android.os.Parcelable
{
  // Comment

  @android.annotation.Hide
  public int x = 5;

  @android.annotation.Hide
  @android.compat.annotation.UnsupportedAppUsage(expectedSignature = "dummy", implicitMember = "dummy", maxTargetSdk = 28, publicAlternatives = "dummy", trackingBug = 42L, overrideSourcePosition="Rect.aidl:7:1:10:14")
  public int y;

  public android.os.ParcelFileDescriptor fd;
  public static final android.os.Parcelable.Creator<Rect> CREATOR = new android.os.Parcelable.Creator<Rect>() {
    @Override
    public Rect createFromParcel(android.os.Parcel _aidl_source) {
      Rect _aidl_out = new Rect();
      _aidl_out.readFromParcel(_aidl_source);
      return _aidl_out;
    }
    @Override
    public Rect[] newArray(int _aidl_size) {
      return new Rect[_aidl_size];
    }
  };
  @Override public final void writeToParcel(android.os.Parcel _aidl_parcel, int _aidl_flag)
  {
    int _aidl_start_pos = _aidl_parcel.dataPosition();
    _aidl_parcel.writeInt(0);
    _aidl_parcel.writeInt(x);
    _aidl_parcel.writeInt(y);
    if ((fd!=null)) {
      _aidl_parcel.writeInt(1);
      fd.writeToParcel(_aidl_parcel, 0);
    }
    else {
      _aidl_parcel.writeInt(0);
    }
    int _aidl_end_pos = _aidl_parcel.dataPosition();
    _aidl_parcel.setDataPosition(_aidl_start_pos);
    _aidl_parcel.writeInt(_aidl_end_pos - _aidl_start_pos);
    _aidl_parcel.setDataPosition(_aidl_end_pos);
  }
  public final void readFromParcel(android.os.Parcel _aidl_parcel)
  {
    int _aidl_start_pos = _aidl_parcel.dataPosition();
    int _aidl_parcelable_size = _aidl_parcel.readInt();
    if (_aidl_parcelable_size < 0) return;
    try {
      x = _aidl_parcel.readInt();
      if (_aidl_parcel.dataPosition() - _aidl_start_pos >= _aidl_parcelable_size) return;
      y = _aidl_parcel.readInt();
      if (_aidl_parcel.dataPosition() - _aidl_start_pos >= _aidl_parcelable_size) return;
      if ((0!=_aidl_parcel.readInt())) {
        fd = android.os.ParcelFileDescriptor.CREATOR.createFromParcel(_aidl_parcel);
      }
      else {
        fd = null;
      }
      if (_aidl_parcel.dataPosition() - _aidl_start_pos >= _aidl_parcelable_size) return;
    } finally {
      _aidl_parcel.setDataPosition(_aidl_start_pos + _aidl_parcelable_size);
    }
  }
  @Override public int describeContents()
  {
    return 0;
  }
}
)";

}  // namespace

class AidlTest : public ::testing::TestWithParam<Options::Language> {
 protected:
  AidlDefinedType* Parse(const string& path, const string& contents, AidlTypenames& typenames_,
                         Options::Language lang, AidlError* error = nullptr,
                         const vector<string> additional_arguments = {}) {
    io_delegate_.SetFileContents(path, contents);
    vector<string> args;
    args.emplace_back("aidl");
    args.emplace_back("--lang=" + Options::LanguageToString(lang));
    for (const string& s : additional_arguments) {
      args.emplace_back(s);
    }
    for (const string& f : preprocessed_files_) {
      args.emplace_back("--preprocessed=" + f);
    }
    for (const string& i : import_paths_) {
      args.emplace_back("--include=" + i);
    }
    args.emplace_back(path);
    Options options = Options::From(args);
    vector<AidlDefinedType*> defined_types;
    vector<string> imported_files;
    ImportResolver import_resolver{io_delegate_, path, import_paths_, {}};
    AidlError actual_error = ::android::aidl::internals::load_and_validate_aidl(
        path, options, io_delegate_, &typenames_, &defined_types, &imported_files);

    if (error != nullptr) {
      *error = actual_error;
    }

    if (actual_error != AidlError::OK) {
      return nullptr;
    }

    EXPECT_EQ(1ul, defined_types.size());

    return defined_types.front();
  }

  Options::Language GetLanguage() { return GetParam(); }

  FakeIoDelegate io_delegate_;
  vector<string> preprocessed_files_;
  set<string> import_paths_;
  AidlTypenames typenames_;
};

// Instantiate the AidlTest parameterized suite, calling all of the TEST_P
// tests with each of the supported languages as a parameter.
INSTANTIATE_TEST_SUITE_P(AidlTestSuite, AidlTest,
                         testing::Values(Options::Language::CPP, Options::Language::JAVA,
                                         Options::Language::NDK),
                         [](const testing::TestParamInfo<Options::Language>& info) {
                           return Options::LanguageToString(info.param);
                         });

TEST_P(AidlTest, AcceptMissingPackage) {
  EXPECT_NE(nullptr, Parse("IFoo.aidl", "interface IFoo { }", typenames_, GetLanguage()));
}

TEST_P(AidlTest, EndsInSingleLineComment) {
  EXPECT_NE(nullptr, Parse("IFoo.aidl", "interface IFoo { } // foo", typenames_, GetLanguage()));
}

TEST_P(AidlTest, RejectsArraysOfBinders) {
  import_paths_.emplace("");
  io_delegate_.SetFileContents("bar/IBar.aidl",
                               "package bar; interface IBar {}");
  const string path = "foo/IFoo.aidl";
  const string contents =
      "package foo;\n"
      "import bar.IBar;\n"
      "interface IFoo { void f(in IBar[] input); }";
  const string expected_stderr = "ERROR: foo/IFoo.aidl:3.27-32: Binder type cannot be an array\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse(path, contents, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, SupportOnlyOutParameters) {
  const string interface_list = "package a; interface IBar { void f(out List<String> bar); }";
  EXPECT_NE(nullptr, Parse("a/IBar.aidl", interface_list, typenames_, GetLanguage()));
  typenames_.Reset();

  const string interface_ibinder = "package a; interface IBaz { void f(out IBinder bar); }";
  const string expected_ibinder_stderr =
      "ERROR: a/IBaz.aidl:1.47-51: 'out IBinder bar' can only be an in parameter.\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IBaz.aidl", interface_ibinder, typenames_, GetLanguage()));
  EXPECT_EQ(expected_ibinder_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, RejectsOnewayOutParameters) {
  const string oneway_interface = "package a; oneway interface IBar { void f(out int bar); }";
  const string expected_stderr =
      "ERROR: a/IBar.aidl:1.40-42: oneway method 'f' cannot have out parameters\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IBar.aidl", oneway_interface, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  typenames_.Reset();

  const string oneway_method = "package a; interface IBar { oneway void f(out int bar); }";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IBar.aidl", oneway_method, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, RejectsOnewayNonVoidReturn) {
  const string oneway_method = "package a; interface IFoo { oneway int f(); }";
  const string expected_stderr =
      "ERROR: a/IFoo.aidl:1.39-41: oneway method 'f' cannot return a value\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", oneway_method, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, RejectsNullablePrimitive) {
  const string oneway_method = "package a; interface IFoo { @nullable int f(); }";
  const string expected_stderr =
      "ERROR: a/IFoo.aidl:1.38-42: Primitive type cannot get nullable annotation\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", oneway_method, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, RejectsDuplicatedArgumentNames) {
  const string method = "package a; interface IFoo { void f(int a, int a); }";
  const string expected_stderr =
      "ERROR: a/IFoo.aidl:1.33-35: method 'f' has duplicate argument name 'a'\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", method, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, RejectsDuplicatedAnnotationParams) {
  const string method = "package a; interface IFoo { @UnsupportedAppUsage(foo=1, foo=2)void f(); }";
  const string expected_stderr = "ERROR: a/IFoo.aidl:1.56-62: Trying to redefine parameter foo.\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", method, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, RejectUnsupportedInterfaceAnnotations) {
  AidlError error;
  const string method = "package a; @nullable interface IFoo { int f(); }";
  const string expected_stderr =
      "ERROR: a/IFoo.aidl:1.21-31: 'nullable' is not a supported annotation for this node. "
      "It must be one of: Hide, UnsupportedAppUsage, VintfStability\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", method, typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::BAD_TYPE, error);
}

TEST_P(AidlTest, RejectUnsupportedTypeAnnotations) {
  AidlError error;
  const string method = "package a; interface IFoo { @JavaOnlyStableParcelable int f(); }";
  const string expected_stderr =
      "ERROR: a/IFoo.aidl:1.54-58: 'JavaOnlyStableParcelable' is not a supported annotation "
      "for this node. It must be one of: Hide, UnsupportedAppUsage, nullable, utf8InCpp\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", method, typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::BAD_TYPE, error);
}

TEST_P(AidlTest, RejectUnsupportedParcelableAnnotations) {
  AidlError error;
  const string method = "package a; @nullable parcelable IFoo cpp_header \"IFoo.h\";";
  const string expected_stderr =
      "ERROR: a/Foo.aidl:1.32-37: 'nullable' is not a supported annotation for this node. "
      "It must be one of: Hide, JavaOnlyStableParcelable, UnsupportedAppUsage, VintfStability\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/Foo.aidl", method, typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::BAD_TYPE, error);
}

TEST_P(AidlTest, RejectUnsupportedParcelableDefineAnnotations) {
  AidlError error;
  const string method = "package a; @nullable parcelable Foo { String a; String b; }";
  const string expected_stderr =
      "ERROR: a/Foo.aidl:1.32-36: 'nullable' is not a supported annotation for this node. "
      "It must be one of: Hide, UnsupportedAppUsage, VintfStability\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/Foo.aidl", method, typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::BAD_TYPE, error);
}

TEST_P(AidlTest, ParsesNullableAnnotation) {
  for (auto is_nullable: {true, false}) {
    auto parse_result = Parse("a/IFoo.aidl",
                              StringPrintf("package a; interface IFoo {%s String f(); }",
                                           (is_nullable) ? "@nullable" : ""),
                              typenames_, GetLanguage());
    ASSERT_NE(nullptr, parse_result);
    const AidlInterface* interface = parse_result->AsInterface();
    ASSERT_NE(nullptr, interface);
    ASSERT_FALSE(interface->GetMethods().empty());
    EXPECT_EQ(interface->GetMethods()[0]->GetType().IsNullable(), is_nullable);
    typenames_.Reset();
  }
}

TEST_P(AidlTest, ParsesUtf8Annotations) {
  for (auto is_utf8: {true, false}) {
    auto parse_result = Parse(
        "a/IFoo.aidl",
        StringPrintf("package a; interface IFoo {%s String f(); }", (is_utf8) ? "@utf8InCpp" : ""),
        typenames_, GetLanguage());
    ASSERT_NE(nullptr, parse_result);
    const AidlInterface* interface = parse_result->AsInterface();
    ASSERT_NE(nullptr, interface);
    ASSERT_FALSE(interface->GetMethods().empty());
    EXPECT_EQ(interface->GetMethods()[0]->GetType().IsUtf8InCpp(), is_utf8);
    typenames_.Reset();
  }
}

TEST_P(AidlTest, VintfRequiresStructuredAndStability) {
  AidlError error;
  const string expected_stderr =
      "ERROR: IFoo.aidl:1.16-26: Must compile @VintfStability type w/ aidl_interface 'stability: "
      "\"vintf\"'\n"
      "ERROR: IFoo.aidl:1.16-26: Must compile @VintfStability type w/ aidl_interface "
      "--structured\n";
  CaptureStderr();
  ASSERT_EQ(nullptr, Parse("IFoo.aidl", "@VintfStability interface IFoo {}", typenames_,
                           GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  ASSERT_EQ(AidlError::NOT_STRUCTURED, error);
}

TEST_P(AidlTest, VintfRequiresStructured) {
  AidlError error;
  const string expected_stderr =
      "ERROR: IFoo.aidl:1.16-26: Must compile @VintfStability type w/ aidl_interface "
      "--structured\n";
  CaptureStderr();
  ASSERT_EQ(nullptr, Parse("IFoo.aidl", "@VintfStability interface IFoo {}", typenames_,
                           GetLanguage(), &error, {"--stability", "vintf"}));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  ASSERT_EQ(AidlError::NOT_STRUCTURED, error);
}

TEST_P(AidlTest, VintfRequiresSpecifiedStability) {
  AidlError error;
  const string expected_stderr =
      "ERROR: IFoo.aidl:1.16-26: Must compile @VintfStability type w/ aidl_interface 'stability: "
      "\"vintf\"'\n";
  CaptureStderr();
  ASSERT_EQ(nullptr, Parse("IFoo.aidl", "@VintfStability interface IFoo {}", typenames_,
                           GetLanguage(), &error, {"--structured"}));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  ASSERT_EQ(AidlError::NOT_STRUCTURED, error);
}

TEST_P(AidlTest, ParsesStabilityAnnotations) {
  AidlError error;
  auto parse_result = Parse("IFoo.aidl", "@VintfStability interface IFoo {}", typenames_,
                            GetLanguage(), &error, {"--structured", "--stability", "vintf"});
  ASSERT_EQ(AidlError::OK, error);
  ASSERT_NE(nullptr, parse_result);
  const AidlInterface* interface = parse_result->AsInterface();
  ASSERT_NE(nullptr, interface);
  ASSERT_TRUE(interface->IsVintfStability());
  typenames_.Reset();
}

TEST_F(AidlTest, ParsesJavaOnlyStableParcelable) {
  Options java_options = Options::From("aidl -o out --structured a/Foo.aidl");
  Options cpp_options = Options::From("aidl --lang=cpp -o out -h out/include a/Foo.aidl");
  Options cpp_structured_options =
      Options::From("aidl --lang=cpp --structured -o out -h out/include a/Foo.aidl");
  io_delegate_.SetFileContents(
      "a/Foo.aidl",
      StringPrintf("package a; @JavaOnlyStableParcelable parcelable Foo cpp_header \"Foo.h\" ;"));

  EXPECT_EQ(0, ::android::aidl::compile_aidl(java_options, io_delegate_));
  EXPECT_EQ(0, ::android::aidl::compile_aidl(cpp_options, io_delegate_));
  const string expected_stderr =
      "ERROR: a/Foo.aidl:1.48-52: Cannot declared parcelable in a --structured interface. "
      "Parcelable must be defined in AIDL directly.\n";
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(cpp_structured_options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, AcceptsOneway) {
  const string oneway_method = "package a; interface IFoo { oneway void f(int a); }";
  const string oneway_interface = "package a; oneway interface IBar { void f(int a); }";
  EXPECT_NE(nullptr, Parse("a/IFoo.aidl", oneway_method, typenames_, GetLanguage()));
  typenames_.Reset();
  EXPECT_NE(nullptr, Parse("a/IBar.aidl", oneway_interface, typenames_, GetLanguage()));
}

TEST_P(AidlTest, AcceptsAnnotatedOnewayMethod) {
  const string oneway_method =
      "package a; interface IFoo { @UnsupportedAppUsage oneway void f(int a); }";
  EXPECT_NE(nullptr, Parse("a/IFoo.aidl", oneway_method, typenames_, GetLanguage()));
}

TEST_P(AidlTest, AnnotationsInMultiplePlaces) {
  const string oneway_method =
      "package a; interface IFoo { @UnsupportedAppUsage oneway @Hide void f(int a); }";
  const AidlDefinedType* defined = Parse("a/IFoo.aidl", oneway_method, typenames_, GetLanguage());
  ASSERT_NE(nullptr, defined);
  const AidlInterface* iface = defined->AsInterface();
  ASSERT_NE(nullptr, iface);

  const auto& methods = iface->GetMethods();
  ASSERT_EQ(1u, methods.size());
  const auto& method = methods[0];
  const AidlTypeSpecifier& ret_type = method->GetType();

  // TODO(b/151102494): these annotations should be on the method
  ASSERT_NE(nullptr, ret_type.UnsupportedAppUsage());
  ASSERT_TRUE(ret_type.IsHide());
}

TEST_P(AidlTest, WritesComments) {
  string foo_interface =
      "package a; /* foo */ interface IFoo {"
      "  /* i */ int i();"
      "  /* j */ @nullable String j();"
      "  /* k */ @UnsupportedAppUsage oneway void k(int a); }";

  auto parse_result = Parse("a/IFoo.aidl", foo_interface, typenames_, GetLanguage());
  EXPECT_NE(nullptr, parse_result);
  EXPECT_EQ("/* foo */", parse_result->GetComments());

  const AidlInterface* interface = parse_result->AsInterface();
  EXPECT_EQ("/* i */", interface->GetMethods()[0]->GetComments());
  EXPECT_EQ("/* j */", interface->GetMethods()[1]->GetComments());
  EXPECT_EQ("/* k */", interface->GetMethods()[2]->GetComments());
}

TEST_F(AidlTest, ParsesPreprocessedFile) {
  string simple_content = "parcelable a.Foo;\ninterface b.IBar;";
  io_delegate_.SetFileContents("path", simple_content);
  EXPECT_FALSE(typenames_.ResolveTypename("a.Foo").is_resolved);
  EXPECT_TRUE(parse_preprocessed_file(io_delegate_, "path", &typenames_));
  EXPECT_TRUE(typenames_.ResolveTypename("a.Foo").is_resolved);
  EXPECT_TRUE(typenames_.ResolveTypename("b.IBar").is_resolved);
}

TEST_F(AidlTest, ParsesPreprocessedFileWithWhitespace) {
  string simple_content = "parcelable    a.Foo;\n  interface b.IBar  ;\t";
  io_delegate_.SetFileContents("path", simple_content);

  EXPECT_FALSE(typenames_.ResolveTypename("a.Foo").is_resolved);
  EXPECT_TRUE(parse_preprocessed_file(io_delegate_, "path", &typenames_));
  EXPECT_TRUE(typenames_.ResolveTypename("a.Foo").is_resolved);
  EXPECT_TRUE(typenames_.ResolveTypename("b.IBar").is_resolved);
}

TEST_P(AidlTest, PreferImportToPreprocessed) {
  io_delegate_.SetFileContents("preprocessed", "interface another.IBar;");
  io_delegate_.SetFileContents("one/IBar.aidl", "package one; "
                                                "interface IBar {}");
  preprocessed_files_.push_back("preprocessed");
  import_paths_.emplace("");
  auto parse_result = Parse("p/IFoo.aidl", "package p; import one.IBar; interface IFoo {}",
                            typenames_, GetLanguage());
  EXPECT_NE(nullptr, parse_result);

  // We expect to know about both kinds of IBar
  EXPECT_TRUE(typenames_.ResolveTypename("one.IBar").is_resolved);
  EXPECT_TRUE(typenames_.ResolveTypename("another.IBar").is_resolved);
  // But if we request just "IBar" we should get our imported one.
  AidlTypeSpecifier ambiguous_type(AIDL_LOCATION_HERE, "IBar", false, nullptr, "");
  ambiguous_type.Resolve(typenames_);
  EXPECT_EQ("one.IBar", ambiguous_type.GetName());
}

// Special case of PreferImportToPreprocessed. Imported type should be preferred
// even when the preprocessed file already has the same type.
TEST_P(AidlTest, B147918827) {
  io_delegate_.SetFileContents("preprocessed", "interface another.IBar;\ninterface one.IBar;");
  io_delegate_.SetFileContents("one/IBar.aidl",
                               "package one; "
                               "interface IBar {}");
  preprocessed_files_.push_back("preprocessed");
  import_paths_.emplace("");
  auto parse_result = Parse("p/IFoo.aidl", "package p; import one.IBar; interface IFoo {}",
                            typenames_, GetLanguage());
  EXPECT_NE(nullptr, parse_result);

  // We expect to know about both kinds of IBar
  EXPECT_TRUE(typenames_.ResolveTypename("one.IBar").is_resolved);
  EXPECT_TRUE(typenames_.ResolveTypename("another.IBar").is_resolved);
  // But if we request just "IBar" we should get our imported one.
  AidlTypeSpecifier ambiguous_type(AIDL_LOCATION_HERE, "IBar", false, nullptr, "");
  ambiguous_type.Resolve(typenames_);
  EXPECT_EQ("one.IBar", ambiguous_type.GetName());
}

TEST_F(AidlTest, WritePreprocessedFile) {
  io_delegate_.SetFileContents("p/Outer.aidl",
                               "package p; parcelable Outer.Inner;");
  io_delegate_.SetFileContents("one/IBar.aidl", "package one; import p.Outer;"
                                                "interface IBar {}");

  vector<string> args {
    "aidl",
    "--preprocess",
    "preprocessed",
    "p/Outer.aidl",
    "one/IBar.aidl"};
  Options options = Options::From(args);
  EXPECT_TRUE(::android::aidl::preprocess_aidl(options, io_delegate_));

  string output;
  EXPECT_TRUE(io_delegate_.GetWrittenContents("preprocessed", &output));
  EXPECT_EQ("parcelable p.Outer.Inner;\ninterface one.IBar;\n", output);
}

TEST_F(AidlTest, JavaParcelableOutput) {
  io_delegate_.SetFileContents(
      "Rect.aidl",
      "@Hide\n"
      "parcelable Rect {\n"
      "  // Comment\n"
      "  @Hide\n"
      "  int x=5;\n"
      "  @Hide\n"
      "  @UnsupportedAppUsage(maxTargetSdk = 28, trackingBug = 42, implicitMember = \"dummy\", "
      "expectedSignature = \"dummy\", publicAlternatives = \"d\" \n + \"u\" + \n \"m\" \n + \"m\" "
      "+ \"y\")\n"
      "  int y;\n"
      "  ParcelFileDescriptor fd;\n"
      "}");

  vector<string> args{"aidl", "Rect.aidl"};
  Options options = Options::From(args);
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  string output;
  EXPECT_TRUE(io_delegate_.GetWrittenContents("Rect.java", &output));
  EXPECT_EQ(kExpectedJavaParcelableOutputContests, output);
}

TEST_P(AidlTest, RequireOuterClass) {
  const string expected_stderr = "ERROR: p/IFoo.aidl:1.54-60: Failed to resolve 'Inner'\n";
  io_delegate_.SetFileContents("p/Outer.aidl",
                               "package p; parcelable Outer.Inner;");
  import_paths_.emplace("");
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl",
                           "package p; import p.Outer; interface IFoo { void f(in Inner c); }",
                           typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, ParseCompoundParcelableFromPreprocess) {
  io_delegate_.SetFileContents("preprocessed",
                               "parcelable p.Outer.Inner;");
  preprocessed_files_.push_back("preprocessed");
  auto parse_result = Parse("p/IFoo.aidl", "package p; interface IFoo { void f(in Inner c); }",
                            typenames_, GetLanguage());
  // TODO(wiley): This should actually return nullptr because we require
  //              the outer class name.  However, for legacy reasons,
  //              this behavior must be maintained.  b/17415692
  EXPECT_NE(nullptr, parse_result);
}

TEST_F(AidlTest, FailOnParcelable) {
  const string expected_foo_stderr =
      "ERROR: p/IFoo.aidl:1.22-27: Refusing to generate code with unstructured parcelables. "
      "Declared parcelables should be in their own file and/or cannot be used with --structured "
      "interfaces.\n";
  io_delegate_.SetFileContents("p/IFoo.aidl", "package p; parcelable IFoo;");

  // By default, we shouldn't fail on parcelable.
  Options options1 = Options::From("aidl p/IFoo.aidl");
  CaptureStderr();
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options1, io_delegate_));
  EXPECT_EQ("", GetCapturedStderr());

  // -b considers this an error
  Options options2 = Options::From("aidl -b p/IFoo.aidl");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options2, io_delegate_));
  EXPECT_EQ(expected_foo_stderr, GetCapturedStderr());

  const string expected_bar_stderr =
      "ERROR: p/IBar.aidl:1.22-26: Refusing to generate code with unstructured parcelables. "
      "Declared parcelables should be in their own file and/or cannot be used with --structured "
      "interfaces.\n";
  io_delegate_.SetFileContents("p/IBar.aidl", "package p; parcelable Foo; interface IBar{}");

  // With '-b' option, a parcelable and an interface should fail.
  Options options3 = Options::From("aidl p/IBar.aidl");
  CaptureStderr();
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options3, io_delegate_));
  EXPECT_EQ("", GetCapturedStderr());
  Options options4 = Options::From("aidl -b p/IBar.aidl");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options4, io_delegate_));
  EXPECT_EQ(expected_bar_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, StructuredFailOnUnstructuredParcelable) {
  const string expected_stderr =
      "ERROR: ./o/WhoKnowsWhat.aidl:1.22-35: o.WhoKnowsWhat is not structured, but this is a "
      "structured interface.\n";
  io_delegate_.SetFileContents("o/WhoKnowsWhat.aidl", "package o; parcelable WhoKnowsWhat;");
  import_paths_.emplace("");
  AidlError error;
  CaptureStderr();
  EXPECT_EQ(
      nullptr,
      Parse("p/IFoo.aidl",
            "package p; import o.WhoKnowsWhat; interface IFoo { void f(in WhoKnowsWhat thisIs); }",
            typenames_, GetLanguage(), &error, {"--structured"}));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::NOT_STRUCTURED, error);
}

TEST_P(AidlTest, FailOnDuplicateConstantNames) {
  AidlError error;
  const string expected_stderr =
      "ERROR: p/IFoo.aidl:4.34-45: Found duplicate constant name 'DUPLICATED'\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl",
                           R"(package p;
                      interface IFoo {
                        const String DUPLICATED = "d";
                        const int DUPLICATED = 1;
                      }
                   )",
                           typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::BAD_TYPE, error);
}

TEST_P(AidlTest, FailOnManyDefinedTypes) {
  AidlError error;
  const string expected_stderr =
      "ERROR: p/IFoo.aidl:3.33-38: You must declare only one type per file.\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl",
                           R"(package p;
                      interface IFoo {}
                      parcelable IBar {}
                      parcelable StructuredParcelable {}
                      interface IBaz {}
                  )",
                           typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  // Parse success is important for clear error handling even if the cases aren't
  // actually supported in code generation.
  EXPECT_EQ(AidlError::BAD_TYPE, error);
}

TEST_P(AidlTest, FailOnNoDefinedTypes) {
  AidlError error;
  const string expected_stderr = "ERROR: p/IFoo.aidl:1.11-11: syntax error, unexpected $end\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl", R"(package p;)", typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::PARSE_ERROR, error);
}

TEST_P(AidlTest, FailOnMalformedConstHexValue) {
  AidlError error;
  const string expected_stderr =
      "ERROR: Could not parse hexvalue: 0xffffffffffffffffff at p/IFoo.aidl:3.50-70.\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl",
                           R"(package p;
                      interface IFoo {
                        const int BAD_HEX_VALUE = 0xffffffffffffffffff;
                      }
                   )",
                           typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::PARSE_ERROR, error);
}

TEST_P(AidlTest, ParsePositiveConstHexValue) {
  AidlError error;
  auto parse_result = Parse("p/IFoo.aidl",
                            R"(package p;
              interface IFoo {
                const int POSITIVE_HEX_VALUE = 0xf5;
              }
           )",
                            typenames_, GetLanguage(), &error);
  EXPECT_NE(nullptr, parse_result);
  const AidlInterface* interface = parse_result->AsInterface();
  ASSERT_NE(nullptr, interface);
  const auto& cpp_constants = interface->GetConstantDeclarations();
  EXPECT_EQ((size_t)1, cpp_constants.size());
  EXPECT_EQ("POSITIVE_HEX_VALUE", cpp_constants[0]->GetName());
  EXPECT_TRUE(cpp_constants[0]->CheckValid(typenames_));
  EXPECT_EQ("245", cpp_constants[0]->ValueString(cpp::ConstantValueDecorator));
}

TEST_P(AidlTest, ParseNegativeConstHexValue) {
  AidlError error;
  auto parse_result = Parse("p/IFoo.aidl",
                            R"(package p;
              interface IFoo {
                const int NEGATIVE_HEX_VALUE = 0xffffffff;
              }
           )",
                            typenames_, GetLanguage(), &error);
  ASSERT_NE(nullptr, parse_result);
  const AidlInterface* interface = parse_result->AsInterface();
  ASSERT_NE(nullptr, interface);
  const auto& cpp_constants = interface->GetConstantDeclarations();
  EXPECT_EQ((size_t)1, cpp_constants.size());
  EXPECT_EQ("NEGATIVE_HEX_VALUE", cpp_constants[0]->GetName());
  EXPECT_EQ(true, cpp_constants[0]->CheckValid(typenames_));
  EXPECT_EQ("-1", cpp_constants[0]->ValueString(cpp::ConstantValueDecorator));
}

TEST_P(AidlTest, UnderstandsNestedParcelables) {
  io_delegate_.SetFileContents(
      "p/Outer.aidl",
      "package p; parcelable Outer.Inner cpp_header \"baz/header\";");
  import_paths_.emplace("");
  const string input_path = "p/IFoo.aidl";
  const string input = "package p; import p.Outer; interface IFoo"
                       " { Outer.Inner get(); }";

  auto parse_result = Parse(input_path, input, typenames_, GetLanguage());
  EXPECT_NE(nullptr, parse_result);

  EXPECT_TRUE(typenames_.ResolveTypename("p.Outer.Inner").is_resolved);
  // C++ uses "::" instead of "." to refer to a inner class.
  AidlTypeSpecifier nested_type(AIDL_LOCATION_HERE, "p.Outer.Inner", false, nullptr, "");
  EXPECT_EQ("::p::Outer::Inner", cpp::CppNameOf(nested_type, typenames_));
}

TEST_P(AidlTest, UnderstandsNativeParcelables) {
  io_delegate_.SetFileContents(
      "p/Bar.aidl",
      "package p; parcelable Bar cpp_header \"baz/header\";");
  import_paths_.emplace("");
  const string input_path = "p/IFoo.aidl";
  const string input = "package p; import p.Bar; interface IFoo { }";
  auto parse_result = Parse(input_path, input, typenames_, GetLanguage());
  EXPECT_NE(nullptr, parse_result);
  EXPECT_TRUE(typenames_.ResolveTypename("p.Bar").is_resolved);
  AidlTypeSpecifier native_type(AIDL_LOCATION_HERE, "p.Bar", false, nullptr, "");
  native_type.Resolve(typenames_);

  EXPECT_EQ("p.Bar", java::InstantiableJavaSignatureOf(native_type, typenames_));
  // C++ understands C++ specific stuff
  EXPECT_EQ("::p::Bar", cpp::CppNameOf(native_type, typenames_));
  set<string> headers;
  cpp::AddHeaders(native_type, typenames_, headers);
  EXPECT_EQ(1u, headers.size());
  EXPECT_EQ(1u, headers.count("baz/header"));
}

TEST_F(AidlTest, WritesCorrectDependencyFile) {
  // While the in tree build system always gives us an output file name,
  // other android tools take advantage of our ability to infer the intended
  // file name.  This test makes sure we handle this correctly.
  vector<string> args = {
    "aidl",
    "-d dep/file/path",
    "-o place/for/output",
    "p/IFoo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; interface IFoo {}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string actual_dep_file_contents;
  EXPECT_TRUE(io_delegate_.GetWrittenContents(options.DependencyFile(), &actual_dep_file_contents));
  EXPECT_EQ(actual_dep_file_contents, kExpectedDepFileContents);
}

TEST_F(AidlTest, WritesCorrectDependencyFileNinja) {
  // While the in tree build system always gives us an output file name,
  // other android tools take advantage of our ability to infer the intended
  // file name.  This test makes sure we handle this correctly.
  vector<string> args = {
    "aidl",
    "-d dep/file/path",
    "--ninja",
    "-o place/for/output",
    "p/IFoo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; interface IFoo {}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string actual_dep_file_contents;
  EXPECT_TRUE(io_delegate_.GetWrittenContents(options.DependencyFile(), &actual_dep_file_contents));
  EXPECT_EQ(actual_dep_file_contents, kExpectedNinjaDepFileContents);
}

TEST_F(AidlTest, WritesTrivialDependencyFileForParcelableDeclaration) {
  // The SDK uses aidl to decide whether a .aidl file is a parcelable.  It does
  // this by calling aidl with every .aidl file it finds, then parsing the
  // generated dependency files.  Those that reference .java output files are
  // for interfaces and those that do not are parcelables.  However, for both
  // parcelables and interfaces, we *must* generate a non-empty dependency file.
  vector<string> args = {
    "aidl",
    "-o place/for/output",
    "-d dep/file/path",
    "p/Foo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; parcelable Foo;");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string actual_dep_file_contents;
  EXPECT_TRUE(io_delegate_.GetWrittenContents(options.DependencyFile(), &actual_dep_file_contents));
  EXPECT_EQ(actual_dep_file_contents, kExpectedParcelableDeclarationDepFileContents);
}

TEST_F(AidlTest, WritesDependencyFileForStructuredParcelable) {
  vector<string> args = {
    "aidl",
    "--structured",
    "-o place/for/output",
    "-d dep/file/path",
    "p/Foo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; parcelable Foo {int a;}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string actual_dep_file_contents;
  EXPECT_TRUE(io_delegate_.GetWrittenContents(options.DependencyFile(), &actual_dep_file_contents));
  EXPECT_EQ(actual_dep_file_contents, kExpectedStructuredParcelableDepFileContents);
}

TEST_F(AidlTest, NoJavaOutputForParcelableDeclaration) {
 vector<string> args = {
    "aidl",
    "--lang=java",
    "-o place/for/output",
    "p/Foo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; parcelable Foo;");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string output_file_contents;
  EXPECT_FALSE(io_delegate_.GetWrittenContents(options.OutputFile(), &output_file_contents));
}

// TODO(b/136048684)
TEST_P(AidlTest, PrimitiveList) {
  const string primitive_interface =
      "package a; interface IFoo {\n"
      "  List<int> foo(); }";
  string expected_stderr;
  switch (GetLanguage()) {
    case Options::Language::CPP:
      expected_stderr =
          "ERROR: a/IFoo.aidl:2.1-7: List<int> is not supported. List in cpp supports only "
          "String and IBinder.\n";
      break;
    case Options::Language::JAVA:
      expected_stderr =
          "ERROR: a/IFoo.aidl:2.1-7: List<int> is not supported. List in Java supports only "
          "String, IBinder, and ParcelFileDescriptor.\n";
      break;
    case Options::Language::NDK:
      expected_stderr =
          "ERROR: a/IFoo.aidl:2.1-7: A generic type cannot have any primitive type parameters.\n";
      break;
    default:
      AIDL_FATAL(AIDL_LOCATION_HERE)
          << "Unexpected Options::Language enumerator: " << static_cast<size_t>(GetLanguage());
  }
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", primitive_interface, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  typenames_.Reset();

  string primitive_parcelable =
      "package a; parcelable IFoo {\n"
      "  List<int> foo;}";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", primitive_parcelable, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_P(AidlTest, RejectsPrimitiveListInStableAidl) {
  AidlError error;
  string expected_stderr =
      "ERROR: a/IFoo.aidl:2.7-11: "
      "Encountered an untyped List or Map. The use of untyped List/Map is "
      "prohibited because it is not guaranteed that the objects in the list are recognizable in "
      "the receiving side. Consider switching to an array or a generic List/Map.\n";
  if (GetLanguage() != Options::Language::JAVA) {
    expected_stderr =
        "ERROR: a/IFoo.aidl:2.1-7: "
        "Currently, only the Java backend supports non-generic List.\n";
  }

  const string primitive_interface =
      "package a; interface IFoo {\n"
      "  List foo(); }";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", primitive_interface, typenames_, GetLanguage(), &error,
                           {"--structured"}));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  typenames_.Reset();

  string primitive_parcelable =
      "package a; parcelable IFoo {\n"
      "  List foo;}";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", primitive_parcelable, typenames_, GetLanguage(), &error,
                           {"--structured"}));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, ApiDump) {
  io_delegate_.SetFileContents(
      "foo/bar/IFoo.aidl",
      "package foo.bar;\n"
      "import foo.bar.Data;\n"
      "// comment @hide\n"
      "interface IFoo {\n"
      "    /* @hide */\n"
      "    int foo(out int[] a, String b, boolean c, inout List<String>  d);\n"
      "    int foo2(@utf8InCpp String x, inout List<String>  y);\n"
      "    IFoo foo3(IFoo foo);\n"
      "    Data getData();\n"
      "    // @hide\n"
      "    const int A = 1;\n"
      "    const String STR = \"Hello\";\n"
      "}\n");
  io_delegate_.SetFileContents("foo/bar/Data.aidl",
                               "package foo.bar;\n"
                               "import foo.bar.IFoo;\n"
                               "/* @hide*/\n"
                               "parcelable Data {\n"
                               "   // @hide\n"
                               "   int x = 10;\n"
                               "   // @hide\n"
                               "   int y;\n"
                               "   IFoo foo;\n"
                               "   List<IFoo> a;\n"
                               "   /*@hide2*/\n"
                               "   List<foo.bar.IFoo> b;\n"
                               "   // It should be @hide property\n"
                               "   @nullable String[] c;\n"
                               "}\n");
  io_delegate_.SetFileContents("api.aidl", "");
  vector<string> args = {"aidl", "--dumpapi", "--out=dump", "--include=.",
                         "foo/bar/IFoo.aidl", "foo/bar/Data.aidl"};
  Options options = Options::From(args);
  bool result = dump_api(options, io_delegate_);
  ASSERT_TRUE(result);
  string actual;
  EXPECT_TRUE(io_delegate_.GetWrittenContents("dump/foo/bar/IFoo.aidl", &actual));
  EXPECT_EQ(actual, string(kPreamble).append(R"(package foo.bar;
/* @hide */
interface IFoo {
  /* @hide */
  int foo(out int[] a, String b, boolean c, inout List<String> d);
  int foo2(@utf8InCpp String x, inout List<String> y);
  foo.bar.IFoo foo3(foo.bar.IFoo foo);
  foo.bar.Data getData();
  /* @hide */
  const int A = 1;
  const String STR = "Hello";
}
)"));

  EXPECT_TRUE(io_delegate_.GetWrittenContents("dump/foo/bar/Data.aidl", &actual));
  EXPECT_EQ(actual, string(kPreamble).append(R"(package foo.bar;
/* @hide */
parcelable Data {
  /* @hide */
  int x = 10;
  /* @hide */
  int y;
  foo.bar.IFoo foo;
  List<foo.bar.IFoo> a;
  List<foo.bar.IFoo> b;
  /* @hide */
  @nullable String[] c;
}
)"));
}

TEST_F(AidlTest, ApiDumpWithManualIds) {
  io_delegate_.SetFileContents(
      "foo/bar/IFoo.aidl",
      "package foo.bar;\n"
      "interface IFoo {\n"
      "    int foo() = 1;\n"
      "    int bar() = 2;\n"
      "    int baz() = 10;\n"
      "}\n");

  vector<string> args = {"aidl", "--dumpapi", "-o dump", "foo/bar/IFoo.aidl"};
  Options options = Options::From(args);
  bool result = dump_api(options, io_delegate_);
  ASSERT_TRUE(result);
  string actual;
  EXPECT_TRUE(io_delegate_.GetWrittenContents("dump/foo/bar/IFoo.aidl", &actual));
  EXPECT_EQ(actual, string(kPreamble).append(R"(package foo.bar;
interface IFoo {
  int foo() = 1;
  int bar() = 2;
  int baz() = 10;
}
)"));
}

TEST_F(AidlTest, ApiDumpWithManualIdsOnlyOnSomeMethods) {
  const string expected_stderr =
      "ERROR: foo/bar/IFoo.aidl:4.8-12: You must either assign id's to all methods or to none of "
      "them.\n";
  io_delegate_.SetFileContents(
      "foo/bar/IFoo.aidl",
      "package foo.bar;\n"
      "interface IFoo {\n"
      "    int foo() = 1;\n"
      "    int bar();\n"
      "    int baz() = 10;\n"
      "}\n");

  vector<string> args = {"aidl", "--dumpapi", "-o dump", "foo/bar/IFoo.aidl"};
  Options options = Options::From(args);
  CaptureStderr();
  EXPECT_FALSE(dump_api(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, CheckNumGenericTypeSecifier) {
  const string expected_list_stderr =
      "ERROR: p/IFoo.aidl:1.37-41: List must have only one type parameter.\n";
  const string expected_map_stderr =
      "ERROR: p/IFoo.aidl:1.37-40: Map must have 0 or 2 type parameters, but got 'Map<String>'\n";
  Options options = Options::From("aidl p/IFoo.aidl IFoo.java");
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "void foo(List<String, String> a);}");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_list_stderr, GetCapturedStderr());

  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "void foo(Map<String> a);}");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_map_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, CheckTypeParameterInMapType) {
  const string expected_stderr =
      "ERROR: p/IFoo.aidl:1.28-31: The type of key in map must be String, but it is 'p.Bar'\n";
  Options options = Options::From("aidl -I p p/IFoo.aidl");
  io_delegate_.SetFileContents("p/Bar.aidl", "package p; parcelable Bar { String s; }");

  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo {"
                               "Map<String, Bar> foo();}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo {"
                               "Map<Bar, Bar> foo();}");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());

  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo {"
                               "Map<String, String> foo();}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo {"
                               "Map<String, ParcelFileDescriptor> foo();}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, WrongGenericType) {
  const string expected_stderr = "ERROR: p/IFoo.aidl:1.28-34: String is not a generic type.\n";
  Options options = Options::From("aidl p/IFoo.aidl IFoo.java");
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "String<String> foo(); }");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, UserDefinedUnstructuredGenericParcelableType) {
  Options optionsForParcelable = Options::From("aidl -I p p/Bar.aidl");
  io_delegate_.SetFileContents("p/Bar.aidl", "package p; parcelable Bar<T, T>;");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(optionsForParcelable, io_delegate_));
  EXPECT_EQ("ERROR: p/Bar.aidl:1.22-26: Every type parameter should be unique.\n",
            GetCapturedStderr());

  Options options = Options::From("aidl -I p p/IFoo.aidl");
  io_delegate_.SetFileContents("p/Bar.aidl", "package p; parcelable Bar;");
  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo {"
                               "Bar<String, String> foo();}");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ("ERROR: p/IFoo.aidl:1.28-31: p.Bar is not a generic type.\n", GetCapturedStderr());
  io_delegate_.SetFileContents("p/Bar.aidl", "package p; parcelable Bar<T>;");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ("ERROR: p/IFoo.aidl:1.28-31: p.Bar must have 1 type parameters, but got 2\n",
            GetCapturedStderr());
  io_delegate_.SetFileContents("p/Bar.aidl", "package p; parcelable Bar<T, V>;");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo {"
                               "Bar<String, ParcelFileDescriptor> foo();}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo {"
                               "Bar<int, long> foo();}");

  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo {"
                               "Bar<int[], long[]> foo();}");

  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, FailOnMultipleTypesInSingleFile) {
  std::vector<std::string> rawOptions{"aidl --lang=java -o out foo/bar/Foo.aidl",
                                      "aidl --lang=cpp -o out -h out/include foo/bar/Foo.aidl"};
  for (const auto& rawOption : rawOptions) {
    string expected_stderr =
        "ERROR: foo/bar/Foo.aidl:3.1-10: You must declare only one type per file.\n";
    Options options = Options::From(rawOption);
    io_delegate_.SetFileContents(options.InputFiles().front(),
                                 "package foo.bar;\n"
                                 "interface IFoo1 { int foo(); }\n"
                                 "interface IFoo2 { int foo(); }\n"
                                 "parcelable Data1 { int a; int b;}\n"
                                 "parcelable Data2 { int a; int b;}\n");
    CaptureStderr();
    EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
    EXPECT_EQ(expected_stderr, GetCapturedStderr());

    io_delegate_.SetFileContents(options.InputFiles().front(),
                                 "package foo.bar;\n"
                                 "interface IFoo1 { int foo(); }\n"
                                 "interface IFoo2 { int foo(); }\n");
    CaptureStderr();
    EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
    EXPECT_EQ(expected_stderr, GetCapturedStderr());

    expected_stderr = "ERROR: foo/bar/Foo.aidl:3.11-17: You must declare only one type per file.\n";
    io_delegate_.SetFileContents(options.InputFiles().front(),
                                 "package foo.bar;\n"
                                 "parcelable Data1 { int a; int b;}\n"
                                 "parcelable Data2 { int a; int b;}\n");
    CaptureStderr();
    EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
    EXPECT_EQ(expected_stderr, GetCapturedStderr());
  }
}

TEST_P(AidlTest, FailParseOnEmptyFile) {
  const string contents = "";
  const string expected_stderr = "ERROR: a/IFoo.aidl:1.1-1: syntax error, unexpected $end\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", contents, typenames_, GetLanguage()));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, MultipleInputFiles) {
  Options options = Options::From(
      "aidl --lang=java -o out -I . foo/bar/IFoo.aidl foo/bar/Data.aidl");

  io_delegate_.SetFileContents(options.InputFiles().at(0),
      "package foo.bar;\n"
      "import foo.bar.Data;\n"
      "interface IFoo { Data getData(); }\n");

  io_delegate_.SetFileContents(options.InputFiles().at(1),
        "package foo.bar;\n"
        "import foo.bar.IFoo;\n"
        "parcelable Data { IFoo foo; }\n");

  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  string content;
  for (const auto file : {
    "out/foo/bar/IFoo.java", "out/foo/bar/Data.java"}) {
    content.clear();
    EXPECT_TRUE(io_delegate_.GetWrittenContents(file, &content));
    EXPECT_FALSE(content.empty());
  }
}

TEST_F(AidlTest, MultipleInputFilesCpp) {
  Options options = Options::From("aidl --lang=cpp -o out -h out/include "
      "-I . foo/bar/IFoo.aidl foo/bar/Data.aidl");

  io_delegate_.SetFileContents(options.InputFiles().at(0),
      "package foo.bar;\n"
      "import foo.bar.Data;\n"
      "interface IFoo { Data getData(); }\n");

  io_delegate_.SetFileContents(options.InputFiles().at(1),
        "package foo.bar;\n"
        "import foo.bar.IFoo;\n"
        "parcelable Data { IFoo foo; }\n");

  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  string content;
  for (const auto file : {
    "out/foo/bar/IFoo.cpp", "out/foo/bar/Data.cpp",
    "out/include/foo/bar/IFoo.h", "out/include/foo/bar/Data.h",
    "out/include/foo/bar/BpFoo.h", "out/include/foo/bar/BpData.h",
    "out/include/foo/bar/BnFoo.h", "out/include/foo/bar/BnData.h"}) {
    content.clear();
    EXPECT_TRUE(io_delegate_.GetWrittenContents(file, &content));
    EXPECT_FALSE(content.empty());
  }
}

TEST_F(AidlTest, ConflictWithMetaTransactionGetVersion) {
  const string expected_stderr =
      "ERROR: p/IFoo.aidl:1.31-51:  method getInterfaceVersion() is reserved for internal use.\n";
  Options options = Options::From("aidl --lang=java -o place/for/output p/IFoo.aidl");
  // int getInterfaceVersion() is one of the meta transactions
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "int getInterfaceVersion(); }");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, ConflictWithSimilarMetaTransaction) {
  // boolean getInterfaceVersion() is not a meta transaction, but should be
  // prevented because return type is not part of a method signature
  const string expected_stderr =
      "ERROR: p/IFoo.aidl:1.35-55:  method getInterfaceVersion() is reserved for internal use.\n";
  Options options = Options::From("aidl --lang=java -o place/for/output p/IFoo.aidl");
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "boolean getInterfaceVersion(); }");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, ConflictWithMetaTransactionGetName) {
  // this is another reserved name
  const string expected_stderr =
      "ERROR: p/IFoo.aidl:1.34-53:  method getTransactionName(int) is reserved for internal use.\n";
  Options options = Options::From("aidl --lang=java -o place/for/output p/IFoo.aidl");
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "String getTransactionName(int code); }");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());

  // this is not a meta interface method as it differs type arguments
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "String getTransactionName(); }");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, DifferentOrderAnnotationsInCheckAPI) {
  Options options = Options::From("aidl --checkapi old new");
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p; interface IFoo{ @utf8InCpp @nullable String foo();}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p; interface IFoo{ @nullable @utf8InCpp String foo();}");

  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
}

TEST_F(AidlTest, SuccessOnIdenticalApiDumps) {
  Options options = Options::From("aidl --checkapi old new");
  io_delegate_.SetFileContents("old/p/IFoo.aidl", "package p; interface IFoo{ void foo();}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "package p; interface IFoo{ void foo();}");

  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
}

class AidlTestCompatibleChanges : public AidlTest {
 protected:
  Options options_ = Options::From("aidl --checkapi old new");
};

TEST_F(AidlTestCompatibleChanges, NewType) {
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/p/IBar.aidl",
                               "package p;"
                               "interface IBar {"
                               "  void bar();"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options_, io_delegate_));
}

TEST_F(AidlTestCompatibleChanges, NewMethod) {
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "  void bar();"
                               "  void baz(in List<IFoo> arg);"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options_, io_delegate_));
}

TEST_F(AidlTestCompatibleChanges, NewField) {
  io_delegate_.SetFileContents("old/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar = 0;"
                               "  @nullable List<Data> list;"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options_, io_delegate_));
}

TEST_F(AidlTestCompatibleChanges, NewEnumerator) {
  io_delegate_.SetFileContents("old/p/Enum.aidl",
                               "package p;"
                               "enum Enum {"
                               "  FOO = 1,"
                               "}");
  io_delegate_.SetFileContents("new/p/Enum.aidl",
                               "package p;"
                               "enum Enum {"
                               "  FOO = 1,"
                               "  BAR = 2,"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options_, io_delegate_));
}

TEST_F(AidlTestCompatibleChanges, ReorderedEnumerator) {
  io_delegate_.SetFileContents("old/p/Enum.aidl",
                               "package p;"
                               "enum Enum {"
                               "  FOO = 1,"
                               "  BAR = 2,"
                               "}");
  io_delegate_.SetFileContents("new/p/Enum.aidl",
                               "package p;"
                               "enum Enum {"
                               "  BAR = 2,"
                               "  FOO = 1,"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options_, io_delegate_));
}

TEST_F(AidlTestCompatibleChanges, NewPackage) {
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("old/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  io_delegate_.SetFileContents("new/q/IFoo.aidl",
                               "package q;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/q/Data.aidl",
                               "package q;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options_, io_delegate_));
}

TEST_F(AidlTestCompatibleChanges, ArgNameChange) {
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int b);"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options_, io_delegate_));
}

TEST_F(AidlTestCompatibleChanges, AddedConstValue) {
  io_delegate_.SetFileContents("old/p/I.aidl",
                               "package p; interface I {"
                               "const int A = 1; }");
  io_delegate_.SetFileContents("new/p/I.aidl",
                               "package p ; interface I {"
                               "const int A = 1; const int B = 2;}");
  EXPECT_TRUE(::android::aidl::check_api(options_, io_delegate_));
}

TEST_F(AidlTestCompatibleChanges, ChangedConstValueOrder) {
  io_delegate_.SetFileContents("old/p/I.aidl",
                               "package p; interface I {"
                               "const int A = 1; const int B = 2;}");
  io_delegate_.SetFileContents("new/p/I.aidl",
                               "package p ; interface I {"
                               "const int B = 2; const int A = 1;}");
  EXPECT_TRUE(::android::aidl::check_api(options_, io_delegate_));
}

class AidlTestIncompatibleChanges : public AidlTest {
 protected:
  Options options_ = Options::From("aidl --checkapi old new");
};

TEST_F(AidlTestIncompatibleChanges, RemovedType) {
  const string expected_stderr = "ERROR: new: API files have been removed: old/p/IFoo.aidl\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, RemovedMethod) {
  const string expected_stderr =
      "ERROR: old/p/IFoo.aidl:1.61-65: Removed or changed method: p.IFoo.bar(String)\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, UntypedListInInterface) {
  const string expected_stderr =
      "ERROR: new/p/IFoo.aidl:1.61-65: "
      "Encountered an untyped List or Map. The use of untyped List/Map is "
      "prohibited because it is not guaranteed that the objects in the list are recognizable in "
      "the receiving side. Consider switching to an array or a generic List/Map.\n"
      "ERROR: new/p/IFoo.aidl: Failed to read.\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(in List arg);"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestCompatibleChanges, UntypedListInParcelable) {
  const string expected_stderr =
      "ERROR: new/p/Data.aidl:1.54-59: "
      "Encountered an untyped List or Map. The use of untyped List/Map is "
      "prohibited because it is not guaranteed that the objects in the list are recognizable in "
      "the receiving side. Consider switching to an array or a generic List/Map.\n"
      "ERROR: new/p/Data.aidl: Failed to read.\n";
  io_delegate_.SetFileContents("old/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  @nullable List list;"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, RemovedField) {
  const string expected_stderr =
      "ERROR: new/p/Data.aidl:1.21-26: Number of fields in p.Data is reduced from 2 to 1.\n";
  io_delegate_.SetFileContents("old/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, RemovedEnumerator) {
  const string expected_stderr =
      "ERROR: new/p/Enum.aidl:1.15-20: Removed enumerator from p.Enum: FOO\n";
  io_delegate_.SetFileContents("old/p/Enum.aidl",
                               "package p;"
                               "enum Enum {"
                               "  FOO = 1,"
                               "  BAR = 2,"
                               "}");
  io_delegate_.SetFileContents("new/p/Enum.aidl",
                               "package p;"
                               "enum Enum {"
                               "  BAR = 2,"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, RenamedMethod) {
  const string expected_stderr =
      "ERROR: old/p/IFoo.aidl:1.61-65: Removed or changed method: p.IFoo.bar(String)\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar2(@utf8InCpp String str);"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, RenamedField) {
  const string expected_stderr = "ERROR: new/p/Data.aidl:1.21-26: Renamed field: bar to bar2.\n";
  io_delegate_.SetFileContents("old/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar2;"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, RenamedType) {
  const string expected_stderr = "ERROR: old/p/IFoo.aidl:1.11-20: Removed type: p.IFoo\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo2.aidl",
                               "package p;"
                               "interface IFoo2 {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, ChangedEnumerator) {
  const string expected_stderr =
      "ERROR: new/p/Enum.aidl:1.15-20: Changed enumerator value: p.Enum::FOO from 1 to 3.\n";
  io_delegate_.SetFileContents("old/p/Enum.aidl",
                               "package p;"
                               "enum Enum {"
                               "  FOO = 1,"
                               "  BAR = 2,"
                               "}");
  io_delegate_.SetFileContents("new/p/Enum.aidl",
                               "package p;"
                               "enum Enum {"
                               "  FOO = 3,"
                               "  BAR = 2,"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, ReorderedMethod) {
  const string expected_stderr =
      "ERROR: new/p/IFoo.aidl:1.67-71: Transaction ID changed: p.IFoo.foo(String[]) is changed "
      "from 0 to 1.\n"
      "ERROR: new/p/IFoo.aidl:1.33-37: Transaction ID changed: p.IFoo.bar(String) is changed from "
      "1 to 0.\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void bar(@utf8InCpp String str);"
                               "  void foo(in String[] str);"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, ReorderedField) {
  const string expected_stderr =
      "ERROR: new/p/Data.aidl:1.21-26: Renamed field: foo to bar.\n"
      "ERROR: new/p/Data.aidl:1.21-26: Renamed field: bar to foo.\n";
  io_delegate_.SetFileContents("old/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int bar;"
                               "  int foo;"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, ChangedDirectionSpecifier) {
  const string expected_stderr = "ERROR: new/p/IFoo.aidl:1.33-37: Direction changed: in to out.\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(out String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, AddedAnnotation) {
  const string expected_stderr =
      "ERROR: new/p/IFoo.aidl:1.51-58: Changed annotations: (empty) to @utf8InCpp\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in @utf8InCpp String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, RemovedAnnotation) {
  const string expected_stderr =
      "ERROR: new/p/IFoo.aidl:1.66-72: Changed annotations: @utf8InCpp to (empty)\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(String str);"
                               "}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, RemovedPackage) {
  const string expected_stderr = "ERROR: old/q/IFoo.aidl:1.11-21: Removed type: q.IFoo\n";
  io_delegate_.SetFileContents("old/p/IFoo.aidl", "package p; interface IFoo{}");
  io_delegate_.SetFileContents("old/q/IFoo.aidl", "package q; interface IFoo{}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "package p; interface IFoo{}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, ChangedDefaultValue) {
  const string expected_stderr = "ERROR: new/p/D.aidl:1.30-32: Changed default value: 1 to 2.\n";
  io_delegate_.SetFileContents("old/p/D.aidl", "package p; parcelable D { int a = 1; }");
  io_delegate_.SetFileContents("new/p/D.aidl", "package p; parcelable D { int a = 2; }");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, RemovedConstValue) {
  const string expected_stderr =
      "ERROR: old/p/I.aidl:1.51-53: Removed constant declaration: p.I.B\n";
  io_delegate_.SetFileContents("old/p/I.aidl",
                               "package p; interface I {"
                               "const int A = 1; const int B = 2;}");
  io_delegate_.SetFileContents("new/p/I.aidl", "package p; interface I { const int A = 1; }");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTestIncompatibleChanges, ChangedConstValue) {
  const string expected_stderr =
      "ERROR: new/p/I.aidl:1.11-21: Changed constant value: p.I.A from 1 to 2.\n";
  io_delegate_.SetFileContents("old/p/I.aidl", "package p; interface I { const int A = 1; }");
  io_delegate_.SetFileContents("new/p/I.aidl", "package p; interface I { const int A = 2; }");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options_, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, RejectAmbiguousImports) {
  const string expected_stderr =
      "ERROR: p/IFoo.aidl: Duplicate files found for q.IBar from:\n"
      "dir1/q/IBar.aidl\n"
      "dir2/q/IBar.aidl\n"
      "ERROR: p/IFoo.aidl: Couldn't find import for class q.IBar\n";
  Options options = Options::From("aidl --lang=java -o out -I dir1 -I dir2 p/IFoo.aidl");
  io_delegate_.SetFileContents("p/IFoo.aidl", "package p; import q.IBar; interface IFoo{}");
  io_delegate_.SetFileContents("dir1/q/IBar.aidl", "package q; interface IBar{}");
  io_delegate_.SetFileContents("dir2/q/IBar.aidl", "package q; interface IBar{}");

  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, HandleManualIdAssignments) {
  const string expected_stderr =
      "ERROR: new/p/IFoo.aidl:1.32-36: Transaction ID changed: p.IFoo.foo() is changed from 10 to "
      "11.\n";
  Options options = Options::From("aidl --checkapi old new");
  io_delegate_.SetFileContents("old/p/IFoo.aidl", "package p; interface IFoo{ void foo() = 10;}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "package p; interface IFoo{ void foo() = 10;}");

  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));

  io_delegate_.SetFileContents("new/p/IFoo.aidl", "package p; interface IFoo{ void foo() = 11;}");
  CaptureStderr();
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, ParcelFileDescriptorIsBuiltinType) {
  Options javaOptions = Options::From("aidl --lang=java -o out p/IFoo.aidl");
  Options cppOptions = Options::From("aidl --lang=cpp -h out -o out p/IFoo.aidl");

  // use without import
  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo{ void foo(in ParcelFileDescriptor fd);}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(javaOptions, io_delegate_));
  EXPECT_EQ(0, ::android::aidl::compile_aidl(cppOptions, io_delegate_));

  // use without import but with full name
  io_delegate_.SetFileContents(
      "p/IFoo.aidl",
      "package p; interface IFoo{ void foo(in android.os.ParcelFileDescriptor fd);}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(javaOptions, io_delegate_));
  EXPECT_EQ(0, ::android::aidl::compile_aidl(cppOptions, io_delegate_));

  // use with import (as before)
  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p;"
                               "import android.os.ParcelFileDescriptor;"
                               "interface IFoo{"
                               "  void foo(in ParcelFileDescriptor fd);"
                               "}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(javaOptions, io_delegate_));
  EXPECT_EQ(0, ::android::aidl::compile_aidl(cppOptions, io_delegate_));
}

TEST_F(AidlTest, ManualIds) {
  Options options = Options::From("aidl --lang=java -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 0;\n"
                               "  void bar() = 1;\n"
                               "}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, ManualIdsWithMetaTransactions) {
  Options options = Options::From("aidl --lang=java --version 10 -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 0;\n"
                               "  void bar() = 1;\n"
                               "}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, FailOnDuplicatedIds) {
  const string expected_stderr =
      "ERROR: IFoo.aidl:3.7-11: Found duplicate method id (3) for method bar\n";
  Options options = Options::From("aidl --lang=java --version 10 -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 3;\n"
                               "  void bar() = 3;\n"
                               "}");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, FailOnOutOfRangeIds) {
  // 16777115 is kLastMetaMethodId + 1
  const string expected_stderr =
      "ERROR: IFoo.aidl:3.7-11: Found out of bounds id (16777115) for method bar. "
      "Value for id must be between 0 and 16777114 inclusive.\n";
  Options options = Options::From("aidl --lang=java --version 10 -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 3;\n"
                               "  void bar() = 16777115;\n"
                               "}");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, FailOnPartiallyAssignedIds) {
  const string expected_stderr =
      "ERROR: IFoo.aidl:3.7-11: You must either assign id's to all methods or to none of them.\n";
  Options options = Options::From("aidl --lang=java --version 10 -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 3;\n"
                               "  void bar();\n"
                               "}");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

TEST_F(AidlTest, AllowDuplicatedImportPaths) {
  Options options = Options::From("aidl --lang=java -I dir -I dir IFoo.aidl");
  io_delegate_.SetFileContents("dir/IBar.aidl", "interface IBar{}");
  io_delegate_.SetFileContents("IFoo.aidl", "import IBar; interface IFoo{}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, FailOnAmbiguousImports) {
  const string expected_stderr =
      "ERROR: IFoo.aidl: Duplicate files found for IBar from:\n"
      "dir/IBar.aidl\n"
      "dir2/IBar.aidl\n"
      "ERROR: IFoo.aidl: Couldn't find import for class IBar\n";

  Options options = Options::From("aidl --lang=java -I dir -I dir2 IFoo.aidl");
  io_delegate_.SetFileContents("dir/IBar.aidl", "interface IBar{}");
  io_delegate_.SetFileContents("dir2/IBar.aidl", "interface IBar{}");
  io_delegate_.SetFileContents("IFoo.aidl", "import IBar; interface IFoo{}");
  CaptureStderr();
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
}

class AidlOutputPathTest : public AidlTest {
 protected:
  void SetUp() override {
    AidlTest::SetUp();
    io_delegate_.SetFileContents("sub/dir/foo/bar/IFoo.aidl", "package foo.bar; interface IFoo {}");
  }

  void Test(const Options& options, const std::string expected_output_path) {
    EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
    // check the existence
    EXPECT_TRUE(io_delegate_.GetWrittenContents(expected_output_path, nullptr));
  }
};

TEST_F(AidlOutputPathTest, OutDirWithNoOutputFile) {
  // <out_dir> / <package_name> / <type_name>.java
  Test(Options::From("aidl -o out sub/dir/foo/bar/IFoo.aidl"), "out/foo/bar/IFoo.java");
}

TEST_F(AidlOutputPathTest, OutDirWithOutputFile) {
  // when output file is explicitly set, it is always respected. -o option is
  // ignored.
  Test(Options::From("aidl -o out sub/dir/foo/bar/IFoo.aidl output/IFoo.java"), "output/IFoo.java");
}

TEST_F(AidlOutputPathTest, NoOutDirWithOutputFile) {
  Test(Options::From("aidl -o out sub/dir/foo/bar/IFoo.aidl output/IFoo.java"), "output/IFoo.java");
}

TEST_F(AidlOutputPathTest, NoOutDirWithNoOutputFile) {
  // output is the same as the input file except for the suffix
  Test(Options::From("aidl sub/dir/foo/bar/IFoo.aidl"), "sub/dir/foo/bar/IFoo.java");
}

TEST_P(AidlTest, FailOnOutOfBoundsInt32MaxConstInt) {
  AidlError error;
  const string expected_stderr =
      "ERROR: p/IFoo.aidl:3.58-69: Invalid type specifier for an int64 literal: int\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl",
                           R"(package p;
                              interface IFoo {
                                const int int32_max_oob = 2147483650;
                              }
                             )",
                           typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::BAD_TYPE, error);
}

TEST_P(AidlTest, FailOnOutOfBoundsInt32MinConstInt) {
  AidlError error;
  const string expected_stderr =
      "ERROR: p/IFoo.aidl:3.58-60: Invalid type specifier for an int64 literal: int\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl",
                           R"(package p;
                              interface IFoo {
                                const int int32_min_oob = -2147483650;
                              }
                             )",
                           typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::BAD_TYPE, error);
}

TEST_P(AidlTest, FailOnOutOfBoundsInt64MaxConstInt) {
  AidlError error;
  const string expected_stderr =
      "ERROR: Could not parse integer: 21474836509999999999999999 at p/IFoo.aidl:3.59-85.\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl",
                           R"(package p;
                              interface IFoo {
                                const long int64_max_oob = 21474836509999999999999999;
                              }
                             )",
                           typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::PARSE_ERROR, error);
}

TEST_P(AidlTest, FailOnOutOfBoundsInt64MinConstInt) {
  AidlError error;
  const string expected_stderr =
      "ERROR: Could not parse integer: 21474836509999999999999999 at p/IFoo.aidl:3.61-86.\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl",
                           R"(package p;
                              interface IFoo {
                                const long int64_min_oob = -21474836509999999999999999;
                              }
                             )",
                           typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::PARSE_ERROR, error);
}

TEST_P(AidlTest, FailOnOutOfBoundsAutofilledEnum) {
  AidlError error;
  const string expected_stderr =
      "ERROR: p/TestEnum.aidl:3.35-44: Invalid type specifier for an int32 "
      "literal: byte\n"
      "ERROR: p/TestEnum.aidl:5.1-36: Enumerator type differs from enum backing type.\n";
  CaptureStderr();
  EXPECT_EQ(nullptr, Parse("p/TestEnum.aidl",
                           R"(package p;
                              @Backing(type="byte")
                              enum TestEnum {
                                FOO = 127,
                                BAR,
                              }
                             )",
                           typenames_, GetLanguage(), &error));
  EXPECT_EQ(expected_stderr, GetCapturedStderr());
  EXPECT_EQ(AidlError::BAD_TYPE, error);
}

}  // namespace aidl
}  // namespace android
