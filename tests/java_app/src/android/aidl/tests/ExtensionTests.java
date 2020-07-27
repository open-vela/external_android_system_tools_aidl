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

import static org.junit.Assert.assertThat;
import static org.hamcrest.core.Is.is;
import static org.hamcrest.core.IsNull.nullValue;

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

import org.junit.Before;
import org.junit.Test;
import org.junit.runners.JUnit4;
import org.junit.runner.RunWith;

@RunWith(JUnit4.class)
public class ExtensionTests {
    private VintfExtendableParcelable vep;
    private VintfParcelable vp;

    private NonVintfExtendableParcelable sep;
    private NonVintfParcelable sp;

    private UnstableExtendableParcelable uep;
    private UnstableParcelable up;

    @Before
    public void setUp() {
        vep = new VintfExtendableParcelable();
        vp = new VintfParcelable();

        sep = new NonVintfExtendableParcelable();
        sp = new NonVintfParcelable();

        uep = new UnstableExtendableParcelable();
        up = new UnstableParcelable();
    }

    @Test
    public void testVintfParcelableHolderCanContainVintfParcelable() {
        assertThat(vep.ext.setParcelable(vp), is(true));
        assertThat(vep.ext.getParcelable(VintfParcelable.class), is(vp));
    }

    @Test
    public void testVintfParcelableHolderCannotContainNonVintfParcelable() {
        assertThat(vep.ext.setParcelable(sp), is(false));
        assertThat(vep.ext.getParcelable(VintfParcelable.class), is(nullValue()));
    }

    @Test
    public void testVintfParcelableHolderCannotContainUnstableParcelable() {
        assertThat(vep.ext.setParcelable(up), is(false));
        assertThat(vep.ext.getParcelable(UnstableParcelable.class), is(nullValue()));
    }

    @Test
    public void testStableParcelableHolderCanContainVintfParcelable() {
        assertThat(sep.ext.setParcelable(vp), is(true));
        assertThat(sep.ext.getParcelable(VintfParcelable.class), is(vp));
    }

    @Test
    public void testStableParcelableHolderCanContainNonVintfParcelable() {
        assertThat(sep.ext.setParcelable(sp), is(true));
        assertThat(sep.ext.getParcelable(NonVintfParcelable.class), is(sp));
    }

    @Test
    public void testStableParcelableHolderCanContainUnstableParcelable() {
        assertThat(sep.ext.setParcelable(up), is(true));
        assertThat(sep.ext.getParcelable(UnstableParcelable.class), is(up));
    }

    @Test
    public void testUnstableParcelableHolderCanContainVintfParcelable() {
        assertThat(uep.ext.setParcelable(vp), is(true));
        assertThat(uep.ext.getParcelable(VintfParcelable.class), is(vp));
    }

    @Test
    public void testUnstableParcelableHolderCanContainNonVintfParcelable() {
        assertThat(uep.ext.setParcelable(sp), is(true));
        assertThat(uep.ext.getParcelable(NonVintfParcelable.class), is(sp));
    }

    @Test
    public void testUnstableParcelableHolderCanContainUnstableParcelable() {
        assertThat(uep.ext.setParcelable(up), is(true));
        assertThat(uep.ext.getParcelable(UnstableParcelable.class), is(up));
    }

    @Test
    public void testReadWriteExtension() {
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
            assertThat("The extension type must be MyExt, so it has to fail even though " +
                    "MyExtLike has the same structure as MyExt.", extLike, is(nullValue()));

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
            assertThat("The extension type must be MyExt, so it has to fail even though " +
                "MyExtLike has the same structure as MyExt.", extLike, is(nullValue()));

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

    private void checkContent(ExtendableParcelable ep, MyExt ext, MyExt2 ext2) {
        assertThat(ep.a, is(1));
        assertThat(ep.b, is("a"));
        assertThat(ep.c, is(42L));

        MyExt actualExt = ep.ext.getParcelable(MyExt.class);
        assertThat(dumpMyExt(actualExt), is(dumpMyExt(ext)));

        MyExt2 actualExt2 = ep.ext2.getParcelable(MyExt2.class);
        assertThat(dumpMyExt2(actualExt2), is(dumpMyExt2(ext2)));
    }

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
}
