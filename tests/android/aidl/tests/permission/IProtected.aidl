package android.aidl.tests.permission;

interface IProtected {
    @Enforce("permission=READ_PHONE_STATE") void PermissionProtected();

    @Enforce("permission=ACCESS_FINE_LOCATION || uid=SYSTEM_UID") void MultiplePermissions();

    @Enforce("permission=INTERNET && permission=VIBRATE || uid=SYSTEM_UID")
    void MultiplePermissions2();
}
