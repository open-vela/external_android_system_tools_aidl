package android.aidl.tests.permission.platform;

import android.content.AttributionSource;

interface IProtected {
    @Enforce("permission=INTERNET && permission=VIBRATE || uid=SYSTEM_UID")
    void ProtectedWithSourceAttribution(in AttributionSource source);
}
