package android.aidl.versioned.tests;
import android.aidl.versioned.tests.BazUnion;
import android.aidl.versioned.tests.Foo;
interface IFooInterface {
    // V1
    void originalApi();
    @utf8InCpp String acceptUnionAndReturnString(in BazUnion u);
    void callWithFoo(out Foo outFoo);
    int ignoreParcelableAndRepeatInt(in Foo inFoo, int value);
    // V2
    void newApi();
}
