/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tests/test_data.h"

namespace android {
namespace aidl {
namespace test_data {
namespace example_interface {

const char kCanonicalName[] = "android.test.IExampleInterface";

const char kJavaOutputPath[] = "some/path/to/output.java";

const char* kImportedParcelables[] = {
  "android.foo.ExampleParcelable",
  nullptr,
};

const char* kImportedInterfaces[] = {
  "android.bar.IAuxInterface",
  "android.test.IAuxInterface2",
  nullptr,
};

const char kInterfaceDefinition[] = R"(
package android.test;

import android.foo.ExampleParcelable;
import android.test.CompoundParcelable;
import android.bar.IAuxInterface;
import android.test.IAuxInterface2;

@Hide
@UnsupportedAppUsage
interface IExampleInterface {
    const int EXAMPLE_CONSTANT = 3;
    boolean isEnabled();
    int getState();
    String getAddress();

    /* Test long comment */
    @Hide
    @UnsupportedAppUsage
    ExampleParcelable[] getParcelables();

    // Test short comment
    boolean setScanMode(int mode, int duration);

    /* Test long comment */
    // And short comment
    void registerBinder(IAuxInterface foo);
    IExampleInterface getRecursiveBinder();

    int takesAnInterface(in IAuxInterface2 arg);
    int takesAParcelable(in CompoundParcelable.Subclass1 arg,
                         inout CompoundParcelable.Subclass2 arg2);
}
)";

const char kInterfaceDefinitionOutlining[] = R"(
package android.test;

import android.foo.ExampleParcelable;
import android.test.CompoundParcelable;
import android.bar.IAuxInterface;
import android.test.IAuxInterface2;

interface IExampleInterface {
    const int EXAMPLE_CONSTANT = 3;
    boolean isEnabled();
    int getState(int a, int b);
    String getAddress();

    /* Test long comment */
    ExampleParcelable[] getParcelables();

    // Test short comment
    boolean setScanMode(int mode, int duration);

    /* Test long comment */
    // And short comment
    void registerBinder(IAuxInterface foo);
    IExampleInterface getRecursiveBinder();

    int takesAnInterface(in IAuxInterface2 arg);
    int takesAParcelable(in CompoundParcelable.Subclass1 arg,
                         inout CompoundParcelable.Subclass2 arg2);
}
)";

const char kExpectedJavaDepsOutput[] =
    R"(some/path/to/output.java : \
  android/test/IExampleInterface.aidl \
  ./android/foo/ExampleParcelable.aidl \
  ./android/test/CompoundParcelable.aidl \
  ./android/bar/IAuxInterface.aidl \
  ./android/test/IAuxInterface2.aidl

android/test/IExampleInterface.aidl :
./android/foo/ExampleParcelable.aidl :
./android/test/CompoundParcelable.aidl :
./android/bar/IAuxInterface.aidl :
./android/test/IAuxInterface2.aidl :
)";

const char kExpectedJavaOutput[] =
    R"(/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
package android.test;
@android.annotation.Hide
@android.compat.annotation.UnsupportedAppUsage(overrideSourcePosition="android/test/IExampleInterface.aidl:10:1:10:21")
public interface IExampleInterface extends android.os.IInterface
{
  /** Default implementation for IExampleInterface. */
  public static class Default implements android.test.IExampleInterface
  {
    @Override public boolean isEnabled() throws android.os.RemoteException
    {
      return false;
    }
    @Override public int getState() throws android.os.RemoteException
    {
      return 0;
    }
    @Override public java.lang.String getAddress() throws android.os.RemoteException
    {
      return null;
    }
    /* Test long comment */
    @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
    {
      return null;
    }
    // Test short comment

    @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
    {
      return false;
    }
    /* Test long comment */// And short comment

    @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
    {
    }
    @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
    {
      return null;
    }
    @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
    {
      return 0;
    }
    @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
    {
      return 0;
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements android.test.IExampleInterface
  {
    /** Construct the stub at attach it to the interface. */
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an android.test.IExampleInterface interface,
     * generating a proxy if needed.
     */
    public static android.test.IExampleInterface asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof android.test.IExampleInterface))) {
        return ((android.test.IExampleInterface)iin);
      }
      return new android.test.IExampleInterface.Stub.Proxy(obj);
    }
    @Override public android.os.IBinder asBinder()
    {
      return this;
    }
    @Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
    {
      java.lang.String descriptor = DESCRIPTOR;
      switch (code)
      {
        case INTERFACE_TRANSACTION:
        {
          reply.writeString(descriptor);
          return true;
        }
        case TRANSACTION_isEnabled:
        {
          data.enforceInterface(descriptor);
          boolean _result = this.isEnabled();
          reply.writeNoException();
          reply.writeInt(((_result)?(1):(0)));
          return true;
        }
        case TRANSACTION_getState:
        {
          data.enforceInterface(descriptor);
          int _result = this.getState();
          reply.writeNoException();
          reply.writeInt(_result);
          return true;
        }
        case TRANSACTION_getAddress:
        {
          data.enforceInterface(descriptor);
          java.lang.String _result = this.getAddress();
          reply.writeNoException();
          reply.writeString(_result);
          return true;
        }
        case TRANSACTION_getParcelables:
        {
          data.enforceInterface(descriptor);
          android.foo.ExampleParcelable[] _result = this.getParcelables();
          reply.writeNoException();
          reply.writeTypedArray(_result, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
          return true;
        }
        case TRANSACTION_setScanMode:
        {
          data.enforceInterface(descriptor);
          int _arg0;
          _arg0 = data.readInt();
          int _arg1;
          _arg1 = data.readInt();
          boolean _result = this.setScanMode(_arg0, _arg1);
          reply.writeNoException();
          reply.writeInt(((_result)?(1):(0)));
          return true;
        }
        case TRANSACTION_registerBinder:
        {
          data.enforceInterface(descriptor);
          android.bar.IAuxInterface _arg0;
          _arg0 = android.bar.IAuxInterface.Stub.asInterface(data.readStrongBinder());
          this.registerBinder(_arg0);
          reply.writeNoException();
          return true;
        }
        case TRANSACTION_getRecursiveBinder:
        {
          data.enforceInterface(descriptor);
          android.test.IExampleInterface _result = this.getRecursiveBinder();
          reply.writeNoException();
          reply.writeStrongBinder((((_result!=null))?(_result.asBinder()):(null)));
          return true;
        }
        case TRANSACTION_takesAnInterface:
        {
          data.enforceInterface(descriptor);
          android.test.IAuxInterface2 _arg0;
          _arg0 = android.test.IAuxInterface2.Stub.asInterface(data.readStrongBinder());
          int _result = this.takesAnInterface(_arg0);
          reply.writeNoException();
          reply.writeInt(_result);
          return true;
        }
        case TRANSACTION_takesAParcelable:
        {
          data.enforceInterface(descriptor);
          android.test.CompoundParcelable.Subclass1 _arg0;
          if ((0!=data.readInt())) {
            _arg0 = android.test.CompoundParcelable.Subclass1.CREATOR.createFromParcel(data);
          }
          else {
            _arg0 = null;
          }
          android.test.CompoundParcelable.Subclass2 _arg1;
          if ((0!=data.readInt())) {
            _arg1 = android.test.CompoundParcelable.Subclass2.CREATOR.createFromParcel(data);
          }
          else {
            _arg1 = null;
          }
          int _result = this.takesAParcelable(_arg0, _arg1);
          reply.writeNoException();
          reply.writeInt(_result);
          if ((_arg1!=null)) {
            reply.writeInt(1);
            _arg1.writeToParcel(reply, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
          }
          else {
            reply.writeInt(0);
          }
          return true;
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
    }
    private static class Proxy implements android.test.IExampleInterface
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
      @Override public boolean isEnabled() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_isEnabled, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().isEnabled();
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int getState() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getState, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getState();
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public java.lang.String getAddress() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        java.lang.String _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getAddress, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getAddress();
            }
          }
          _reply.readException();
          _result = _reply.readString();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      /* Test long comment */
      @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.foo.ExampleParcelable[] _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getParcelables, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getParcelables();
            }
          }
          _reply.readException();
          _result = _reply.createTypedArray(android.foo.ExampleParcelable.CREATOR);
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      // Test short comment

