package android.aidl.versioned.tests;
import android.aidl.versioned.tests.BazUnion;
interface IFooInterface {
    // V1
    void originalApi();
    @utf8InCpp String acceptUnionAndReturnString(in BazUnion u);
    // V2
    void newApi();
}
