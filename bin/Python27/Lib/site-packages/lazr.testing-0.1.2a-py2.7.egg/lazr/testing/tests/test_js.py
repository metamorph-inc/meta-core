import os
import re
import unittest
import doctest
import socket
import operator
import sys
from cStringIO import StringIO

from os.path import dirname

from mocker import ANY, ARGS, KWARGS, MockerTestCase

from zope.testing import testrunner

from lazr.testing.jstestdriver import (
    JsTestDriverTestCase, JsTestDriverLayer)


class JsTestDriverSelfTest(JsTestDriverTestCase):

    config_filename = os.path.abspath(os.path.join(os.path.dirname(__file__),
                                                   "js", "tests.conf"))


class JsTestDriverErrorTests(MockerTestCase):

    def setUp(self):
        super(JsTestDriverErrorTests, self).setUp()
        env_keys = [
            "JSTESTDRIVER",
            "JSTESTDRIVER_SERVER",
            "JSTESTDRIVER_PORT",
            "JSTESTDRIVER_CAPTURE_TIMEOUT",
            "JSTESTDRIVER_BROWSER"]

        def cleanup_non_existing_key(some_key):
            try:
                del os.environ[some_key]
            except KeyError:
                pass

        for key in env_keys:
            if key in os.environ:
                self.addCleanup(
                    operator.setitem, os.environ, key, os.environ[key])
            else:
                self.addCleanup(cleanup_non_existing_key, key)

    def test_binding(self):
        """
        If a specific port is requested, and a server is already started in the
        requested port, then the layer setup fails.
        """
        s = socket.socket()
        try:
            s.bind((socket.gethostbyname(""), 4225))
        except socket.error:
            s.close()
            s = None
            raise

        if "JSTESTDRIVER_SERVER" in os.environ:
            del os.environ["JSTESTDRIVER_SERVER"]
        os.environ["JSTESTDRIVER_PORT"] = "4225"
        try:
            try:
                JsTestDriverLayer.setUp()
            except ValueError, e:
                msg = str(e)
                self.assertIn(
                    "Failed to execute JsTestDriver server on port 4225", msg)
                self.assertIn(
                    "java.lang.RuntimeException: java.net.BindException: "
                    "Address already in use", msg)
            else:
                self.fail("ValueError not raised")
        finally:
            JsTestDriverLayer.tearDown()
            s.close()

    def test_connection_error(self):
        """
        An appropriate message is printed when the JsTestDriver client cannot
        connect to a server.
        """
        runner = getattr(testrunner, "run_internal", testrunner.run)
        root_directory = os.path.abspath(
            dirname(dirname(dirname(dirname(__file__)))))
        defaults = [
            "--path", root_directory,
            "-m", "lazr.testing.tests.test_js",
            "-t", "JsTestDriverSelfTest"]
        arguments = [
            "--no-progress"]
        os.environ["JSTESTDRIVER_SERVER"] = "http://localhost:4226"
        os.environ["JSTESTDRIVER_SELFTEST"] = "1"
        # Patch stdout to prevent spurious output
        test_stdout = StringIO()
        old_stdout = sys.stdout
        sys.stdout = test_stdout
        try:
            try:
                runner(defaults, arguments)
            except ValueError, e:
                msg = str(e)
                self.assertIn("Failed to execute JsTestDriver tests for", msg)
                self.assertIn(
                    "lazr/testing/tests/js/tests.conf "
                    "(http://localhost:4226)", msg)
                self.assertIn(
                    "java.lang.RuntimeException: java.net.ConnectException: "
                    "Connection refused", msg)
            else:
                self.fail("ValueError not raised")
        finally:
            sys.stdout = old_stdout

    def test_timeout_browser(self):
        """
        If we fail to capture a browser within the specified timeout, an
        appropriate message is shown. In order to test that, let's set the
        browser to be empty so that the JsTestDriver server doesn't capture a
        browser automatically, and set the timeout to a very short time so we
        don't wait for too long.
        """
        os.environ["JSTESTDRIVER_CAPTURE_TIMEOUT"] = "1"
        os.environ["JSTESTDRIVER_BROWSER"] = "not-a-browser"
        if "JSTESTDRIVER_SERVER" in os.environ:
            del os.environ["JSTESTDRIVER_SERVER"]
        os.environ["JSTESTDRIVER_PORT"] = "4225"

        try:
            try:
                JsTestDriverLayer.setUp()
            except ValueError, e:
                msg = str(e)
                self.assertIn("Failed to capture a browser in 1 seconds", msg)
            else:
                self.fail("ValueError not raised")
        finally:
            JsTestDriverLayer.tearDown()

    def mock_popen(self):
        """Replace subprocess.Popen and make it return a mock process.

        The mock process is returned.
        """
        mock_Popen = self.mocker.replace("subprocess.Popen")
        self.mock_proc = self.mocker.mock()
        mock_Popen(ARGS, KWARGS)
        self.mocker.result(self.mock_proc)
        return self.mock_proc

    def mock_builtin_open(self):
        """Replace built-in open and make it return a mock file.

        The mock file is returned.
        """
        mock_open = self.mocker.replace("__builtin__.open")
        mock_open(ANY)
        self.mock_file = self.mocker.mock()
        self.mocker.result(self.mock_file)
        return self.mock_file

    def test_wait_for_server_startup(self):
        """
        Even if we don't wait for the browser to be captured, we wait
        for the server to start up.
        """
        mock_proc = self.mock_popen()
        mock_file = self.mock_builtin_open()

        with self.mocker.order():
            mock_time = self.mocker.replace("time.time")
            # The first time is to initialize the start time.
            mock_time()
            start_time = 0
            self.mocker.result(start_time)

            # The second time is to check if the timeout is exceeded in
            # the while loop.
            mock_time()
            self.mocker.result(start_time)
            # Go one iteration of the while loop, reporting the server
            # has started.
            mock_proc.poll()
            self.mocker.result(None)
            mock_file.readline()
            self.mocker.result("INFO: Finished action run.")

            # The opened file is closed.
            mock_file.close()
            self.mocker.result(None)

            # Last check to make sure the server is running ok.
            mock_proc.poll()
            self.mocker.result(None)

        self.mocker.replay()

        os.environ["JSTESTDRIVER_BROWSER"] = ""
        if "JSTESTDRIVER_SERVER" in os.environ:
            del os.environ["JSTESTDRIVER_SERVER"]
        os.environ["JSTESTDRIVER_PORT"] = "4225"

        JsTestDriverLayer.setUp()
        self.assertEqual(
            "http://localhost:4225", os.environ["JSTESTDRIVER_SERVER"])

    def test_server_fail(self):
        """
        If we a poll of the process returns a non-None value while we
        are waiting, we report that server couldn't be started.
        """
        mock_proc = self.mock_popen()
        mock_file = self.mock_builtin_open()

        with self.mocker.order():
            mock_time = self.mocker.replace("time.time")
            # The first time is to initialize the start time.
            mock_time()
            start_time = 0
            self.mocker.result(start_time)

            # The second time is to check if the timeout is exceeded in
            # the while loop.
            mock_time()
            self.mocker.result(start_time)
            # Go one iteration of the while loop, reporting the server
            # is starting up.
            mock_proc.poll()
            self.mocker.result(None)
            mock_file.readline()
            self.mocker.result("INFO: still starting")

            # Go another iteration of the while loop, reporting the
            # server failed to start up.
            mock_time()
            self.mocker.result(start_time)
            mock_proc.poll()
            self.mocker.result(1)

            # The opened file is closed.
            mock_file.close()
            self.mocker.result(None)

        self.mocker.replay()

        if "JSTESTDRIVER_SERVER" in os.environ:
            del os.environ["JSTESTDRIVER_SERVER"]
        os.environ["JSTESTDRIVER_PORT"] = "4225"

        try:
            JsTestDriverLayer.setUp()
        except ValueError, e:
            msg = str(e)
            self.assertIn(
                "Failed to execute JsTestDriver server on port 4225", msg)
        else:
            self.fail("ValueError not raised")

    def test_server_timeout(self):
        """
        If we don't see that the server is started before the timeout, a
        ValueError is raised, even if the process is still running.
        """
        timeout = 1
        os.environ["JSTESTDRIVER_CAPTURE_TIMEOUT"] = "%s" % timeout
        os.environ["JSTESTDRIVER_BROWSER"] = ""
        if "JSTESTDRIVER_SERVER" in os.environ:
            del os.environ["JSTESTDRIVER_SERVER"]
        os.environ["JSTESTDRIVER_PORT"] = "4225"

        mock_proc = self.mock_popen()
        mock_file = self.mock_builtin_open()

        with self.mocker.order():
            mock_time = self.mocker.replace("time.time")
            # The first time is to initialize the start time.
            mock_time()
            start_time = 0
            self.mocker.result(start_time)

            # The second time is to check if the timeout is exceeded in
            # the while loop.
            mock_time()
            self.mocker.result(start_time)
            # Go one iteration of the while loop, reporting the server
            # is starting up.
            mock_proc.poll()
            self.mocker.result(None)
            mock_file.readline()
            self.mocker.result("INFO: still starting")

            # Trigger a timeout.
            mock_time()
            self.mocker.result(start_time + timeout + 1)

            # The opened file is closed.
            mock_file.close()
            self.mocker.result(None)

            # Last check whether the server is still running.
            mock_proc.poll()
            self.mocker.result(None)

            # Since the server is running, it gets terminated.
            mock_proc.terminate()
            self.mocker.result(None)
            mock_proc.wait()
            self.mocker.result(None)

        self.mocker.replay()

        try:
            JsTestDriverLayer.setUp()
        except ValueError, e:
            msg = str(e)
            self.assertIn(
                "Failed to execute JsTestDriver server in 1 seconds"
                " on port 4225", msg)
        else:
            self.fail("ValueError not raised")

    def tearDown(self):
        super(JsTestDriverErrorTests, self).tearDown()
        self.mocker.restore()