      @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeInt(mode);
          _data.writeInt(duration);
          boolean _status = mRemote.transact(Stub.TRANSACTION_setScanMode, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().setScanMode(mode, duration);
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      /* Test long comment */// And short comment

      @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((foo!=null))?(foo.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_registerBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              getDefaultImpl().registerBinder(foo);
              return;
            }
          }
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
      }
      @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.test.IExampleInterface _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getRecursiveBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getRecursiveBinder();
            }
          }
          _reply.readException();
          _result = android.test.IExampleInterface.Stub.asInterface(_reply.readStrongBinder());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((arg!=null))?(arg.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAnInterface, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAnInterface(arg);
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          if ((arg!=null)) {
            _data.writeInt(1);
            arg.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          if ((arg2!=null)) {
            _data.writeInt(1);
            arg2.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAParcelable, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAParcelable(arg, arg2);
            }
          }
          _reply.readException();
          _result = _reply.readInt();
          if ((0!=_reply.readInt())) {
            arg2.readFromParcel(_reply);
          }
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      public static android.test.IExampleInterface sDefaultImpl;
    }
    static final int TRANSACTION_isEnabled = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
    static final int TRANSACTION_getState = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
    static final int TRANSACTION_getAddress = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
    static final int TRANSACTION_getParcelables = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
    static final int TRANSACTION_setScanMode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 4);
    static final int TRANSACTION_registerBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 5);
    static final int TRANSACTION_getRecursiveBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 6);
    static final int TRANSACTION_takesAnInterface = (android.os.IBinder.FIRST_CALL_TRANSACTION + 7);
    static final int TRANSACTION_takesAParcelable = (android.os.IBinder.FIRST_CALL_TRANSACTION + 8);
    public static boolean setDefaultImpl(android.test.IExampleInterface impl) {
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
    public static android.test.IExampleInterface getDefaultImpl() {
      return Stub.Proxy.sDefaultImpl;
    }
  }
  public static final java.lang.String DESCRIPTOR = "android.test.IExampleInterface";
  public static final int EXAMPLE_CONSTANT = 3;
  public boolean isEnabled() throws android.os.RemoteException;
  public int getState() throws android.os.RemoteException;
  public java.lang.String getAddress() throws android.os.RemoteException;
  /* Test long comment */
  @android.annotation.Hide
  @android.compat.annotation.UnsupportedAppUsage(overrideSourcePosition="android/test/IExampleInterface.aidl:19:1:19:25")
  public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException;
  // Test short comment

  public boolean setScanMode(int mode, int duration) throws android.os.RemoteException;
  /* Test long comment */// And short comment

  public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException;
  public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException;
  public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException;
  public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException;
}
)";

const char kExpectedJavaOutputWithTransactionNames[] =
    R"(/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
package android.test;
@android.annotation.Hide
@android.compat.annotation.UnsupportedAppUsage(overrideSourcePosition="android/test/IExampleInterface.aidl:10:1:10:21")
public interface IExampleInterface extends android.os.IInterface
{
  /** Default implementation for IExampleInterface. */
  public static class Default implements android.test.IExampleInterface
  {
    @Override public boolean isEnabled() throws android.os.RemoteException
    {
      return false;
    }
    @Override public int getState() throws android.os.RemoteException
    {
      return 0;
    }
    @Override public java.lang.String getAddress() throws android.os.RemoteException
    {
      return null;
    }
    /* Test long comment */
    @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
    {
      return null;
    }
    // Test short comment

    @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
    {
      return false;
    }
    /* Test long comment */// And short comment

    @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
    {
    }
    @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
    {
      return null;
    }
    @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
    {
      return 0;
    }
    @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
    {
      return 0;
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements android.test.IExampleInterface
  {
    /** Construct the stub at attach it to the interface. */
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an android.test.IExampleInterface interface,
     * generating a proxy if needed.
     */
    public static android.test.IExampleInterface asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof android.test.IExampleInterface))) {
        return ((android.test.IExampleInterface)iin);
      }
      return new android.test.IExampleInterface.Stub.Proxy(obj);
    }
    @Override public android.os.IBinder asBinder()
    {
      return this;
    }
    /** @hide */
    public static java.lang.String getDefaultTransactionName(int transactionCode)
    {
      switch (transactionCode)
      {
        case TRANSACTION_isEnabled:
        {
          return "isEnabled";
        }
        case TRANSACTION_getState:
        {
          return "getState";
        }
        case TRANSACTION_getAddress:
        {
          return "getAddress";
        }
        case TRANSACTION_getParcelables:
        {
          return "getParcelables";
        }
        case TRANSACTION_setScanMode:
        {
          return "setScanMode";
        }
        case TRANSACTION_registerBinder:
        {
          return "registerBinder";
        }
        case TRANSACTION_getRecursiveBinder:
        {
          return "getRecursiveBinder";
        }
        case TRANSACTION_takesAnInterface:
        {
          return "takesAnInterface";
        }
        case TRANSACTION_takesAParcelable:
        {
          return "takesAParcelable";
        }
        default:
        {
          return null;
        }
      }
    }
    /** @hide */
    public java.lang.String getTransactionName(int transactionCode)
    {
      return this.getDefaultTransactionName(transactionCode);
    }
    @Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
    {
      java.lang.String descriptor = DESCRIPTOR;
      switch (code)
      {
        case INTERFACE_TRANSACTION:
        {
          reply.writeString(descriptor);
          return true;
        }
        case TRANSACTION_isEnabled:
        {
          data.enforceInterface(descriptor);
          boolean _result = this.isEnabled();
          reply.writeNoException();
          reply.writeInt(((_result)?(1):(0)));
          return true;
        }
        case TRANSACTION_getState:
        {
          data.enforceInterface(descriptor);
          int _result = this.getState();
          reply.writeNoException();
          reply.writeInt(_result);
          return true;
        }
        case TRANSACTION_getAddress:
        {
          data.enforceInterface(descriptor);
          java.lang.String _result = this.getAddress();
          reply.writeNoException();
          reply.writeString(_result);
          return true;
        }
        case TRANSACTION_getParcelables:
        {
          data.enforceInterface(descriptor);
          android.foo.ExampleParcelable[] _result = this.getParcelables();
          reply.writeNoException();
          reply.writeTypedArray(_result, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
          return true;
        }
        case TRANSACTION_setScanMode:
        {
          data.enforceInterface(descriptor);
          int _arg0;
          _arg0 = data.readInt();
          int _arg1;
          _arg1 = data.readInt();
          boolean _result = this.setScanMode(_arg0, _arg1);
          reply.writeNoException();
          reply.writeInt(((_result)?(1):(0)));
          return true;
        }
        case TRANSACTION_registerBinder:
        {
          data.enforceInterface(descriptor);
          android.bar.IAuxInterface _arg0;
          _arg0 = android.bar.IAuxInterface.Stub.asInterface(data.readStrongBinder());
          this.registerBinder(_arg0);
          reply.writeNoException();
          return true;
        }
        case TRANSACTION_getRecursiveBinder:
        {
          data.enforceInterface(descriptor);
          android.test.IExampleInterface _result = this.getRecursiveBinder();
          reply.writeNoException();
          reply.writeStrongBinder((((_result!=null))?(_result.asBinder()):(null)));
          return true;
        }
        case TRANSACTION_takesAnInterface:
        {
          data.enforceInterface(descriptor);
          android.test.IAuxInterface2 _arg0;
          _arg0 = android.test.IAuxInterface2.Stub.asInterface(data.readStrongBinder());
          int _result = this.takesAnInterface(_arg0);
          reply.writeNoException();
          reply.writeInt(_result);
          return true;
        }
        case TRANSACTION_takesAParcelable:
        {
          data.enforceInterface(descriptor);
          android.test.CompoundParcelable.Subclass1 _arg0;
          if ((0!=data.readInt())) {
            _arg0 = android.test.CompoundParcelable.Subclass1.CREATOR.createFromParcel(data);
          }
          else {
            _arg0 = null;
          }
          android.test.CompoundParcelable.Subclass2 _arg1;
          if ((0!=data.readInt())) {
            _arg1 = android.test.CompoundParcelable.Subclass2.CREATOR.createFromParcel(data);
          }
          else {
            _arg1 = null;
          }
          int _result = this.takesAParcelable(_arg0, _arg1);
          reply.writeNoException();
          reply.writeInt(_result);
          if ((_arg1!=null)) {
            reply.writeInt(1);
            _arg1.writeToParcel(reply, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
          }
          else {
            reply.writeInt(0);
          }
          return true;
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
    }
    private static class Proxy implements android.test.IExampleInterface
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
      @Override public boolean isEnabled() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_isEnabled, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().isEnabled();
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int getState() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getState, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getState();
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public java.lang.String getAddress() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        java.lang.String _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getAddress, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getAddress();
            }
          }
          _reply.readException();
          _result = _reply.readString();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      /* Test long comment */
      @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.foo.ExampleParcelable[] _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getParcelables, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getParcelables();
            }
          }
          _reply.readException();
          _result = _reply.createTypedArray(android.foo.ExampleParcelable.CREATOR);
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      // Test short comment

      @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeInt(mode);
          _data.writeInt(duration);
          boolean _status = mRemote.transact(Stub.TRANSACTION_setScanMode, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().setScanMode(mode, duration);
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      /* Test long comment */// And short comment

      @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((foo!=null))?(foo.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_registerBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              getDefaultImpl().registerBinder(foo);
              return;
            }
          }
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
      }
      @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.test.IExampleInterface _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getRecursiveBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getRecursiveBinder();
            }
          }
          _reply.readException();
          _result = android.test.IExampleInterface.Stub.asInterface(_reply.readStrongBinder());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((arg!=null))?(arg.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAnInterface, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAnInterface(arg);
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          if ((arg!=null)) {
            _data.writeInt(1);
            arg.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          if ((arg2!=null)) {
            _data.writeInt(1);
            arg2.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAParcelable, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAParcelable(arg, arg2);
            }
          }
          _reply.readException();
          _result = _reply.readInt();
          if ((0!=_reply.readInt())) {
            arg2.readFromParcel(_reply);
          }
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      public static android.test.IExampleInterface sDefaultImpl;
    }
    static final int TRANSACTION_isEnabled = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
    static final int TRANSACTION_getState = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
    static final int TRANSACTION_getAddress = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
    static final int TRANSACTION_getParcelables = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
    static final int TRANSACTION_setScanMode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 4);
    static final int TRANSACTION_registerBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 5);
    static final int TRANSACTION_getRecursiveBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 6);
    static final int TRANSACTION_takesAnInterface = (android.os.IBinder.FIRST_CALL_TRANSACTION + 7);
    static final int TRANSACTION_takesAParcelable = (android.os.IBinder.FIRST_CALL_TRANSACTION + 8);
    public static boolean setDefaultImpl(android.test.IExampleInterface impl) {
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
    public static android.test.IExampleInterface getDefaultImpl() {
      return Stub.Proxy.sDefaultImpl;
    }
  }
  public static final java.lang.String DESCRIPTOR = "android.test.IExampleInterface";
  public static final int EXAMPLE_CONSTANT = 3;
  public boolean isEnabled() throws android.os.RemoteException;
  public int getState() throws android.os.RemoteException;
  public java.lang.String getAddress() throws android.os.RemoteException;
  /* Test long comment */
  @android.annotation.Hide
  @android.compat.annotation.UnsupportedAppUsage(overrideSourcePosition="android/test/IExampleInterface.aidl:19:1:19:25")
  public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException;
  // Test short comment

  public boolean setScanMode(int mode, int duration) throws android.os.RemoteException;
  /* Test long comment */// And short comment

  public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException;
  public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException;
  public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException;
  public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException;
}
)";

