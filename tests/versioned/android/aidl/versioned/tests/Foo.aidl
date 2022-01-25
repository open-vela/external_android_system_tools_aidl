package android.aidl.versioned.tests;

@JavaSuppressLint(value={"NewApi"})
parcelable Foo {
    // V1 is empty
    // V2
    int intDefault42 = 42;
}