def test_suite():
    suite = unittest.TestSuite()

    if os.environ.get("JSTESTDRIVER_SELFTEST"):
        suite.addTests(unittest.makeSuite(JsTestDriverSelfTest))
    else:
        suite.addTests(unittest.makeSuite(JsTestDriverErrorTests))

        def setUp(test):
            test.globs["this_directory"] = os.path.abspath(dirname(__file__))
            test.globs["root_directory"] = dirname(
                dirname(dirname(test.globs["this_directory"])))

        from zope.testing import renormalizing
        checker = renormalizing.RENormalizing([
            (re.compile(r"\d+[.]\d\d\d seconds"), "N.NNN seconds"),
            (re.compile(r"\d+[.]\d\d\d s"), "N.NNN s"),
            (re.compile(r"\d+[.]\d\d\d{"), "N.NNN{"),
            (re.compile(r":\w+[\d\.]+ "), ":BrowserN.N.N.N "),
            (re.compile(r":\w+_\d+_\w+ "), ":BrowserN.N.N.N "),

            # omit traceback entries for jstestdriver.py or doctest.py from
            # output:
            (re.compile(r'^ +File "[^\n]*/lazr/testing/jstestdriver.py"'
                        r", [^\n]+\n[^\n]+\n",
                        re.MULTILINE),
             r"...\n"),
            ])
        suite.addTests(doctest.DocFileSuite(
            "jstestdriver.txt",
            setUp=setUp,
            checker=checker,
            optionflags=(doctest.ELLIPSIS |
                         doctest.NORMALIZE_WHITESPACE |
                         doctest.REPORT_NDIFF)))
    return suite