const char kExpectedJavaOutputWithTrace[] =
    R"(/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
package android.test;
@android.annotation.Hide
@android.compat.annotation.UnsupportedAppUsage(overrideSourcePosition="android/test/IExampleInterface.aidl:10:1:10:21")
public interface IExampleInterface extends android.os.IInterface
{
  /** Default implementation for IExampleInterface. */
  public static class Default implements android.test.IExampleInterface
  {
    @Override public boolean isEnabled() throws android.os.RemoteException
    {
      return false;
    }
    @Override public int getState() throws android.os.RemoteException
    {
      return 0;
    }
    @Override public java.lang.String getAddress() throws android.os.RemoteException
    {
      return null;
    }
    /* Test long comment */
    @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
    {
      return null;
    }
    // Test short comment

    @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
    {
      return false;
    }
    /* Test long comment */// And short comment

    @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
    {
    }
    @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
    {
      return null;
    }
    @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
    {
      return 0;
    }
    @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
    {
      return 0;
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements android.test.IExampleInterface
  {
    /** Construct the stub at attach it to the interface. */
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an android.test.IExampleInterface interface,
     * generating a proxy if needed.
     */
    public static android.test.IExampleInterface asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof android.test.IExampleInterface))) {
        return ((android.test.IExampleInterface)iin);
      }
      return new android.test.IExampleInterface.Stub.Proxy(obj);
    }
    @Override public android.os.IBinder asBinder()
    {
      return this;
    }
    @Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
    {
      java.lang.String descriptor = DESCRIPTOR;
      switch (code)
      {
        case INTERFACE_TRANSACTION:
        {
          reply.writeString(descriptor);
          return true;
        }
        case TRANSACTION_isEnabled:
        {
          try {
            android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::isEnabled::server");
            data.enforceInterface(descriptor);
            boolean _result = this.isEnabled();
            reply.writeNoException();
            reply.writeInt(((_result)?(1):(0)));
            return true;
          }
          finally {
            android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
          }
        }
        case TRANSACTION_getState:
        {
          try {
            android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::getState::server");
            data.enforceInterface(descriptor);
            int _result = this.getState();
            reply.writeNoException();
            reply.writeInt(_result);
            return true;
          }
          finally {
            android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
          }
        }
        case TRANSACTION_getAddress:
        {
          try {
            android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::getAddress::server");
            data.enforceInterface(descriptor);
            java.lang.String _result = this.getAddress();
            reply.writeNoException();
            reply.writeString(_result);
            return true;
          }
          finally {
            android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
          }
        }
        case TRANSACTION_getParcelables:
        {
          try {
            android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::getParcelables::server");
            data.enforceInterface(descriptor);
            android.foo.ExampleParcelable[] _result = this.getParcelables();
            reply.writeNoException();
            reply.writeTypedArray(_result, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
            return true;
          }
          finally {
            android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
          }
        }
        case TRANSACTION_setScanMode:
        {
          try {
            android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::setScanMode::server");
            data.enforceInterface(descriptor);
            int _arg0;
            _arg0 = data.readInt();
            int _arg1;
            _arg1 = data.readInt();
            boolean _result = this.setScanMode(_arg0, _arg1);
            reply.writeNoException();
            reply.writeInt(((_result)?(1):(0)));
            return true;
          }
          finally {
            android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
          }
        }
        case TRANSACTION_registerBinder:
        {
          try {
            android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::registerBinder::server");
            data.enforceInterface(descriptor);
            android.bar.IAuxInterface _arg0;
            _arg0 = android.bar.IAuxInterface.Stub.asInterface(data.readStrongBinder());
            this.registerBinder(_arg0);
            reply.writeNoException();
            return true;
          }
          finally {
            android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
          }
        }
        case TRANSACTION_getRecursiveBinder:
        {
          try {
            android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::getRecursiveBinder::server");
            data.enforceInterface(descriptor);
            android.test.IExampleInterface _result = this.getRecursiveBinder();
            reply.writeNoException();
            reply.writeStrongBinder((((_result!=null))?(_result.asBinder()):(null)));
            return true;
          }
          finally {
            android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
          }
        }
        case TRANSACTION_takesAnInterface:
        {
          try {
            android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::takesAnInterface::server");
            data.enforceInterface(descriptor);
            android.test.IAuxInterface2 _arg0;
            _arg0 = android.test.IAuxInterface2.Stub.asInterface(data.readStrongBinder());
            int _result = this.takesAnInterface(_arg0);
            reply.writeNoException();
            reply.writeInt(_result);
            return true;
          }
          finally {
            android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
          }
        }
        case TRANSACTION_takesAParcelable:
        {
          try {
            android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::takesAParcelable::server");
            data.enforceInterface(descriptor);
            android.test.CompoundParcelable.Subclass1 _arg0;
            if ((0!=data.readInt())) {
              _arg0 = android.test.CompoundParcelable.Subclass1.CREATOR.createFromParcel(data);
            }
            else {
              _arg0 = null;
            }
            android.test.CompoundParcelable.Subclass2 _arg1;
            if ((0!=data.readInt())) {
              _arg1 = android.test.CompoundParcelable.Subclass2.CREATOR.createFromParcel(data);
            }
            else {
              _arg1 = null;
            }
            int _result = this.takesAParcelable(_arg0, _arg1);
            reply.writeNoException();
            reply.writeInt(_result);
            if ((_arg1!=null)) {
              reply.writeInt(1);
              _arg1.writeToParcel(reply, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
            }
            else {
              reply.writeInt(0);
            }
            return true;
          }
          finally {
            android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
          }
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
    }
    private static class Proxy implements android.test.IExampleInterface
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
      @Override public boolean isEnabled() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::isEnabled::client");
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_isEnabled, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().isEnabled();
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
          android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
        }
        return _result;
      }
      @Override public int getState() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::getState::client");
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getState, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getState();
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
          android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
        }
        return _result;
      }
      @Override public java.lang.String getAddress() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        java.lang.String _result;
        try {
          android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::getAddress::client");
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getAddress, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getAddress();
            }
          }
          _reply.readException();
          _result = _reply.readString();
        }
        finally {
          _reply.recycle();
          _data.recycle();
          android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
        }
        return _result;
      }
      /* Test long comment */
      @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.foo.ExampleParcelable[] _result;
        try {
          android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::getParcelables::client");
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getParcelables, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getParcelables();
            }
          }
          _reply.readException();
          _result = _reply.createTypedArray(android.foo.ExampleParcelable.CREATOR);
        }
        finally {
          _reply.recycle();
          _data.recycle();
          android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
        }
        return _result;
      }
      // Test short comment

      @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::setScanMode::client");
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeInt(mode);
          _data.writeInt(duration);
          boolean _status = mRemote.transact(Stub.TRANSACTION_setScanMode, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().setScanMode(mode, duration);
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
          android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
        }
        return _result;
      }
      /* Test long comment */// And short comment

      @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::registerBinder::client");
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((foo!=null))?(foo.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_registerBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              getDefaultImpl().registerBinder(foo);
              return;
            }
          }
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
          android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
        }
      }
      @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.test.IExampleInterface _result;
        try {
          android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::getRecursiveBinder::client");
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getRecursiveBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getRecursiveBinder();
            }
          }
          _reply.readException();
          _result = android.test.IExampleInterface.Stub.asInterface(_reply.readStrongBinder());
        }
        finally {
          _reply.recycle();
          _data.recycle();
          android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
        }
        return _result;
      }
      @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::takesAnInterface::client");
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((arg!=null))?(arg.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAnInterface, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAnInterface(arg);
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
          android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
        }
        return _result;
      }
      @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          android.os.Trace.traceBegin(android.os.Trace.TRACE_TAG_AIDL, "AIDL::java::IExampleInterface::takesAParcelable::client");
          _data.writeInterfaceToken(DESCRIPTOR);
          if ((arg!=null)) {
            _data.writeInt(1);
            arg.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          if ((arg2!=null)) {
            _data.writeInt(1);
            arg2.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAParcelable, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAParcelable(arg, arg2);
            }
          }
          _reply.readException();
          _result = _reply.readInt();
          if ((0!=_reply.readInt())) {
            arg2.readFromParcel(_reply);
          }
        }
        finally {
          _reply.recycle();
          _data.recycle();
          android.os.Trace.traceEnd(android.os.Trace.TRACE_TAG_AIDL);
        }
        return _result;
      }
      public static android.test.IExampleInterface sDefaultImpl;
    }
    static final int TRANSACTION_isEnabled = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
    static final int TRANSACTION_getState = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
    static final int TRANSACTION_getAddress = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
    static final int TRANSACTION_getParcelables = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
    static final int TRANSACTION_setScanMode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 4);
    static final int TRANSACTION_registerBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 5);
    static final int TRANSACTION_getRecursiveBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 6);
    static final int TRANSACTION_takesAnInterface = (android.os.IBinder.FIRST_CALL_TRANSACTION + 7);
    static final int TRANSACTION_takesAParcelable = (android.os.IBinder.FIRST_CALL_TRANSACTION + 8);
    public static boolean setDefaultImpl(android.test.IExampleInterface impl) {
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
    public static android.test.IExampleInterface getDefaultImpl() {
      return Stub.Proxy.sDefaultImpl;
    }
  }
  public static final java.lang.String DESCRIPTOR = "android.test.IExampleInterface";
  public static final int EXAMPLE_CONSTANT = 3;
  public boolean isEnabled() throws android.os.RemoteException;
  public int getState() throws android.os.RemoteException;
  public java.lang.String getAddress() throws android.os.RemoteException;
  /* Test long comment */
  @android.annotation.Hide
  @android.compat.annotation.UnsupportedAppUsage(overrideSourcePosition="android/test/IExampleInterface.aidl:19:1:19:25")
  public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException;
  // Test short comment

  public boolean setScanMode(int mode, int duration) throws android.os.RemoteException;
  /* Test long comment */// And short comment

  public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException;
  public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException;
  public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException;
  public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException;
}
)";

