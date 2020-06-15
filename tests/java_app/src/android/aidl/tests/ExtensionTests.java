/*
 * Copyright (C) 2020, The Android Open Source Project
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

package android.aidl.tests;

import android.aidl.tests.extension.ExtendableParcelable;
import android.aidl.tests.extension.MyExt;
import android.aidl.tests.extension.MyExt2;
import android.aidl.tests.extension.MyExtLike;
import android.aidl.tests.nonvintf.NonVintfExtendableParcelable;
import android.aidl.tests.nonvintf.NonVintfParcelable;
import android.aidl.tests.unstable.UnstableExtendableParcelable;
import android.aidl.tests.unstable.UnstableParcelable;
import android.aidl.tests.vintf.VintfExtendableParcelable;
import android.aidl.tests.vintf.VintfParcelable;
import android.os.Parcel;
import android.os.ParcelableHolder;
import java.util.Arrays;
import java.util.HashMap;

class ExtensionTests {
  private TestLogger mLog;

  public ExtensionTests(TestLogger logger) { mLog = logger; }

  private static String dumpMyExt(MyExt ext) {
    if (ext == null) {
      return null;
    }
    return "{a: " + ext.a + ", b: " + ext.b + "}";
  }

  private static String dumpMyExt2(MyExt2 ext2) {
    if (ext2 == null) {
      return null;
    }
    return "{a: " + ext2.a + ", b: " + dumpMyExt(ext2.b) + ", " + ext2.c + "}";
  }
  public void checkStability() throws TestFailException {
    mLog.log("Checking if extension's stability is checked properly.");

    VintfExtendableParcelable vep = new VintfExtendableParcelable();
    VintfParcelable vp = new VintfParcelable();

    NonVintfExtendableParcelable sep = new NonVintfExtendableParcelable();
    NonVintfParcelable sp = new NonVintfParcelable();

    UnstableExtendableParcelable uep = new UnstableExtendableParcelable();
    UnstableParcelable up = new UnstableParcelable();

    mLog.assertTrue(
        vep.ext.setParcelable(vp), "vintf ParcelableHolder can contain a vintf parcelable");
    mLog.assertTrue(vep.ext.getParcelable(VintfParcelable.class) == vp,
        "vintf ParcelableHolder can contain a vintf parcelable");
    vep.ext.setParcelable(null);
    mLog.assertFalse(
        vep.ext.setParcelable(sp), "vintf ParcelableHolder cannot contain a non-vintf parcelable");
    mLog.assertTrue(vep.ext.getParcelable(NonVintfParcelable.class) == null,
        "vintf ParcelableHolder cannot contain a non-vintf parcelable");
    vep.ext.setParcelable(null);
    mLog.assertFalse(
        vep.ext.setParcelable(up), "vintf ParcelableHolder cannot contain an unstable parcelable");
    mLog.assertTrue(vep.ext.getParcelable(UnstableParcelable.class) == null,
        "vintf ParcelableHolder cannot contain an unstable parcelable");

    mLog.assertTrue(
        sep.ext.setParcelable(vp), "a stable ParcelableHolder can contain a vintf parcelable");
    mLog.assertTrue(sep.ext.getParcelable(VintfParcelable.class) == vp,
        "a stable ParcelableHolder can contain a vintf parcelable");
    sep.ext.setParcelable(null);
    mLog.assertTrue(
        sep.ext.setParcelable(sp), "a stable ParcelableHolder can contain a non-vintf parcelable");
    mLog.assertTrue(sep.ext.getParcelable(NonVintfParcelable.class) == sp,
        "a stable ParcelableHolder can contain a non-vintf parcelable");
    sep.ext.setParcelable(null);
    mLog.assertTrue(
        sep.ext.setParcelable(up), "a stable ParcelableHolder can contain an unstable parcelable");
    mLog.assertTrue(sep.ext.getParcelable(UnstableParcelable.class) == up,
        "a stable ParcelableHolder can contain an unstable parcelable");

    mLog.assertTrue(
        uep.ext.setParcelable(vp), "an unstable ParcelableHolder can contain a vintf parcelable");
    mLog.assertTrue(uep.ext.getParcelable(VintfParcelable.class) == vp,
        "an unstable ParcelableHolder can contain a vintf parcelable");
    uep.ext.setParcelable(null);
    mLog.assertTrue(uep.ext.setParcelable(sp),
        "an unstable ParcelableHolder can contain a non-vintf parcelable");
    mLog.assertTrue(uep.ext.getParcelable(NonVintfParcelable.class) == sp,
        "an unstable ParcelableHolder can contain a non-vintf parcelable");
    uep.ext.setParcelable(null);
    mLog.assertTrue(uep.ext.setParcelable(up),
        "an unstable ParcelableHolder can contain an unstable parcelable");
    mLog.assertTrue(uep.ext.getParcelable(UnstableParcelable.class) == up,
        "an unstable ParcelableHolder can contain an unstable parcelable");
  }
  public void checkExtension() throws TestFailException {
    mLog.log("Checking if extension's data is transferred well.");
    MyExt ext = new MyExt();
    ext.a = 42;
    ext.b = "EXT";

    MyExt2 ext2 = new MyExt2();
    ext2.a = 42;
    ext2.b = new MyExt();
    ext2.b.a = 24;
    ext2.b.b = "INEXT";
    ext2.c = "EXT2";

    Parcel parcel = Parcel.obtain();
    {
      ExtendableParcelable ep = new ExtendableParcelable();
      ep.a = 1;
      ep.b = "a";
      ep.c = 42L;

      ep.ext.setParcelable(ext);

      ep.ext2.setParcelable(ext2);
      MyExtLike extLike = ep.ext.<MyExtLike>getParcelable(MyExtLike.class);
      mLog.assertTrue(extLike == null,
          "The extension type must be MyExt, so it has to fail even though MyExtLike has the same structure as MyExt.");

      MyExt actualExt = ep.ext.getParcelable(MyExt.class);
      MyExt2 actualExt2 = ep.ext2.getParcelable(MyExt2.class);

      checkContent(ep, ext, ext2);

      ep.writeToParcel(parcel, 0);
    }
    parcel.setDataPosition(0);
    {
      ExtendableParcelable ep1 = new ExtendableParcelable();
      ep1.readFromParcel(parcel);

      parcel.setDataPosition(0);
      ep1.writeToParcel(parcel, 0);
      parcel.setDataPosition(0);

      ExtendableParcelable ep2 = new ExtendableParcelable();

      ep2.readFromParcel(parcel);

      MyExtLike extLike = ep2.ext.<MyExtLike>getParcelable(MyExtLike.class);
      mLog.assertTrue(extLike == null,
          "The extension type must be MyExt, so it has to fail even though MyExtLike has the same structure as MyExt.");

      MyExt actualExt = ep2.ext.getParcelable(MyExt.class);

      MyExt2 newExt2 = new MyExt2();
      newExt2.a = 79;
      newExt2.b = new MyExt();
      newExt2.b.a = 42;
      newExt2.b.b = "INNEWEXT";
      newExt2.c = "NEWEXT2";
      ep2.ext2.setParcelable(newExt2);
      checkContent(ep1, ext, ext2);
      checkContent(ep2, ext, newExt2);
    }
  }

  private void checkContent(ExtendableParcelable ep, MyExt ext, MyExt2 ext2)
      throws TestFailException {
    if (ep.a != 1 || !"a".equals(ep.b) || ep.c != 42L) {
      mLog.logAndThrow("ExtendableParcelable must be equal to the original object.");
    }

    MyExt actualExt = ep.ext.getParcelable(MyExt.class);
    MyExt2 actualExt2 = ep.ext2.getParcelable(MyExt2.class);
    if (!dumpMyExt(actualExt).equals(dumpMyExt(ext))) {
      mLog.logAndThrow("ext.getParcelable() is expected to be " + dumpMyExt(ext) + ", but "
          + dumpMyExt(actualExt));
    }

    if (!dumpMyExt2(actualExt2).equals(dumpMyExt2(ext2))) {
      mLog.logAndThrow("ext2.getParcelable() is expected to be " + dumpMyExt2(ext2) + ", but "
          + dumpMyExt2(actualExt2));
    }
  }

  public void runTests() throws TestFailException {
    checkExtension();
    checkStability();
  }
}
