/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
package android.aidl.tests.permission;
public interface IProtected extends android.os.IInterface
{
  /** Default implementation for IProtected. */
  public static class Default implements android.aidl.tests.permission.IProtected
  {
    @Override public void PermissionProtected() throws android.os.RemoteException
    {
    }
    @Override public void MultiplePermissions() throws android.os.RemoteException
    {
    }
    @Override public void MultiplePermissions2() throws android.os.RemoteException
    {
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements android.aidl.tests.permission.IProtected
  {
    /** Construct the stub at attach it to the interface. */
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an android.aidl.tests.permission.IProtected interface,
     * generating a proxy if needed.
     */
    public static android.aidl.tests.permission.IProtected asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof android.aidl.tests.permission.IProtected))) {
        return ((android.aidl.tests.permission.IProtected)iin);
      }
      return new android.aidl.tests.permission.IProtected.Stub.Proxy(obj);
    }
    @Override public android.os.IBinder asBinder()
    {
      return this;
    }
    @Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
    {
      java.lang.String descriptor = DESCRIPTOR;
      if (code >= android.os.IBinder.FIRST_CALL_TRANSACTION && code <= android.os.IBinder.LAST_CALL_TRANSACTION) {
        data.enforceInterface(descriptor);
      }
      switch (code)
      {
        case INTERFACE_TRANSACTION:
        {
          reply.writeString(descriptor);
          return true;
        }
      }
      switch (code)
      {
        case TRANSACTION_PermissionProtected:
        {
          if ((this.permissionCheckerWrapper(android.Manifest.permission.READ_PHONE_STATE, this.getCallingPid(), new android.content.AttributionSource(getCallingUid(), null, null))!=true)) {
            throw new SecurityException("Access denied, requires: permission = READ_PHONE_STATE");
          }
          this.PermissionProtected();
          reply.writeNoException();
          break;
        }
        case TRANSACTION_MultiplePermissions:
        {
          if (((this.permissionCheckerWrapper(android.Manifest.permission.ACCESS_FINE_LOCATION, this.getCallingPid(), new android.content.AttributionSource(getCallingUid(), null, null))||(this.getCallingUid()==android.os.Process.SYSTEM_UID))!=true)) {
            throw new SecurityException("Access denied, requires: permission = ACCESS_FINE_LOCATION || uid = SYSTEM_UID");
          }
          this.MultiplePermissions();
          reply.writeNoException();
          break;
        }
        case TRANSACTION_MultiplePermissions2:
        {
          if ((((this.permissionCheckerWrapper(android.Manifest.permission.INTERNET, this.getCallingPid(), new android.content.AttributionSource(getCallingUid(), null, null))&&this.permissionCheckerWrapper(android.Manifest.permission.VIBRATE, this.getCallingPid(), new android.content.AttributionSource(getCallingUid(), null, null)))||(this.getCallingUid()==android.os.Process.SYSTEM_UID))!=true)) {
            throw new SecurityException("Access denied, requires: permission = INTERNET && permission = VIBRATE || uid = SYSTEM_UID");
          }
          this.MultiplePermissions2();
          reply.writeNoException();
          break;
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
      return true;
    }
    private static class Proxy implements android.aidl.tests.permission.IProtected
    {
      private android.os.IBinder mRemote;
      Proxy(android.os.IBinder remote)
      {
        mRemote = remote;
      }
      @Override public android.os.IBinder asBinder()
      {
        return mRemote;
      }
      public java.lang.String getInterfaceDescriptor()
      {
        return DESCRIPTOR;
      }
      @Override public void PermissionProtected() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain(asBinder());
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_PermissionProtected, _data, _reply, 0);
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
      }
      @Override public void MultiplePermissions() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain(asBinder());
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_MultiplePermissions, _data, _reply, 0);
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
      }
      @Override public void MultiplePermissions2() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain(asBinder());
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_MultiplePermissions2, _data, _reply, 0);
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
      }
    }
    private boolean permissionCheckerWrapper(
        String permission, int pid, android.content.AttributionSource attributionSource) {
      android.content.Context ctx =
          android.app.ActivityThread.currentActivityThread().getSystemContext();
      return (android.content.PermissionChecker.checkPermissionForDataDelivery(
              ctx, permission, pid, attributionSource, "" /*message*/) ==
          android.content.PermissionChecker.PERMISSION_GRANTED);
    }
    static final int TRANSACTION_PermissionProtected = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
    static final int TRANSACTION_MultiplePermissions = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
    static final int TRANSACTION_MultiplePermissions2 = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
  }
  public static final java.lang.String DESCRIPTOR = "android$aidl$tests$permission$IProtected".replace('$', '.');
  public void PermissionProtected() throws android.os.RemoteException;
  public void MultiplePermissions() throws android.os.RemoteException;
  public void MultiplePermissions2() throws android.os.RemoteException;
}
