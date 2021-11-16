package android.aidl.tests.permission;

@Enforce("permission=ACCESS_FINE_LOCATION")
interface IProtectedInterface {
    void Method1();

    @Enforce("permission=INTERNET") void Method2();
}
