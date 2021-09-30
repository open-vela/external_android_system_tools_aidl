package android.aidl.tests;

interface IProtected {
    @Enforce(condition="permission = READ_PHONE_STATE") void PermissionProtected();

    @Enforce(condition="permission = ACCESS_FINE_LOCATION || uid = SYSTEM_UID")
    void MultiplePermissions();

    @Enforce(condition="permission = INTERNET && permission = VIBRATE || uid = SYSTEM_UID")
    void MultiplePermissions2();
}
