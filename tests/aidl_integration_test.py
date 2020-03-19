#!/usr/bin/env python

import pipes
import subprocess
import unittest

BITNESS_32 = ("", "32")
BITNESS_64 = ("64", "64")

JAVA_OUTPUT_READER_FOR_BITNESS = '/data/nativetest%s/aidl_test_sentinel_searcher/aidl_test_sentinel_searcher%s'
NATIVE_TEST_CLIENT_FOR_BITNESS = ' /data/nativetest%s/aidl_test_client/aidl_test_client%s'
NATIVE_TEST_SERVICE_FOR_BITNESS = ' /data/nativetest%s/aidl_test_service/aidl_test_service%s'

JAVA_CLIENT_TIMEOUT_SECONDS = 30
JAVA_LOG_FILE = '/data/data/android.aidl.tests/files/test-client.log'
JAVA_SUCCESS_SENTINEL = '>>> Java Client Success <<<'
JAVA_FAILURE_SENTINEL = '>>> Java Client Failure <<<'

class TestFail(Exception):
    """Raised on test failures."""
    pass

def pretty_bitness(bitness):
    """Returns a human readable version of bitness, corresponding to BITNESS_* variable"""
    return bitness[-1]

class ShellResult(object):
    """Represents the result of running a shell command."""

    def __init__(self, exit_status, stdout, stderr):
        """Construct an instance.

        Args:
            exit_status: integer exit code of shell command
            stdout: string stdout of shell command
            stderr: string stderr of shell command
        """
        self.stdout = stdout
        self.stderr = stderr
        self.exit_status = exit_status

    def printable_string(self):
        """Get a string we could print to the logs and understand."""
        output = []
        output.append('stdout:')
        for line in self.stdout.splitlines():
            output.append('  > %s' % line)
        output.append('stderr:')
        for line in self.stderr.splitlines():
            output.append('  > %s' % line)
        return '\n'.join(output)


class AdbHost(object):
    """Represents a device connected via ADB."""

    def run(self, command, background=False, ignore_status=False):
        """Run a command on the device via adb shell.

        Args:
            command: string containing a shell command to run.
            background: True iff we should run this command in the background.
            ignore_status: True iff we should ignore the command's exit code.

        Returns:
            instance of ShellResult.

        Raises:
            subprocess.CalledProcessError on command exit != 0.
        """
        if background:
            command = '( %s ) </dev/null >/dev/null 2>&1 &' % command
        return self.adb('shell %s' % pipes.quote(command),
                        ignore_status=ignore_status)

    def adb(self, command, ignore_status=False):
        """Run an ADB command (e.g. `adb sync`).

        Args:
            command: string containing command to run
            ignore_status: True iff we should ignore the command's exit code.

        Returns:
            instance of ShellResult.

        Raises:
            subprocess.CalledProcessError on command exit != 0.
        """
        command = 'adb %s' % command
        p = subprocess.Popen(command, shell=True, close_fds=True,
                             stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                             universal_newlines=True)
        stdout, stderr = p.communicate()
        if not ignore_status and p.returncode:
            raise subprocess.CalledProcessError(p.returncode, command)
        return ShellResult(p.returncode, stdout, stderr)

class NativeServer:
    def __init__(self, host, bitness):
        self.name = "%s_bit_native_server" % pretty_bitness(bitness)
        self.host = host
        self.binary = NATIVE_TEST_SERVICE_FOR_BITNESS % bitness
    def cleanup(self):
        self.host.run('killall %s' % self.binary, ignore_status=True)
    def run(self):
        return self.host.run(self.binary, background=True)

class NativeClient:
    def __init__(self, host, bitness):
        self.name = "%s_bit_native_client" % pretty_bitness(bitness)
        self.host = host
        self.binary = NATIVE_TEST_CLIENT_FOR_BITNESS % bitness
    def cleanup(self):
        self.host.run('killall %s' % self.binary, ignore_status=True)
    def run(self):
        result = self.host.run(self.binary, ignore_status=True)
        if result.exit_status:
            print(result.printable_string())
            raise TestFail('%s returned status code %d' %
                           (self.binary, result.exit_status))

class JavaClient:
    def __init__(self, host, native_bitness):
        self.name = "java_client"
        self.host = host
        self.native_bitness = native_bitness
    def cleanup(self):
        host.run('setenforce 1')
        self.host.run('rm -f %s' % JAVA_LOG_FILE, ignore_status=True)
        self.host.run('killall android.aidl.tests', ignore_status=True)
    def run(self):
        host.run('setenforce 0') # Java app needs selinux off
        JAVA_OUTPUT_READER = JAVA_OUTPUT_READER_FOR_BITNESS % self.native_bitness
        self.host.run('am start -S -a android.intent.action.MAIN '
                      '-n android.aidl.tests/.TestServiceClient '
                      '--es sentinel.success "%s" '
                      '--es sentinel.failure "%s"' %
                      (JAVA_SUCCESS_SENTINEL, JAVA_FAILURE_SENTINEL))
        result = self.host.run('%s %d %s "%s" "%s"' %
                               (JAVA_OUTPUT_READER, JAVA_CLIENT_TIMEOUT_SECONDS,
                                JAVA_LOG_FILE, JAVA_SUCCESS_SENTINEL,
                                JAVA_FAILURE_SENTINEL),
                               ignore_status=True)
        if result.exit_status:
            print(result.printable_string())
            raise TestFail('Java client did not complete successfully.')

def supported_bitnesses(host):
    bitnesses = []
    if host.run('ls /data/nativetest/', ignore_status=True).exit_status == 0:
        bitnesses = [BITNESS_32]
    if host.run('ls /data/nativetest64/', ignore_status=True).exit_status == 0:
        bitnesses += [BITNESS_64]
    return bitnesses

# tests added dynamically below
class TestAidl(unittest.TestCase):
    pass

def make_test(client, server):
    def test(self):
        try:
            client.cleanup()
            server.cleanup()
            server.run()
            client.run()
        finally:
            client.cleanup()
            server.cleanup()
    return test

if __name__ == '__main__':
    host = AdbHost()
    bitnesses = supported_bitnesses(host)
    if len(bitnesses) == 0:
        print("No clients installed")
        exit(1)

    clients = []
    servers = []

    for bitness in bitnesses:
        clients += [NativeClient(host, bitness)]
        servers += [NativeServer(host, bitness)]

    # Java only supports one bitness, but needs to run a native binary
    # to process its results
    clients += [JavaClient(host, bitnesses[-1])]

    for client in clients:
        for server in servers:
            test_name = 'test_%s_to_%s' % (client.name, server.name)
            test = make_test(client, server)
            setattr(TestAidl, test_name, test)

    suite = unittest.TestLoader().loadTestsFromTestCase(TestAidl)
    unittest.TextTestRunner(verbosity=2).run(suite)