const char kExpectedJavaOutputOutlining[] =
    R"(/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
package android.test;
public interface IExampleInterface extends android.os.IInterface
{
  /** Default implementation for IExampleInterface. */
  public static class Default implements android.test.IExampleInterface
  {
    @Override public boolean isEnabled() throws android.os.RemoteException
    {
      return false;
    }
    @Override public int getState(int a, int b) throws android.os.RemoteException
    {
      return 0;
    }
    @Override public java.lang.String getAddress() throws android.os.RemoteException
    {
      return null;
    }
    /* Test long comment */
    @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
    {
      return null;
    }
    // Test short comment

    @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
    {
      return false;
    }
    /* Test long comment */// And short comment

    @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
    {
    }
    @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
    {
      return null;
    }
    @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
    {
      return 0;
    }
    @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
    {
      return 0;
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements android.test.IExampleInterface
  {
    /** Construct the stub at attach it to the interface. */
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an android.test.IExampleInterface interface,
     * generating a proxy if needed.
     */
    public static android.test.IExampleInterface asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof android.test.IExampleInterface))) {
        return ((android.test.IExampleInterface)iin);
      }
      return new android.test.IExampleInterface.Stub.Proxy(obj);
    }
    @Override public android.os.IBinder asBinder()
    {
      return this;
    }
    @Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
    {
      java.lang.String descriptor = DESCRIPTOR;
      switch (code)
      {
        case INTERFACE_TRANSACTION:
        {
          reply.writeString(descriptor);
          return true;
        }
        case TRANSACTION_isEnabled:
        {
          data.enforceInterface(descriptor);
          boolean _result = this.isEnabled();
          reply.writeNoException();
          reply.writeInt(((_result)?(1):(0)));
          return true;
        }
        case TRANSACTION_getState:
        {
          return this.onTransact$getState$(data, reply);
        }
        case TRANSACTION_getAddress:
        {
          data.enforceInterface(descriptor);
          java.lang.String _result = this.getAddress();
          reply.writeNoException();
          reply.writeString(_result);
          return true;
        }
        case TRANSACTION_getParcelables:
        {
          data.enforceInterface(descriptor);
          android.foo.ExampleParcelable[] _result = this.getParcelables();
          reply.writeNoException();
          reply.writeTypedArray(_result, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
          return true;
        }
        case TRANSACTION_setScanMode:
        {
          return this.onTransact$setScanMode$(data, reply);
        }
        case TRANSACTION_registerBinder:
        {
          return this.onTransact$registerBinder$(data, reply);
        }
        case TRANSACTION_getRecursiveBinder:
        {
          return this.onTransact$getRecursiveBinder$(data, reply);
        }
        case TRANSACTION_takesAnInterface:
        {
          return this.onTransact$takesAnInterface$(data, reply);
        }
        case TRANSACTION_takesAParcelable:
        {
          return this.onTransact$takesAParcelable$(data, reply);
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
    }
    private static class Proxy implements android.test.IExampleInterface
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
      @Override public boolean isEnabled() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_isEnabled, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().isEnabled();
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int getState(int a, int b) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeInt(a);
          _data.writeInt(b);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getState, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getState(a, b);
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public java.lang.String getAddress() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        java.lang.String _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getAddress, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getAddress();
            }
          }
          _reply.readException();
          _result = _reply.readString();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      /* Test long comment */
      @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.foo.ExampleParcelable[] _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getParcelables, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getParcelables();
            }
          }
          _reply.readException();
          _result = _reply.createTypedArray(android.foo.ExampleParcelable.CREATOR);
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      // Test short comment

      @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeInt(mode);
          _data.writeInt(duration);
          boolean _status = mRemote.transact(Stub.TRANSACTION_setScanMode, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().setScanMode(mode, duration);
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      /* Test long comment */// And short comment

      @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((foo!=null))?(foo.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_registerBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              getDefaultImpl().registerBinder(foo);
              return;
            }
          }
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
      }
      @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.test.IExampleInterface _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getRecursiveBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getRecursiveBinder();
            }
          }
          _reply.readException();
          _result = android.test.IExampleInterface.Stub.asInterface(_reply.readStrongBinder());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((arg!=null))?(arg.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAnInterface, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAnInterface(arg);
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          if ((arg!=null)) {
            _data.writeInt(1);
            arg.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          if ((arg2!=null)) {
            _data.writeInt(1);
            arg2.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAParcelable, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAParcelable(arg, arg2);
            }
          }
          _reply.readException();
          _result = _reply.readInt();
          if ((0!=_reply.readInt())) {
            arg2.readFromParcel(_reply);
          }
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      public static android.test.IExampleInterface sDefaultImpl;
    }
    static final int TRANSACTION_isEnabled = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
    static final int TRANSACTION_getState = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
    private boolean onTransact$getState$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      int _arg0;
      _arg0 = data.readInt();
      int _arg1;
      _arg1 = data.readInt();
      int _result = this.getState(_arg0, _arg1);
      reply.writeNoException();
      reply.writeInt(_result);
      return true;
    }
    static final int TRANSACTION_getAddress = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
    static final int TRANSACTION_getParcelables = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
    static final int TRANSACTION_setScanMode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 4);
    private boolean onTransact$setScanMode$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      int _arg0;
      _arg0 = data.readInt();
      int _arg1;
      _arg1 = data.readInt();
      boolean _result = this.setScanMode(_arg0, _arg1);
      reply.writeNoException();
      reply.writeInt(((_result)?(1):(0)));
      return true;
    }
    static final int TRANSACTION_registerBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 5);
    private boolean onTransact$registerBinder$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      android.bar.IAuxInterface _arg0;
      _arg0 = android.bar.IAuxInterface.Stub.asInterface(data.readStrongBinder());
      this.registerBinder(_arg0);
      reply.writeNoException();
      return true;
    }
    static final int TRANSACTION_getRecursiveBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 6);
    private boolean onTransact$getRecursiveBinder$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      android.test.IExampleInterface _result = this.getRecursiveBinder();
      reply.writeNoException();
      reply.writeStrongBinder((((_result!=null))?(_result.asBinder()):(null)));
      return true;
    }
    static final int TRANSACTION_takesAnInterface = (android.os.IBinder.FIRST_CALL_TRANSACTION + 7);
    private boolean onTransact$takesAnInterface$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      android.test.IAuxInterface2 _arg0;
      _arg0 = android.test.IAuxInterface2.Stub.asInterface(data.readStrongBinder());
      int _result = this.takesAnInterface(_arg0);
      reply.writeNoException();
      reply.writeInt(_result);
      return true;
    }
    static final int TRANSACTION_takesAParcelable = (android.os.IBinder.FIRST_CALL_TRANSACTION + 8);
    private boolean onTransact$takesAParcelable$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      android.test.CompoundParcelable.Subclass1 _arg0;
      if ((0!=data.readInt())) {
        _arg0 = android.test.CompoundParcelable.Subclass1.CREATOR.createFromParcel(data);
      }
      else {
        _arg0 = null;
      }
      android.test.CompoundParcelable.Subclass2 _arg1;
      if ((0!=data.readInt())) {
        _arg1 = android.test.CompoundParcelable.Subclass2.CREATOR.createFromParcel(data);
      }
      else {
        _arg1 = null;
      }
      int _result = this.takesAParcelable(_arg0, _arg1);
      reply.writeNoException();
      reply.writeInt(_result);
      if ((_arg1!=null)) {
        reply.writeInt(1);
        _arg1.writeToParcel(reply, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
      }
      else {
        reply.writeInt(0);
      }
      return true;
    }
    public static boolean setDefaultImpl(android.test.IExampleInterface impl) {
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
    public static android.test.IExampleInterface getDefaultImpl() {
      return Stub.Proxy.sDefaultImpl;
    }
  }
  public static final java.lang.String DESCRIPTOR = "android.test.IExampleInterface";
  public static final int EXAMPLE_CONSTANT = 3;
  public boolean isEnabled() throws android.os.RemoteException;
  public int getState(int a, int b) throws android.os.RemoteException;
  public java.lang.String getAddress() throws android.os.RemoteException;
  /* Test long comment */
  public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException;
  // Test short comment

  public boolean setScanMode(int mode, int duration) throws android.os.RemoteException;
  /* Test long comment */// And short comment

  public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException;
  public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException;
  public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException;
  public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException;
}
)";

