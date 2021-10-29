package android.aidl.tests.permission;

@Enforce(condition="permission = ACCESS_FINE_LOCATION")
interface IProtectedInterface {
    void Method1();

    @Enforce(condition="permission = INTERNET") void Method2();
}
