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

import static org.hamcrest.core.Is.is;
import static org.junit.Assert.assertThat;
import static org.junit.Assert.fail;

import android.aidl.tests.immutable.Bar;
import android.aidl.tests.immutable.Foo;
import android.os.Parcel;
import android.os.RemoteException;
import java.lang.UnsupportedOperationException;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public class ImmutableAnnotationTests {
  @Test
  public void testReadWriteImmutableParcelable() {
    Parcel parcel = Parcel.obtain();
    List<Bar> list = new ArrayList<Bar>();
    list.add(new Bar("aa"));
    Map<String, Bar> map = new HashMap<String, Bar>();
    map.put("key", new Bar("value"));
    Bar[] array = new Bar[3];
    array[0] = new Bar("zero");
    array[1] = new Bar("one");
    array[2] = new Bar("two");

    Foo foo = new Foo(7, new Bar("my"), list, map, array);
    foo.writeToParcel(parcel, 0);
    parcel.setDataPosition(0);
    Foo foo2 = Foo.CREATOR.createFromParcel(parcel);

    assertThat(foo.a, is(foo2.a));
    assertThat(foo.b.s, is(foo2.b.s));
    assertThat(foo.c.size(), is(foo.c.size()));
    assertThat(foo.c.get(0).s, is(foo2.c.get(0).s));
    assertThat(foo.c.size(), is(foo.c.size()));
    assertThat(foo.d.get("key").s, is(foo2.d.get("key").s));
    assertThat(foo.e.length, is(foo.e.length));

    for (int i = 0; i < foo.e.length; i++) {
      assertThat(foo.e[i].s, is(foo2.e[i].s));
    }
  }

  @Test
  public void testEveryFieldIsFinal() {
    for (Field f : Foo.class.getDeclaredFields()) {
      if (!Modifier.isFinal(f.getModifiers())) {
        fail(f.getName() + " should be final.");
      }
    }
  }

  @Test(expected = UnsupportedOperationException.class)
  public void testListIsUnmodifiable() {
    Foo foo =
        new Foo(7, new Bar("my"), new ArrayList<Bar>(), new HashMap<String, Bar>(), new Bar[5]);
    foo.c.add(new Bar("hi"));
    // It is supposed to fail.
    fail("A List in an immutable parcelable isn't modifiable.");
  }

  @Test(expected = UnsupportedOperationException.class)
  public void testMapIsUnmodifiable() {
    Foo foo =
        new Foo(7, new Bar("my"), new ArrayList<Bar>(), new HashMap<String, Bar>(), new Bar[5]);
    foo.d.put("key", new Bar("value"));
    // It is supposed to fail.
    fail("A Map in an immutable parcelable isn't modifiable.");
  }
}
