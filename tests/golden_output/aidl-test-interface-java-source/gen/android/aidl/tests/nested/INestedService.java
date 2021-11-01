/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
package android.aidl.tests.nested;
public interface INestedService extends android.os.IInterface
{
  /** Default implementation for INestedService. */
  public static class Default implements android.aidl.tests.nested.INestedService
  {
    @Override public android.aidl.tests.nested.INestedService.Result flipStatus(android.aidl.tests.nested.ParcelableWithNested p) throws android.os.RemoteException
    {
      return null;
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements android.aidl.tests.nested.INestedService
  {
    /** Construct the stub at attach it to the interface. */
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an android.aidl.tests.nested.INestedService interface,
     * generating a proxy if needed.
     */
    public static android.aidl.tests.nested.INestedService asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof android.aidl.tests.nested.INestedService))) {
        return ((android.aidl.tests.nested.INestedService)iin);
      }
      return new android.aidl.tests.nested.INestedService.Stub.Proxy(obj);
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
        case TRANSACTION_flipStatus:
        {
          android.aidl.tests.nested.ParcelableWithNested _arg0;
          if ((0!=data.readInt())) {
            _arg0 = android.aidl.tests.nested.ParcelableWithNested.CREATOR.createFromParcel(data);
          }
          else {
            _arg0 = null;
          }
          android.aidl.tests.nested.INestedService.Result _result = this.flipStatus(_arg0);
          reply.writeNoException();
          if ((_result!=null)) {
            reply.writeInt(1);
            _result.writeToParcel(reply, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
          }
          else {
            reply.writeInt(0);
          }
          break;
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
      return true;
    }
    private static class Proxy implements android.aidl.tests.nested.INestedService
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
      @Override public android.aidl.tests.nested.INestedService.Result flipStatus(android.aidl.tests.nested.ParcelableWithNested p) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain(asBinder());
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.aidl.tests.nested.INestedService.Result _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          if ((p!=null)) {
            _data.writeInt(1);
            p.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          boolean _status = mRemote.transact(Stub.TRANSACTION_flipStatus, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().flipStatus(p);
            }
          }
          _reply.readException();
          if ((0!=_reply.readInt())) {
            _result = android.aidl.tests.nested.INestedService.Result.CREATOR.createFromParcel(_reply);
          }
          else {
            _result = null;
          }
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      public static android.aidl.tests.nested.INestedService sDefaultImpl;
    }
    static final int TRANSACTION_flipStatus = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
    public static boolean setDefaultImpl(android.aidl.tests.nested.INestedService impl) {
      // Only one user of this interface can use this function
      // at a time. This is a heuristic to detect if two different
      // users in the same process use this function.
      if (Stub.Proxy.sDefaultImpl != null) {
        throw new IllegalStateException("setDefaultImpl() called twice");
      }
      if (impl != null) {
        Stub.Proxy.sDefaultImpl = impl;
        return true;
      }
      return false;
    }
    public static android.aidl.tests.nested.INestedService getDefaultImpl() {
      return Stub.Proxy.sDefaultImpl;
    }
  }
  public static final java.lang.String DESCRIPTOR = "android$aidl$tests$nested$INestedService".replace('$', '.');
  public android.aidl.tests.nested.INestedService.Result flipStatus(android.aidl.tests.nested.ParcelableWithNested p) throws android.os.RemoteException;
  public static class Result implements android.os.Parcelable
  {
    public byte status = android.aidl.tests.nested.ParcelableWithNested.Status.OK;
    public static final android.os.Parcelable.Creator<Result> CREATOR = new android.os.Parcelable.Creator<Result>() {
      @Override
      public Result createFromParcel(android.os.Parcel _aidl_source) {
        Result _aidl_out = new Result();
        _aidl_out.readFromParcel(_aidl_source);
        return _aidl_out;
      }
      @Override
      public Result[] newArray(int _aidl_size) {
        return new Result[_aidl_size];
      }
    };
    @Override public final void writeToParcel(android.os.Parcel _aidl_parcel, int _aidl_flag)
    {
      int _aidl_start_pos = _aidl_parcel.dataPosition();
      _aidl_parcel.writeInt(0);
      _aidl_parcel.writeByte(status);
      int _aidl_end_pos = _aidl_parcel.dataPosition();
      _aidl_parcel.setDataPosition(_aidl_start_pos);
      _aidl_parcel.writeInt(_aidl_end_pos - _aidl_start_pos);
      _aidl_parcel.setDataPosition(_aidl_end_pos);
    }
    public final void readFromParcel(android.os.Parcel _aidl_parcel)
    {
      int _aidl_start_pos = _aidl_parcel.dataPosition();
      int _aidl_parcelable_size = _aidl_parcel.readInt();
      try {
        if (_aidl_parcelable_size < 0) return;
        if (_aidl_parcel.dataPosition() - _aidl_start_pos >= _aidl_parcelable_size) return;
        status = _aidl_parcel.readByte();
      } finally {
        if (_aidl_start_pos > (Integer.MAX_VALUE - _aidl_parcelable_size)) {
          throw new android.os.BadParcelableException("Overflow in the size of parcelable");
        }
        _aidl_parcel.setDataPosition(_aidl_start_pos + _aidl_parcelable_size);
      }
    }
    @Override
    public int describeContents() {
      int _mask = 0;
      return _mask;
    }
  }
}
