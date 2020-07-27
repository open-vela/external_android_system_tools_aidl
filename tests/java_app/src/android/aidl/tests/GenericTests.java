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

import android.aidl.tests.generic.Baz;
import android.aidl.tests.generic.IFaz;
import android.aidl.tests.generic.Pair;
import android.os.IBinder;
import android.os.RemoteException;

import org.junit.Test;
import org.junit.runners.JUnit4;
import org.junit.runner.RunWith;

@RunWith(JUnit4.class)
public class GenericTests {

    @Test
    public void testGeneric() throws RemoteException {
        IFaz.Stub ifaz = new IFaz.Stub() {
            public Pair<Integer, String> getPair() {
                Pair<Integer, String> ret = new Pair<Integer, String>();
                ret.mFirst = 15;
                ret.mSecond = "My";
                return ret;
            }
            public Pair<Baz, Baz> getPair2() {
                Pair<Baz, Baz> ret = new Pair<Baz, Baz>();
                ret.mFirst = new Baz();
                ret.mSecond = new Baz();
                return ret;
            }
        };

        IFaz service = IFaz.Stub.asInterface(ifaz);
        assertThat(service.getPair().mFirst, is(15));
        assertThat(service.getPair().mSecond, is("My"));
    }
}
