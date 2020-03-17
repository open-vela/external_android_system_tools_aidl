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

def run_test(host):
    """Body of the test.

    Args:
        host: AdbHost object to run tests on
    """

    if host.run('ls /data/nativetest64', ignore_status=True).exit_status:
        bitness = BITNESS_32
    else:
        bitness = BITNESS_64

    JAVA_OUTPUT_READER = JAVA_OUTPUT_READER_FOR_BITNESS % bitness
    NATIVE_TEST_CLIENT = NATIVE_TEST_CLIENT_FOR_BITNESS % bitness
    NATIVE_TEST_SERVICE = NATIVE_TEST_SERVICE_FOR_BITNESS % bitness

    # Kill any previous test context
    host.run('rm -f %s' % JAVA_LOG_FILE, ignore_status=True)
    host.run('killall %s' % NATIVE_TEST_SERVICE, ignore_status=True)

    # Start up a native server
    host.run(NATIVE_TEST_SERVICE, background=True)

    # Start up clients
    if True:
        host.run('killall %s' % NATIVE_TEST_CLIENT, ignore_status=True)
        result = host.run(NATIVE_TEST_CLIENT, ignore_status=True)
        if result.exit_status:
            print(result.printable_string())
            raise TestFail('%s returned status code %d' %
                           (NATIVE_TEST_CLIENT, result.exit_status))

    if True:
        host.run('am start -S -a android.intent.action.MAIN '
                 '-n android.aidl.tests/.TestServiceClient '
                 '--es sentinel.success "%s" '
                 '--es sentinel.failure "%s"' %
                 (JAVA_SUCCESS_SENTINEL, JAVA_FAILURE_SENTINEL))
        result = host.run('%s %d %s "%s" "%s"' %
                          (JAVA_OUTPUT_READER, JAVA_CLIENT_TIMEOUT_SECONDS,
                           JAVA_LOG_FILE, JAVA_SUCCESS_SENTINEL,
                           JAVA_FAILURE_SENTINEL),
                          ignore_status=True)
        if result.exit_status:
            print(result.printable_string())
            raise TestFail('Java client did not complete successfully.')

    host.run('killall %s' % NATIVE_TEST_SERVICE, ignore_status=True)
    host.run('killall android.aidl.tests', ignore_status=True)

# Simple wrapper to call old test entry point. Could be improved by making
# separate cases for testing native/java etc.
class TestAidl(unittest.TestCase):
    def test_native_and_java_integration_tests(self):
        host = AdbHost()
        try:
            # Tragically, SELinux interferes with our testing
            host.run('setenforce 0')
            run_test(host)
        finally:
            host.run('setenforce 1')

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(TestAidl)
    unittest.TextTestRunner(verbosity=2).run(suite)