const char kExpectedJavaOutputWithVersionAndHash[] =
    R"(/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
package android.test;
public interface IExampleInterface extends android.os.IInterface
{
  /**
   * The version of this interface that the caller is built against.
   * This might be different from what {@link #getInterfaceVersion()
   * getInterfaceVersion} returns as that is the version of the interface
   * that the remote object is implementing.
   */
  public static final int VERSION = 10;
  public static final String HASH = "abcdefg";
  /** Default implementation for IExampleInterface. */
  public static class Default implements android.test.IExampleInterface
  {
    @Override public boolean isEnabled() throws android.os.RemoteException
    {
      return false;
    }
    @Override public int getState(int a, int b) throws android.os.RemoteException
    {
      return 0;
    }
    @Override public java.lang.String getAddress() throws android.os.RemoteException
    {
      return null;
    }
    /* Test long comment */
    @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
    {
      return null;
    }
    // Test short comment

    @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
    {
      return false;
    }
    /* Test long comment */// And short comment

    @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
    {
    }
    @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
    {
      return null;
    }
    @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
    {
      return 0;
    }
    @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
    {
      return 0;
    }
    @Override
    public int getInterfaceVersion() {
      return 0;
    }
    @Override
    public String getInterfaceHash() {
      return "";
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements android.test.IExampleInterface
  {
    /** Construct the stub at attach it to the interface. */
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an android.test.IExampleInterface interface,
     * generating a proxy if needed.
     */
    public static android.test.IExampleInterface asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof android.test.IExampleInterface))) {
        return ((android.test.IExampleInterface)iin);
      }
      return new android.test.IExampleInterface.Stub.Proxy(obj);
    }
    @Override public android.os.IBinder asBinder()
    {
      return this;
    }
    @Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
    {
      java.lang.String descriptor = DESCRIPTOR;
      switch (code)
      {
        case INTERFACE_TRANSACTION:
        {
          reply.writeString(descriptor);
          return true;
        }
        case TRANSACTION_isEnabled:
        {
          data.enforceInterface(descriptor);
          boolean _result = this.isEnabled();
          reply.writeNoException();
          reply.writeInt(((_result)?(1):(0)));
          return true;
        }
        case TRANSACTION_getState:
        {
          return this.onTransact$getState$(data, reply);
        }
        case TRANSACTION_getAddress:
        {
          data.enforceInterface(descriptor);
          java.lang.String _result = this.getAddress();
          reply.writeNoException();
          reply.writeString(_result);
          return true;
        }
        case TRANSACTION_getParcelables:
        {
          data.enforceInterface(descriptor);
          android.foo.ExampleParcelable[] _result = this.getParcelables();
          reply.writeNoException();
          reply.writeTypedArray(_result, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
          return true;
        }
        case TRANSACTION_setScanMode:
        {
          return this.onTransact$setScanMode$(data, reply);
        }
        case TRANSACTION_registerBinder:
        {
          return this.onTransact$registerBinder$(data, reply);
        }
        case TRANSACTION_getRecursiveBinder:
        {
          return this.onTransact$getRecursiveBinder$(data, reply);
        }
        case TRANSACTION_takesAnInterface:
        {
          return this.onTransact$takesAnInterface$(data, reply);
        }
        case TRANSACTION_takesAParcelable:
        {
          return this.onTransact$takesAParcelable$(data, reply);
        }
        case TRANSACTION_getInterfaceVersion:
        {
          data.enforceInterface(descriptor);
          reply.writeNoException();
          reply.writeInt(getInterfaceVersion());
          return true;
        }
        case TRANSACTION_getInterfaceHash:
        {
          data.enforceInterface(descriptor);
          reply.writeNoException();
          reply.writeString(getInterfaceHash());
          return true;
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
    }
    private static class Proxy implements android.test.IExampleInterface
    {
      private android.os.IBinder mRemote;
      Proxy(android.os.IBinder remote)
      {
        mRemote = remote;
      }
      private int mCachedVersion = -1;
      private String mCachedHash = "-1";
      @Override public android.os.IBinder asBinder()
      {
        return mRemote;
      }
      public java.lang.String getInterfaceDescriptor()
      {
        return DESCRIPTOR;
      }
      @Override public boolean isEnabled() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_isEnabled, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().isEnabled();
            }
            else {
              throw new RuntimeException("Method isEnabled is unimplemented.");
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int getState(int a, int b) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeInt(a);
          _data.writeInt(b);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getState, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getState(a, b);
            }
            else {
              throw new RuntimeException("Method getState is unimplemented.");
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public java.lang.String getAddress() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        java.lang.String _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getAddress, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getAddress();
            }
            else {
              throw new RuntimeException("Method getAddress is unimplemented.");
            }
          }
          _reply.readException();
          _result = _reply.readString();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      /* Test long comment */
      @Override public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.foo.ExampleParcelable[] _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getParcelables, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getParcelables();
            }
            else {
              throw new RuntimeException("Method getParcelables is unimplemented.");
            }
          }
          _reply.readException();
          _result = _reply.createTypedArray(android.foo.ExampleParcelable.CREATOR);
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      // Test short comment

      @Override public boolean setScanMode(int mode, int duration) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeInt(mode);
          _data.writeInt(duration);
          boolean _status = mRemote.transact(Stub.TRANSACTION_setScanMode, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().setScanMode(mode, duration);
            }
            else {
              throw new RuntimeException("Method setScanMode is unimplemented.");
            }
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      /* Test long comment */// And short comment

      @Override public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((foo!=null))?(foo.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_registerBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              getDefaultImpl().registerBinder(foo);
              return;
            }
            else {
              throw new RuntimeException("Method registerBinder is unimplemented.");
            }
          }
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
      }
      @Override public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        android.test.IExampleInterface _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getRecursiveBinder, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().getRecursiveBinder();
            }
            else {
              throw new RuntimeException("Method getRecursiveBinder is unimplemented.");
            }
          }
          _reply.readException();
          _result = android.test.IExampleInterface.Stub.asInterface(_reply.readStrongBinder());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeStrongBinder((((arg!=null))?(arg.asBinder()):(null)));
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAnInterface, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAnInterface(arg);
            }
            else {
              throw new RuntimeException("Method takesAnInterface is unimplemented.");
            }
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          if ((arg!=null)) {
            _data.writeInt(1);
            arg.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          if ((arg2!=null)) {
            _data.writeInt(1);
            arg2.writeToParcel(_data, 0);
          }
          else {
            _data.writeInt(0);
          }
          boolean _status = mRemote.transact(Stub.TRANSACTION_takesAParcelable, _data, _reply, 0);
          if (!_status) {
            if (getDefaultImpl() != null) {
              return getDefaultImpl().takesAParcelable(arg, arg2);
            }
            else {
              throw new RuntimeException("Method takesAParcelable is unimplemented.");
            }
          }
          _reply.readException();
          _result = _reply.readInt();
          if ((0!=_reply.readInt())) {
            arg2.readFromParcel(_reply);
          }
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override
      public int getInterfaceVersion() throws android.os.RemoteException {
        if (mCachedVersion == -1) {
          android.os.Parcel data = android.os.Parcel.obtain();
          android.os.Parcel reply = android.os.Parcel.obtain();
          try {
            data.writeInterfaceToken(DESCRIPTOR);
            boolean _status = mRemote.transact(Stub.TRANSACTION_getInterfaceVersion, data, reply, 0);
            if (!_status) {
              if (getDefaultImpl() != null) {
                return getDefaultImpl().getInterfaceVersion();
              }
            }
            reply.readException();
            mCachedVersion = reply.readInt();
          } finally {
            reply.recycle();
            data.recycle();
          }
        }
        return mCachedVersion;
      }
      @Override
      public synchronized String getInterfaceHash() throws android.os.RemoteException {
        if ("-1".equals(mCachedHash)) {
          android.os.Parcel data = android.os.Parcel.obtain();
          android.os.Parcel reply = android.os.Parcel.obtain();
          try {
            data.writeInterfaceToken(DESCRIPTOR);
            boolean _status = mRemote.transact(Stub.TRANSACTION_getInterfaceHash, data, reply, 0);
            if (!_status) {
              if (getDefaultImpl() != null) {
                return getDefaultImpl().getInterfaceHash();
              }
            }
            reply.readException();
            mCachedHash = reply.readString();
          } finally {
            reply.recycle();
            data.recycle();
          }
        }
        return mCachedHash;
      }
      public static android.test.IExampleInterface sDefaultImpl;
    }
    static final int TRANSACTION_isEnabled = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
    static final int TRANSACTION_getState = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
    private boolean onTransact$getState$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      int _arg0;
      _arg0 = data.readInt();
      int _arg1;
      _arg1 = data.readInt();
      int _result = this.getState(_arg0, _arg1);
      reply.writeNoException();
      reply.writeInt(_result);
      return true;
    }
    static final int TRANSACTION_getAddress = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
    static final int TRANSACTION_getParcelables = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
    static final int TRANSACTION_setScanMode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 4);
    private boolean onTransact$setScanMode$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      int _arg0;
      _arg0 = data.readInt();
      int _arg1;
      _arg1 = data.readInt();
      boolean _result = this.setScanMode(_arg0, _arg1);
      reply.writeNoException();
      reply.writeInt(((_result)?(1):(0)));
      return true;
    }
    static final int TRANSACTION_registerBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 5);
    private boolean onTransact$registerBinder$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      android.bar.IAuxInterface _arg0;
      _arg0 = android.bar.IAuxInterface.Stub.asInterface(data.readStrongBinder());
      this.registerBinder(_arg0);
      reply.writeNoException();
      return true;
    }
    static final int TRANSACTION_getRecursiveBinder = (android.os.IBinder.FIRST_CALL_TRANSACTION + 6);
    private boolean onTransact$getRecursiveBinder$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      android.test.IExampleInterface _result = this.getRecursiveBinder();
      reply.writeNoException();
      reply.writeStrongBinder((((_result!=null))?(_result.asBinder()):(null)));
      return true;
    }
    static final int TRANSACTION_takesAnInterface = (android.os.IBinder.FIRST_CALL_TRANSACTION + 7);
    private boolean onTransact$takesAnInterface$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      android.test.IAuxInterface2 _arg0;
      _arg0 = android.test.IAuxInterface2.Stub.asInterface(data.readStrongBinder());
      int _result = this.takesAnInterface(_arg0);
      reply.writeNoException();
      reply.writeInt(_result);
      return true;
    }
    static final int TRANSACTION_takesAParcelable = (android.os.IBinder.FIRST_CALL_TRANSACTION + 8);
    private boolean onTransact$takesAParcelable$(android.os.Parcel data, android.os.Parcel reply) throws android.os.RemoteException
    {
      data.enforceInterface(DESCRIPTOR);
      android.test.CompoundParcelable.Subclass1 _arg0;
      if ((0!=data.readInt())) {
        _arg0 = android.test.CompoundParcelable.Subclass1.CREATOR.createFromParcel(data);
      }
      else {
        _arg0 = null;
      }
      android.test.CompoundParcelable.Subclass2 _arg1;
      if ((0!=data.readInt())) {
        _arg1 = android.test.CompoundParcelable.Subclass2.CREATOR.createFromParcel(data);
      }
      else {
        _arg1 = null;
      }
      int _result = this.takesAParcelable(_arg0, _arg1);
      reply.writeNoException();
      reply.writeInt(_result);
      if ((_arg1!=null)) {
        reply.writeInt(1);
        _arg1.writeToParcel(reply, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
      }
      else {
        reply.writeInt(0);
      }
      return true;
    }
    static final int TRANSACTION_getInterfaceVersion = (android.os.IBinder.FIRST_CALL_TRANSACTION + 16777214);
    static final int TRANSACTION_getInterfaceHash = (android.os.IBinder.FIRST_CALL_TRANSACTION + 16777213);
    public static boolean setDefaultImpl(android.test.IExampleInterface impl) {
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
    public static android.test.IExampleInterface getDefaultImpl() {
      return Stub.Proxy.sDefaultImpl;
    }
  }
  public static final java.lang.String DESCRIPTOR = "android.test.IExampleInterface";
  public static final int EXAMPLE_CONSTANT = 3;
  public boolean isEnabled() throws android.os.RemoteException;
  public int getState(int a, int b) throws android.os.RemoteException;
  public java.lang.String getAddress() throws android.os.RemoteException;
  /* Test long comment */
  public android.foo.ExampleParcelable[] getParcelables() throws android.os.RemoteException;
  // Test short comment

  public boolean setScanMode(int mode, int duration) throws android.os.RemoteException;
  /* Test long comment */// And short comment

  public void registerBinder(android.bar.IAuxInterface foo) throws android.os.RemoteException;
  public android.test.IExampleInterface getRecursiveBinder() throws android.os.RemoteException;
  public int takesAnInterface(android.test.IAuxInterface2 arg) throws android.os.RemoteException;
  public int takesAParcelable(android.test.CompoundParcelable.Subclass1 arg, android.test.CompoundParcelable.Subclass2 arg2) throws android.os.RemoteException;
  public int getInterfaceVersion() throws android.os.RemoteException;
  public String getInterfaceHash() throws android.os.RemoteException;
}
)";

