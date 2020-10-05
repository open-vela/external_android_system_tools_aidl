/*
 * Copyright (C) 2019, The Android Open Source Project
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

import android.aidl.tests.map.Bar;
import android.aidl.tests.map.Foo;
import android.aidl.tests.map.IEmpty;
import android.os.Parcel;
import java.util.HashMap;

import org.junit.Test;
import org.junit.runners.JUnit4;
import org.junit.runner.RunWith;

@RunWith(JUnit4.class)
public class MapTests {

    @Test
    public void testWriteAndThenReadMaps() {
        Parcel parcel = Parcel.obtain();

        // Write
        IEmpty intf = new IEmpty.Stub() {};
        Foo foo = new Foo();
        Bar bar = new Bar();
        bar.a = 42;
        bar.b = "Bar";
        foo.barMap = new HashMap<>();
        foo.barMap.put("Foo", bar);

        foo.stringMap = new HashMap<>();
        foo.stringMap.put("Foo", "Bar");

        foo.interfaceMap = new HashMap<>();
        foo.interfaceMap.put("Foo", intf);

        foo.ibinderMap = new HashMap<>();
        foo.ibinderMap.put("Foo", intf.asBinder());

        foo.writeToParcel(parcel, 0);

        // And then read
        parcel.setDataPosition(0);

        Foo readFoo = new Foo();
        readFoo.readFromParcel(parcel);
        assertThat(readFoo.barMap.containsKey("Foo"), is(true));
        assertThat(readFoo.barMap.size(), is(1));

        Bar readBar = readFoo.barMap.get("Foo");
        assertThat(readBar.a, is(42));
        assertThat(readBar.b, is("Bar"));

        assertThat(readFoo.stringMap.size(), is(1));
        assertThat(readFoo.stringMap.get("Foo"), is("Bar"));

        assertThat(readFoo.interfaceMap.size(), is(1));
        assertThat(readFoo.interfaceMap.get("Foo"), is(intf));

        assertThat(readFoo.ibinderMap.size(), is(1));
        assertThat(readFoo.ibinderMap.get("Foo"), is(intf.asBinder()));
    }
}
