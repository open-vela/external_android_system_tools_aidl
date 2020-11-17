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
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertThat;
import static org.junit.Assert.assertTrue;

import android.aidl.versioned.tests.IFooInterface;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.ServiceManager;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public class TestVersionedInterface {
    private IFooInterface service;

    @Before
    public void setUp() {
        IBinder binder = ServiceManager.getService(IFooInterface.class.getName());
        assertNotNull(binder);
        service = IFooInterface.Stub.asInterface(binder);
        assertNotNull(service);
    }

    @Test
    public void testGetInterfaceVersion() throws RemoteException {
        assertThat(service.getInterfaceVersion(), is(1));
    }

    @Test
    public void testGetInterfaceHash() throws RemoteException {
        assertThat(service.getInterfaceHash(), is("fcd4f9c806cbc8af3694d569fd1de1ecc8cf7d22"));
    }

    @Rule public ExpectedException expectedException = ExpectedException.none();

    @Test
    public void testUnimplementedMethodTriggersException() throws RemoteException {
      expectedException.expect(RemoteException.class);
      expectedException.expectMessage("Method bar is unimplemented.");

      service.bar();
    }
}