const char kRustOutputDirectory[] = "some/path/to";
const char kRustOutputPath[] = "some/path/to/android/test/IExampleInterface.rs";

const char kExpectedRustDepsOutput[] =
    R"(some/path/to/android/test/IExampleInterface.rs : \
  android/test/IExampleInterface.aidl \
  ./android/foo/ExampleParcelable.aidl \
  ./android/test/CompoundParcelable.aidl \
  ./android/bar/IAuxInterface.aidl \
  ./android/test/IAuxInterface2.aidl

android/test/IExampleInterface.aidl :
./android/foo/ExampleParcelable.aidl :
./android/test/CompoundParcelable.aidl :
./android/bar/IAuxInterface.aidl :
./android/test/IAuxInterface2.aidl :
)";

const char kExpectedRustOutput[] =
    R"(#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinder;
use binder::declare_binder_interface;
declare_binder_interface! {
  IExampleInterface["android.test.IExampleInterface"] {
    native: BnExampleInterface(on_transact),
    proxy: BpExampleInterface {
    },
  }
}
pub trait IExampleInterface: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.test.IExampleInterface" }
  fn isEnabled(&self) -> binder::public_api::Result<bool> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getState(&self) -> binder::public_api::Result<i32> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getAddress(&self) -> binder::public_api::Result<String> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getParcelables(&self) -> binder::public_api::Result<Vec<crate::mangled::_7_android_3_foo_17_ExampleParcelable>> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn setScanMode(&self, _arg_mode: i32, _arg_duration: i32) -> binder::public_api::Result<bool> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn registerBinder(&self, _arg_foo: &dyn crate::mangled::_7_android_3_bar_13_IAuxInterface) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getRecursiveBinder(&self) -> binder::public_api::Result<Box<dyn crate::mangled::_7_android_4_test_17_IExampleInterface>> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn takesAnInterface(&self, _arg_arg: &dyn crate::mangled::_7_android_4_test_14_IAuxInterface2) -> binder::public_api::Result<i32> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn takesAParcelable(&self, _arg_arg: &crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass1, _arg_arg2: &mut crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass2) -> binder::public_api::Result<i32> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getDefaultImpl() -> IExampleInterfaceDefault where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: IExampleInterfaceDefault) -> IExampleInterfaceDefault where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub mod transactions {
  #[allow(unused_imports)] use binder::IBinder;
  pub const isEnabled: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 0;
  pub const getState: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 1;
  pub const getAddress: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 2;
  pub const getParcelables: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 3;
  pub const setScanMode: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 4;
  pub const registerBinder: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 5;
  pub const getRecursiveBinder: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 6;
  pub const takesAnInterface: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 7;
  pub const takesAParcelable: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 8;
}
pub type IExampleInterfaceDefault = Option<std::sync::Arc<dyn IExampleInterface + Sync>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<IExampleInterfaceDefault> = std::sync::Mutex::new(None);
}
pub const EXAMPLE_CONSTANT: i32 = 3;
pub(crate) mod mangled { pub use super::IExampleInterface as _7_android_4_test_17_IExampleInterface; }
impl IExampleInterface for BpExampleInterface {
  fn isEnabled(&self) -> binder::public_api::Result<bool> {
    let _aidl_reply = self.binder.transact(transactions::isEnabled, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.isEnabled();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: bool = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn getState(&self) -> binder::public_api::Result<i32> {
    let _aidl_reply = self.binder.transact(transactions::getState, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getState();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: i32 = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn getAddress(&self) -> binder::public_api::Result<String> {
    let _aidl_reply = self.binder.transact(transactions::getAddress, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getAddress();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn getParcelables(&self) -> binder::public_api::Result<Vec<crate::mangled::_7_android_3_foo_17_ExampleParcelable>> {
    let _aidl_reply = self.binder.transact(transactions::getParcelables, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getParcelables();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: Vec<crate::mangled::_7_android_3_foo_17_ExampleParcelable> = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn setScanMode(&self, _arg_mode: i32, _arg_duration: i32) -> binder::public_api::Result<bool> {
    let _aidl_reply = self.binder.transact(transactions::setScanMode, 0, |_aidl_data| {
      _aidl_data.write(&_arg_mode)?;
      _aidl_data.write(&_arg_duration)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.setScanMode(_arg_mode, _arg_duration);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: bool = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn registerBinder(&self, _arg_foo: &dyn crate::mangled::_7_android_3_bar_13_IAuxInterface) -> binder::public_api::Result<()> {
    let _aidl_reply = self.binder.transact(transactions::registerBinder, 0, |_aidl_data| {
      _aidl_data.write(_arg_foo)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.registerBinder(_arg_foo);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
  fn getRecursiveBinder(&self) -> binder::public_api::Result<Box<dyn crate::mangled::_7_android_4_test_17_IExampleInterface>> {
    let _aidl_reply = self.binder.transact(transactions::getRecursiveBinder, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getRecursiveBinder();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: Box<dyn crate::mangled::_7_android_4_test_17_IExampleInterface> = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn takesAnInterface(&self, _arg_arg: &dyn crate::mangled::_7_android_4_test_14_IAuxInterface2) -> binder::public_api::Result<i32> {
    let _aidl_reply = self.binder.transact(transactions::takesAnInterface, 0, |_aidl_data| {
      _aidl_data.write(_arg_arg)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.takesAnInterface(_arg_arg);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: i32 = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn takesAParcelable(&self, _arg_arg: &crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass1, _arg_arg2: &mut crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass2) -> binder::public_api::Result<i32> {
    let _aidl_reply = self.binder.transact(transactions::takesAParcelable, 0, |_aidl_data| {
      _aidl_data.write(_arg_arg)?;
      _aidl_data.write(_arg_arg2)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.takesAParcelable(_arg_arg, _arg_arg2);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: i32 = _aidl_reply.read()?;
    *_arg_arg2 = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
}
impl IExampleInterface for binder::Binder<BnExampleInterface> {
  fn isEnabled(&self) -> binder::public_api::Result<bool> { self.0.isEnabled() }
  fn getState(&self) -> binder::public_api::Result<i32> { self.0.getState() }
  fn getAddress(&self) -> binder::public_api::Result<String> { self.0.getAddress() }
  fn getParcelables(&self) -> binder::public_api::Result<Vec<crate::mangled::_7_android_3_foo_17_ExampleParcelable>> { self.0.getParcelables() }
  fn setScanMode(&self, _arg_mode: i32, _arg_duration: i32) -> binder::public_api::Result<bool> { self.0.setScanMode(_arg_mode, _arg_duration) }
  fn registerBinder(&self, _arg_foo: &dyn crate::mangled::_7_android_3_bar_13_IAuxInterface) -> binder::public_api::Result<()> { self.0.registerBinder(_arg_foo) }
  fn getRecursiveBinder(&self) -> binder::public_api::Result<Box<dyn crate::mangled::_7_android_4_test_17_IExampleInterface>> { self.0.getRecursiveBinder() }
  fn takesAnInterface(&self, _arg_arg: &dyn crate::mangled::_7_android_4_test_14_IAuxInterface2) -> binder::public_api::Result<i32> { self.0.takesAnInterface(_arg_arg) }
  fn takesAParcelable(&self, _arg_arg: &crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass1, _arg_arg2: &mut crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass2) -> binder::public_api::Result<i32> { self.0.takesAParcelable(_arg_arg, _arg_arg2) }
}
fn on_transact(_aidl_service: &dyn IExampleInterface, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::Parcel, _aidl_reply: &mut binder::parcel::Parcel) -> binder::Result<()> {
  match _aidl_code {
    transactions::isEnabled => {
      let _aidl_return = _aidl_service.isEnabled();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getState => {
      let _aidl_return = _aidl_service.getState();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getAddress => {
      let _aidl_return = _aidl_service.getAddress();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getParcelables => {
      let _aidl_return = _aidl_service.getParcelables();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::setScanMode => {
      let _arg_mode: i32 = _aidl_data.read()?;
      let _arg_duration: i32 = _aidl_data.read()?;
      let _aidl_return = _aidl_service.setScanMode(_arg_mode, _arg_duration);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::registerBinder => {
      let _arg_foo: Box<dyn crate::mangled::_7_android_3_bar_13_IAuxInterface> = _aidl_data.read()?;
      let _aidl_return = _aidl_service.registerBinder(&*_arg_foo);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getRecursiveBinder => {
      let _aidl_return = _aidl_service.getRecursiveBinder();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::takesAnInterface => {
      let _arg_arg: Box<dyn crate::mangled::_7_android_4_test_14_IAuxInterface2> = _aidl_data.read()?;
      let _aidl_return = _aidl_service.takesAnInterface(&*_arg_arg);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::takesAParcelable => {
      let _arg_arg: crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass1 = _aidl_data.read()?;
      let mut _arg_arg2: crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass2 = _aidl_data.read()?;
      let _aidl_return = _aidl_service.takesAParcelable(&_arg_arg, &mut _arg_arg2);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
          _aidl_reply.write(&_arg_arg2)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    _ => Err(binder::StatusCode::UNKNOWN_TRANSACTION)
  }
}
)";

const char kExpectedRustOutputWithVersionAndHash[] =
    R"(#![allow(non_upper_case_globals)]
#![allow(non_snake_case)]
#[allow(unused_imports)] use binder::IBinder;
use binder::declare_binder_interface;
declare_binder_interface! {
  IExampleInterface["android.test.IExampleInterface"] {
    native: BnExampleInterface(on_transact),
    proxy: BpExampleInterface {
      cached_version: std::sync::atomic::AtomicI32 = std::sync::atomic::AtomicI32::new(-1),
      cached_hash: std::sync::Mutex<Option<String>> = std::sync::Mutex::new(None)
    },
  }
}
pub trait IExampleInterface: binder::Interface + Send {
  fn get_descriptor() -> &'static str where Self: Sized { "android.test.IExampleInterface" }
  fn isEnabled(&self) -> binder::public_api::Result<bool> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getState(&self, _arg_a: i32, _arg_b: i32) -> binder::public_api::Result<i32> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getAddress(&self) -> binder::public_api::Result<String> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getParcelables(&self) -> binder::public_api::Result<Vec<crate::mangled::_7_android_3_foo_17_ExampleParcelable>> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn setScanMode(&self, _arg_mode: i32, _arg_duration: i32) -> binder::public_api::Result<bool> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn registerBinder(&self, _arg_foo: &dyn crate::mangled::_7_android_3_bar_13_IAuxInterface) -> binder::public_api::Result<()> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getRecursiveBinder(&self) -> binder::public_api::Result<Box<dyn crate::mangled::_7_android_4_test_17_IExampleInterface>> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn takesAnInterface(&self, _arg_arg: &dyn crate::mangled::_7_android_4_test_14_IAuxInterface2) -> binder::public_api::Result<i32> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn takesAParcelable(&self, _arg_arg: &crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass1, _arg_arg2: &mut crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass2) -> binder::public_api::Result<i32> {
    Err(binder::StatusCode::UNKNOWN_TRANSACTION.into())
  }
  fn getInterfaceVersion(&self) -> binder::public_api::Result<i32> {
    Ok(VERSION)
  }
  fn getInterfaceHash(&self) -> binder::public_api::Result<String> {
    Ok(HASH.into())
  }
  fn getDefaultImpl() -> IExampleInterfaceDefault where Self: Sized {
    DEFAULT_IMPL.lock().unwrap().clone()
  }
  fn setDefaultImpl(d: IExampleInterfaceDefault) -> IExampleInterfaceDefault where Self: Sized {
    std::mem::replace(&mut *DEFAULT_IMPL.lock().unwrap(), d)
  }
}
pub mod transactions {
  #[allow(unused_imports)] use binder::IBinder;
  pub const isEnabled: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 0;
  pub const getState: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 1;
  pub const getAddress: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 2;
  pub const getParcelables: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 3;
  pub const setScanMode: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 4;
  pub const registerBinder: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 5;
  pub const getRecursiveBinder: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 6;
  pub const takesAnInterface: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 7;
  pub const takesAParcelable: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 8;
  pub const getInterfaceVersion: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 16777214;
  pub const getInterfaceHash: binder::TransactionCode = binder::SpIBinder::FIRST_CALL_TRANSACTION + 16777213;
}
pub type IExampleInterfaceDefault = Option<std::sync::Arc<dyn IExampleInterface + Sync>>;
use lazy_static::lazy_static;
lazy_static! {
  static ref DEFAULT_IMPL: std::sync::Mutex<IExampleInterfaceDefault> = std::sync::Mutex::new(None);
}
pub const EXAMPLE_CONSTANT: i32 = 3;
pub(crate) mod mangled { pub use super::IExampleInterface as _7_android_4_test_17_IExampleInterface; }
pub const VERSION: i32 = 10;
pub const HASH: &str = "abcdefg";
impl IExampleInterface for BpExampleInterface {
  fn isEnabled(&self) -> binder::public_api::Result<bool> {
    let _aidl_reply = self.binder.transact(transactions::isEnabled, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.isEnabled();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: bool = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn getState(&self, _arg_a: i32, _arg_b: i32) -> binder::public_api::Result<i32> {
    let _aidl_reply = self.binder.transact(transactions::getState, 0, |_aidl_data| {
      _aidl_data.write(&_arg_a)?;
      _aidl_data.write(&_arg_b)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getState(_arg_a, _arg_b);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: i32 = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn getAddress(&self) -> binder::public_api::Result<String> {
    let _aidl_reply = self.binder.transact(transactions::getAddress, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getAddress();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn getParcelables(&self) -> binder::public_api::Result<Vec<crate::mangled::_7_android_3_foo_17_ExampleParcelable>> {
    let _aidl_reply = self.binder.transact(transactions::getParcelables, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getParcelables();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: Vec<crate::mangled::_7_android_3_foo_17_ExampleParcelable> = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn setScanMode(&self, _arg_mode: i32, _arg_duration: i32) -> binder::public_api::Result<bool> {
    let _aidl_reply = self.binder.transact(transactions::setScanMode, 0, |_aidl_data| {
      _aidl_data.write(&_arg_mode)?;
      _aidl_data.write(&_arg_duration)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.setScanMode(_arg_mode, _arg_duration);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: bool = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn registerBinder(&self, _arg_foo: &dyn crate::mangled::_7_android_3_bar_13_IAuxInterface) -> binder::public_api::Result<()> {
    let _aidl_reply = self.binder.transact(transactions::registerBinder, 0, |_aidl_data| {
      _aidl_data.write(_arg_foo)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.registerBinder(_arg_foo);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    Ok(())
  }
  fn getRecursiveBinder(&self) -> binder::public_api::Result<Box<dyn crate::mangled::_7_android_4_test_17_IExampleInterface>> {
    let _aidl_reply = self.binder.transact(transactions::getRecursiveBinder, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getRecursiveBinder();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: Box<dyn crate::mangled::_7_android_4_test_17_IExampleInterface> = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn takesAnInterface(&self, _arg_arg: &dyn crate::mangled::_7_android_4_test_14_IAuxInterface2) -> binder::public_api::Result<i32> {
    let _aidl_reply = self.binder.transact(transactions::takesAnInterface, 0, |_aidl_data| {
      _aidl_data.write(_arg_arg)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.takesAnInterface(_arg_arg);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: i32 = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn takesAParcelable(&self, _arg_arg: &crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass1, _arg_arg2: &mut crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass2) -> binder::public_api::Result<i32> {
    let _aidl_reply = self.binder.transact(transactions::takesAParcelable, 0, |_aidl_data| {
      _aidl_data.write(_arg_arg)?;
      _aidl_data.write(_arg_arg2)?;
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.takesAParcelable(_arg_arg, _arg_arg2);
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: i32 = _aidl_reply.read()?;
    *_arg_arg2 = _aidl_reply.read()?;
    Ok(_aidl_return)
  }
  fn getInterfaceVersion(&self) -> binder::public_api::Result<i32> {
    let _aidl_version = self.cached_version.load(std::sync::atomic::Ordering::Relaxed);
    if _aidl_version != -1 { return Ok(_aidl_version); }
    let _aidl_reply = self.binder.transact(transactions::getInterfaceVersion, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getInterfaceVersion();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: i32 = _aidl_reply.read()?;
    self.cached_version.store(_aidl_return, std::sync::atomic::Ordering::Relaxed);
    Ok(_aidl_return)
  }
  fn getInterfaceHash(&self) -> binder::public_api::Result<String> {
    {
      let _aidl_hash_lock = self.cached_hash.lock().unwrap();
      if let Some(ref _aidl_hash) = *_aidl_hash_lock {
        return Ok(_aidl_hash.clone());
      }
    }
    let _aidl_reply = self.binder.transact(transactions::getInterfaceHash, 0, |_aidl_data| {
      Ok(())
    });
    if let Err(binder::StatusCode::UNKNOWN_TRANSACTION) = _aidl_reply {
      if let Some(_aidl_default_impl) = <Self as IExampleInterface>::getDefaultImpl() {
        return _aidl_default_impl.getInterfaceHash();
      }
    }
    let _aidl_reply = _aidl_reply?;
    let _aidl_status: binder::Status = _aidl_reply.read()?;
    if !_aidl_status.is_ok() { return Err(_aidl_status); }
    let _aidl_return: String = _aidl_reply.read()?;
    *self.cached_hash.lock().unwrap() = Some(_aidl_return.clone());
    Ok(_aidl_return)
  }
}
impl IExampleInterface for binder::Binder<BnExampleInterface> {
  fn isEnabled(&self) -> binder::public_api::Result<bool> { self.0.isEnabled() }
  fn getState(&self, _arg_a: i32, _arg_b: i32) -> binder::public_api::Result<i32> { self.0.getState(_arg_a, _arg_b) }
  fn getAddress(&self) -> binder::public_api::Result<String> { self.0.getAddress() }
  fn getParcelables(&self) -> binder::public_api::Result<Vec<crate::mangled::_7_android_3_foo_17_ExampleParcelable>> { self.0.getParcelables() }
  fn setScanMode(&self, _arg_mode: i32, _arg_duration: i32) -> binder::public_api::Result<bool> { self.0.setScanMode(_arg_mode, _arg_duration) }
  fn registerBinder(&self, _arg_foo: &dyn crate::mangled::_7_android_3_bar_13_IAuxInterface) -> binder::public_api::Result<()> { self.0.registerBinder(_arg_foo) }
  fn getRecursiveBinder(&self) -> binder::public_api::Result<Box<dyn crate::mangled::_7_android_4_test_17_IExampleInterface>> { self.0.getRecursiveBinder() }
  fn takesAnInterface(&self, _arg_arg: &dyn crate::mangled::_7_android_4_test_14_IAuxInterface2) -> binder::public_api::Result<i32> { self.0.takesAnInterface(_arg_arg) }
  fn takesAParcelable(&self, _arg_arg: &crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass1, _arg_arg2: &mut crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass2) -> binder::public_api::Result<i32> { self.0.takesAParcelable(_arg_arg, _arg_arg2) }
  fn getInterfaceVersion(&self) -> binder::public_api::Result<i32> { self.0.getInterfaceVersion() }
  fn getInterfaceHash(&self) -> binder::public_api::Result<String> { self.0.getInterfaceHash() }
}
fn on_transact(_aidl_service: &dyn IExampleInterface, _aidl_code: binder::TransactionCode, _aidl_data: &binder::parcel::Parcel, _aidl_reply: &mut binder::parcel::Parcel) -> binder::Result<()> {
  match _aidl_code {
    transactions::isEnabled => {
      let _aidl_return = _aidl_service.isEnabled();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getState => {
      let _arg_a: i32 = _aidl_data.read()?;
      let _arg_b: i32 = _aidl_data.read()?;
      let _aidl_return = _aidl_service.getState(_arg_a, _arg_b);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getAddress => {
      let _aidl_return = _aidl_service.getAddress();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getParcelables => {
      let _aidl_return = _aidl_service.getParcelables();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::setScanMode => {
      let _arg_mode: i32 = _aidl_data.read()?;
      let _arg_duration: i32 = _aidl_data.read()?;
      let _aidl_return = _aidl_service.setScanMode(_arg_mode, _arg_duration);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::registerBinder => {
      let _arg_foo: Box<dyn crate::mangled::_7_android_3_bar_13_IAuxInterface> = _aidl_data.read()?;
      let _aidl_return = _aidl_service.registerBinder(&*_arg_foo);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getRecursiveBinder => {
      let _aidl_return = _aidl_service.getRecursiveBinder();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::takesAnInterface => {
      let _arg_arg: Box<dyn crate::mangled::_7_android_4_test_14_IAuxInterface2> = _aidl_data.read()?;
      let _aidl_return = _aidl_service.takesAnInterface(&*_arg_arg);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::takesAParcelable => {
      let _arg_arg: crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass1 = _aidl_data.read()?;
      let mut _arg_arg2: crate::mangled::_7_android_4_test_18_CompoundParcelable_9_Subclass2 = _aidl_data.read()?;
      let _aidl_return = _aidl_service.takesAParcelable(&_arg_arg, &mut _arg_arg2);
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
          _aidl_reply.write(&_arg_arg2)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getInterfaceVersion => {
      let _aidl_return = _aidl_service.getInterfaceVersion();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    transactions::getInterfaceHash => {
      let _aidl_return = _aidl_service.getInterfaceHash();
      match &_aidl_return {
        Ok(_aidl_return) => {
          _aidl_reply.write(&binder::Status::from(binder::StatusCode::OK))?;
          _aidl_reply.write(_aidl_return)?;
        }
        Err(_aidl_status) => _aidl_reply.write(_aidl_status)?
      }
      Ok(())
    }
    _ => Err(binder::StatusCode::UNKNOWN_TRANSACTION)
  }
}
)";

}  // namespace example_interface
}  // namespace test_data
}  // namespace aidl
}  // namespace android
