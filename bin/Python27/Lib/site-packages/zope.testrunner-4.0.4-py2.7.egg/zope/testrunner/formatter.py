##############################################################################
#
# Copyright (c) 2004-2008 Zope Foundation and Contributors.
# All Rights Reserved.
#
# This software is subject to the provisions of the Zope Public License,
# Version 2.1 (ZPL).  A copy of the ZPL should accompany this distribution.
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY AND ALL EXPRESS OR IMPLIED
# WARRANTIES ARE DISCLAIMED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF TITLE, MERCHANTABILITY, AGAINST INFRINGEMENT, AND FITNESS
# FOR A PARTICULAR PURPOSE.
#
##############################################################################
"""Output formatting.
"""

import doctest
import os
import sys
import re
import tempfile
import traceback

from datetime import datetime, timedelta

from zope.testrunner.exceptions import DocTestFailureException

doctest_template = """
File "%s", line %s, in %s

%s
Want:
%s
Got:
%s
"""


class OutputFormatter(object):
    """Test runner output formatter."""

    # Implementation note: be careful about printing stuff to sys.stderr.
    # It is used for interprocess communication between the parent and the
    # child test runner, when you run some test layers in a subprocess.
    # resume_layer() reasigns sys.stderr for this reason, but be careful
    # and don't store the original one in __init__ or something.

    max_width = 80

    def __init__(self, options):
        self.options = options
        self.last_width = 0
        self.compute_max_width()

    progress = property(lambda self: self.options.progress)
    verbose = property(lambda self: self.options.verbose)
    in_subprocess = property(
        lambda self: self.options.resume_layer is not None and
                     self.options.processes > 1)

    def compute_max_width(self):
        """Try to determine the terminal width."""
        # Note that doing this every time is more test friendly.
        self.max_width = tigetnum('cols', self.max_width)

    def getShortDescription(self, test, room):
        """Return a description of a test that fits in ``room`` characters."""
        room -= 1
        s = str(test)
        if len(s) > room:
            pos = s.find(" (")
            if pos >= 0:
                w = room - (pos + 5)
                if w < 1:
                    # first portion (test method name) is too long
                    s = s[:room-3] + "..."
                else:
                    pre = s[:pos+2]
                    post = s[-w:]
                    s = "%s...%s" % (pre, post)
            else:
                w = room - 4
                s = '... ' + s[-w:]

        return ' ' + s[:room]

    def info(self, message):
        """Print an informative message."""
        print message

    def info_suboptimal(self, message):
        """Print an informative message about losing some of the features.

        For example, when you run some tests in a subprocess, you lose the
        ability to use the debugger.
        """
        print message

    def error(self, message):
        """Report an error."""
        print message

    def error_with_banner(self, message):
        """Report an error with a big ASCII banner."""
        print
        print '*'*70
        self.error(message)
        print '*'*70
        print

    def profiler_stats(self, stats):
        """Report profiler stats."""
        stats.print_stats(50)

    def import_errors(self, import_errors):
        """Report test-module import errors (if any)."""
        if import_errors:
            print "Test-module import failures:"
            for error in import_errors:
                self.print_traceback("Module: %s\n" % error.module,
                                     error.exc_info),
            print

    def tests_with_errors(self, errors):
        """Report names of tests with errors (if any)."""
        if errors:
            print
            print "Tests with errors:"
            for test, exc_info in errors:
                print "  ", test

    def tests_with_failures(self, failures):
        """Report names of tests with failures (if any)."""
        if failures:
            print
            print "Tests with failures:"
            for test, exc_info in failures:
                print "  ", test

    def modules_with_import_problems(self, import_errors):
        """Report names of modules with import problems (if any)."""
        if import_errors:
            print
            print "Test-modules with import problems:"
            for test in import_errors:
                print "  " + test.module

    def format_seconds(self, n_seconds):
        """Format a time in seconds."""
        if n_seconds >= 60:
            n_minutes, n_seconds = divmod(n_seconds, 60)
            return "%d minutes %.3f seconds" % (n_minutes, n_seconds)
        else:
            return "%.3f seconds" % n_seconds

    def format_seconds_short(self, n_seconds):
        """Format a time in seconds (short version)."""
        return "%.3f s" % n_seconds

    def summary(self, n_tests, n_failures, n_errors, n_seconds):
        """Summarize the results of a single test layer."""
        print ("  Ran %s tests with %s failures and %s errors in %s."
               % (n_tests, n_failures, n_errors,
                  self.format_seconds(n_seconds)))

    def totals(self, n_tests, n_failures, n_errors, n_seconds):
        """Summarize the results of all layers."""
        print ("Total: %s tests, %s failures, %s errors in %s."
               % (n_tests, n_failures, n_errors,
                  self.format_seconds(n_seconds)))

    def list_of_tests(self, tests, layer_name):
        """Report a list of test names."""
        print "Listing %s tests:" % layer_name
        for test in tests:
            print ' ', test

    def garbage(self, garbage):
        """Report garbage generated by tests."""
        if garbage:
            print "Tests generated new (%d) garbage:" % len(garbage)
            print garbage

    def test_garbage(self, test, garbage):
        """Report garbage generated by a test."""
        if garbage:
            print "The following test left garbage:"
            print test
            print garbage

    def test_threads(self, test, new_threads):
        """Report threads left behind by a test."""
        if new_threads:
            print "The following test left new threads behind:"
            print test
            print "New thread(s):", new_threads

    def refcounts(self, rc, prev):
        """Report a change in reference counts."""
        print "  sys refcount=%-8d change=%-6d" % (rc, rc - prev)

    def detailed_refcounts(self, track, rc, prev):
        """Report a change in reference counts, with extra detail."""
        print ("  sum detail refcount=%-8d"
               " sys refcount=%-8d"
               " change=%-6d"
               % (track.n, rc, rc - prev))
        track.output()

    def start_set_up(self, layer_name):
        """Report that we're setting up a layer.

        The next output operation should be stop_set_up().
        """
        print "  Set up %s" % layer_name,
        sys.stdout.flush()

    def stop_set_up(self, seconds):
        """Report that we've set up a layer.

        Should be called right after start_set_up().
        """
        print "in %s." % self.format_seconds(seconds)

    def start_tear_down(self, layer_name):
        """Report that we're tearing down a layer.

        The next output operation should be stop_tear_down() or
        tear_down_not_supported().
        """
        print "  Tear down %s" % layer_name,
        sys.stdout.flush()

    def stop_tear_down(self, seconds):
        """Report that we've tore down a layer.

        Should be called right after start_tear_down().
        """
        print "in %s." % self.format_seconds(seconds)

    def tear_down_not_supported(self):
        """Report that we could not tear down a layer.

        Should be called right after start_tear_down().
        """
        print "... not supported"

    def start_test(self, test, tests_run, total_tests):
        """Report that we're about to run a test.

        The next output operation should be test_success(), test_error(), or
        test_failure().
        """
        self.test_width = 0
        if self.progress:
            if self.last_width:
                sys.stdout.write('\r' + (' ' * self.last_width) + '\r')

            s = "    %d/%d (%.1f%%)" % (tests_run, total_tests,
                                        tests_run * 100.0 / total_tests)
            sys.stdout.write(s)
            self.test_width += len(s)
            if self.verbose == 1:
                room = self.max_width - self.test_width - 1
                s = self.getShortDescription(test, room)
                sys.stdout.write(s)
                self.test_width += len(s)

        elif self.verbose == 1:
            sys.stdout.write('.' * test.countTestCases())

        elif self.in_subprocess:
            sys.stdout.write('.' * test.countTestCases())
            # Give the parent process a new line so it sees the progress
            # in a timely manner.
            sys.stdout.write('\n')

        if self.verbose > 1:
            s = str(test)
            sys.stdout.write(' ')
            sys.stdout.write(s)
            self.test_width += len(s) + 1

        sys.stdout.flush()

    def test_success(self, test, seconds):
        """Report that a test was successful.

        Should be called right after start_test().

        The next output operation should be stop_test().
        """
        if self.verbose > 2:
            s = " (%s)" % self.format_seconds_short(seconds)
            sys.stdout.write(s)
            self.test_width += len(s) + 1

    def test_error(self, test, seconds, exc_info):
        """Report that an error occurred while running a test.

        Should be called right after start_test().

        The next output operation should be stop_test().
        """
        if self.verbose > 2:
            print " (%s)" % self.format_seconds_short(seconds)
        print
        self.print_traceback("Error in test %s" % test, exc_info)
        self.test_width = self.last_width = 0

    def test_failure(self, test, seconds, exc_info):
        """Report that a test failed.

        Should be called right after start_test().

        The next output operation should be stop_test().
        """
        if self.verbose > 2:
            print " (%s)" % self.format_seconds_short(seconds)
        print
        self.print_traceback("Failure in test %s" % test, exc_info)
        self.test_width = self.last_width = 0

    def print_traceback(self, msg, exc_info):
        """Report an error with a traceback."""
        print
        print msg
        print self.format_traceback(exc_info)

    def format_traceback(self, exc_info):
        """Format the traceback."""
        v = exc_info[1]
        if isinstance(v, DocTestFailureException):
            tb = v.args[0]
        elif isinstance(v, doctest.DocTestFailure):
            tb = doctest_template % (
                v.test.filename,
                v.test.lineno + v.example.lineno + 1,
                v.test.name,
                v.example.source,
                v.example.want,
                v.got,
                )
        else:
            tb = "".join(traceback.format_exception(*exc_info))
        return tb

    def stop_test(self, test):
        """Clean up the output state after a test."""
        if self.progress:
            self.last_width = self.test_width
        elif self.verbose > 1:
            print
        sys.stdout.flush()

    def stop_tests(self):
        """Clean up the output state after a collection of tests."""
        if self.progress and self.last_width:
            sys.stdout.write('\r' + (' ' * self.last_width) + '\r')
        if self.verbose == 1 or self.progress:
            print


def tigetnum(attr, default=None):
    """Return a value from the terminfo database.

    Terminfo is used on Unix-like systems to report various terminal attributes
    (such as width, height or the number of supported colors).

    Returns ``default`` when the ``curses`` module is not available, or when
    sys.stdout is not a terminal.
    """
    try:
        import curses
    except ImportError:
        # avoid reimporting a broken module in python 2.3
        sys.modules['curses'] = None
    else:
        # If sys.stdout is not a real file object (e.g. in unit tests that 
        # use various wrappers), you get an error, different depending on
        # Python version:
        if sys.version_info >= (3,):
            import io
            expected_exceptions = (curses.error, TypeError, io.UnsupportedOperation)
        else:
            expected_exceptions = (curses.error, TypeError)
        try:
            curses.setupterm()
        except expected_exceptions:
            # You get curses.error when $TERM is set to an unknown name
            pass
        else:
            return curses.tigetnum(attr)
    return default


def terminal_has_colors():
    """Determine whether the terminal supports colors.

    Some terminals (e.g. the emacs built-in one) don't.
    """
    return tigetnum('colors', -1) >= 8


class ColorfulOutputFormatter(OutputFormatter):
    """Output formatter that uses ANSI color codes.

    Like syntax highlighting in your text editor, colorizing
    test failures helps the developer.
    """

    # These colors are carefully chosen to have enough contrast
    # on terminals with both black and white background.
    colorscheme = {'normal': 'normal',
                   'default': 'default',
                   'info': 'normal',
                   'suboptimal-behaviour': 'magenta',
                   'error': 'brightred',
                   'number': 'green',
                   'slow-test': 'brightmagenta',
                   'ok-number': 'green',
                   'error-number': 'brightred',
                   'filename': 'lightblue',
                   'lineno': 'lightred',
                   'testname': 'lightcyan',
                   'failed-example': 'cyan',
                   'expected-output': 'green',
                   'actual-output': 'red',
                   'character-diffs': 'magenta',
                   'diff-chunk': 'magenta',
                   'exception': 'red'}

    # Map prefix character to color in diff output.  This handles ndiff and
    # udiff correctly, but not cdiff.  In cdiff we ought to highlight '!' as
    # expected-output until we see a '-', then highlight '!' as actual-output,
    # until we see a '*', then switch back to highlighting '!' as
    # expected-output.  Nevertheless, coloried cdiffs are reasonably readable,
    # so I'm not going to fix this.
    #   -- mgedmin
    diff_color = {'-': 'expected-output',
                  '+': 'actual-output',
                  '?': 'character-diffs',
                  '@': 'diff-chunk',
                  '*': 'diff-chunk',
                  '!': 'actual-output',}

    prefixes = [('dark', '0;'),
                ('light', '1;'),
                ('bright', '1;'),
                ('bold', '1;'),]

    colorcodes = {'default': 0, 'normal': 0,
                  'black': 30,
                  'red': 31,
                  'green': 32,
                  'brown': 33, 'yellow': 33,
                  'blue': 34,
                  'magenta': 35,
                  'cyan': 36,
                  'grey': 37, 'gray': 37, 'white': 37}

    slow_test_threshold = 10.0 # seconds

    def color_code(self, color):
        """Convert a color description (e.g. 'lightgray') to a terminal code."""
        prefix_code = ''
        for prefix, code in self.prefixes:
            if color.startswith(prefix):
                color = color[len(prefix):]
                prefix_code = code
                break
        color_code = self.colorcodes[color]
        return '\033[%s%sm' % (prefix_code, color_code)

    def color(self, what):
        """Pick a named color from the color scheme"""
        return self.color_code(self.colorscheme[what])

    def colorize(self, what, message, normal='normal'):
        """Wrap message in color."""
        return self.color(what) + message + self.color(normal)

    def error_count_color(self, n):
        """Choose a color for the number of errors."""
        if n:
            return self.color('error-number')
        else:
            return self.color('ok-number')

    def info(self, message):
        """Print an informative message."""
        print self.colorize('info', message)

    def info_suboptimal(self, message):
        """Print an informative message about losing some of the features.

        For example, when you run some tests in a subprocess, you lose the
        ability to use the debugger.
        """
        print self.colorize('suboptimal-behaviour', message)

    def error(self, message):
        """Report an error."""
        print self.colorize('error', message)

    def error_with_banner(self, message):
        """Report an error with a big ASCII banner."""
        print
        print self.colorize('error', '*'*70)
        self.error(message)
        print self.colorize('error', '*'*70)
        print

    def tear_down_not_supported(self):
        """Report that we could not tear down a layer.

        Should be called right after start_tear_down().
        """
        print "...", self.colorize('suboptimal-behaviour', "not supported")

    def format_seconds(self, n_seconds, normal='normal'):
        """Format a time in seconds."""
        if n_seconds >= 60:
            n_minutes, n_seconds = divmod(n_seconds, 60)
            return "%s minutes %s seconds" % (
                        self.colorize('number', '%d' % n_minutes, normal),
                        self.colorize('number', '%.3f' % n_seconds, normal))
        else:
            return "%s seconds" % (
                        self.colorize('number', '%.3f' % n_seconds, normal))

    def format_seconds_short(self, n_seconds):
        """Format a time in seconds (short version)."""
        if n_seconds >= self.slow_test_threshold:
            color = 'slow-test'
        else:
            color = 'number'
        return self.colorize(color, "%.3f s" % n_seconds)

    def summary(self, n_tests, n_failures, n_errors, n_seconds):
        """Summarize the results."""
        sys.stdout.writelines([
            self.color('info'), '  Ran ',
            self.color('number'), str(n_tests),
            self.color('info'), ' tests with ',
            self.error_count_color(n_failures), str(n_failures),
            self.color('info'), ' failures and ',
            self.error_count_color(n_errors), str(n_errors),
            self.color('info'), ' errors in ',
            self.format_seconds(n_seconds, 'info'), '.',
            self.color('normal'), '\n'])

    def totals(self, n_tests, n_failures, n_errors, n_seconds):
        """Report totals (number of tests, failures, and errors)."""
        sys.stdout.writelines([
            self.color('info'), 'Total: ',
            self.color('number'), str(n_tests),
            self.color('info'), ' tests, ',
            self.error_count_color(n_failures), str(n_failures),
            self.color('info'), ' failures, ',
            self.error_count_color(n_errors), str(n_errors),
            self.color('info'), ' errors in ',
            self.format_seconds(n_seconds, 'info'), '.',
            self.color('normal'), '\n'])

    def print_traceback(self, msg, exc_info):
        """Report an error with a traceback."""
        print
        print self.colorize('error', msg)
        v = exc_info[1]
        if isinstance(v, DocTestFailureException):
            self.print_doctest_failure(v.args[0])
        elif isinstance(v, doctest.DocTestFailure):
            # I don't think these are ever used... -- mgedmin
            tb = self.format_traceback(exc_info)
            print tb
        else:
            tb = self.format_traceback(exc_info)
            self.print_colorized_traceback(tb)

    def print_doctest_failure(self, formatted_failure):
        """Report a doctest failure.

        ``formatted_failure`` is a string -- that's what
        DocTestSuite/DocFileSuite gives us.
        """
        color_of_indented_text = 'normal'
        colorize_diff = False
        for line in formatted_failure.splitlines():
            if line.startswith('File '):
                m = re.match(r'File "(.*)", line (\d*), in (.*)$', line)
                if m:
                    filename, lineno, test = m.groups()
                    sys.stdout.writelines([
                        self.color('normal'), 'File "',
                        self.color('filename'), filename,
                        self.color('normal'), '", line ',
                        self.color('lineno'), lineno,
                        self.color('normal'), ', in ',
                        self.color('testname'), test,
                        self.color('normal'), '\n'])
                else:
                    print line
            elif line.startswith('    ') or line.strip() == '':
                if colorize_diff and len(line) > 4:
                    color = self.diff_color.get(line[4], color_of_indented_text)
                    print self.colorize(color, line)
                else:
                    if line.strip() != '':
                        print self.colorize(color_of_indented_text, line)
                    else:
                        print line
            else:
                colorize_diff = False
                if line.startswith('Failed example'):
                    color_of_indented_text = 'failed-example'
                elif line.startswith('Expected:'):
                    color_of_indented_text = 'expected-output'
                elif line.startswith('Got:'):
                    color_of_indented_text = 'actual-output'
                elif line.startswith('Exception raised:'):
                    color_of_indented_text = 'exception'
                elif line.startswith('Differences '):
                    color_of_indented_text = 'normal'
                    colorize_diff = True
                else:
                    color_of_indented_text = 'normal'
                print line
        print

    def print_colorized_traceback(self, formatted_traceback):
        """Report a test failure.

        ``formatted_traceback`` is a string.
        """
        for line in formatted_traceback.splitlines():
            if line.startswith('  File'):
                m = re.match(r'  File "(.*)", line (\d*), in (.*)$', line)
                if m:
                    filename, lineno, test = m.groups()
                    sys.stdout.writelines([
                        self.color('normal'), '  File "',
                        self.color('filename'), filename,
                        self.color('normal'), '", line ',
                        self.color('lineno'), lineno,
                        self.color('normal'), ', in ',
                        self.color('testname'), test,
                        self.color('normal'), '\n'])
                else:
                    print line
            elif line.startswith('    '):
                print self.colorize('failed-example', line)
            elif line.startswith('Traceback (most recent call last)'):
                print line
            else:
                print self.colorize('exception', line)
        print


class FakeTest(object):
    """A fake test object that only has an id."""

    failureException = None

    def __init__(self, test_id):
        self._id = test_id

    def id(self):
        return self._id


# Conditional imports, we don't want zope.testrunner to have a hard dependency on
# subunit.
try:
    import subunit
    from subunit.iso8601 import Utc
except ImportError:
    subunit = None


# testtools is a hard dependency of subunit itself, guarding separately for
# richer error messages.
try:
    from testtools import content
except ImportError:
    content = None


class SubunitOutputFormatter(object):
    """A subunit output formatter.

    This output formatter generates subunit compatible output (see
    https://launchpad.net/subunit). Subunit output is essentially a stream of
    results of unit tests. In this formatter, non-test events (such as layer
    set up) are encoded as specially tagged tests and summary-generating
    methods (such as modules_with_import_problems) deliberately do nothing.

    In particular, for a layer 'foo', the fake tests related to layer set up
    and tear down are tagged with 'zope:layer' and are called 'foo:setUp' and
    'foo:tearDown'. Any tests within layer 'foo' are tagged with
    'zope:layer:foo'.

    Note that all tags specific to this formatter begin with 'zope:'
    """

    # subunit output is designed for computers, so displaying a progress bar
    # isn't helpful.
    progress = False
    verbose = property(lambda self: self.options.verbose)

    TAG_INFO_SUBOPTIMAL = 'zope:info_suboptimal'
    TAG_ERROR_WITH_BANNER = 'zope:error_with_banner'
    TAG_LAYER = 'zope:layer'
    TAG_IMPORT_ERROR = 'zope:import_error'
    TAG_PROFILER_STATS = 'zope:profiler_stats'
    TAG_GARBAGE = 'zope:garbage'
    TAG_THREADS = 'zope:threads'
    TAG_REFCOUNTS = 'zope:refcounts'

    def __init__(self, options, stream=None):
        if subunit is None:
            raise Exception("Requires subunit 0.0.5 or better")
        if content is None:
            raise Exception("Requires testtools 0.9.2 or better")
        self.options = options

        if stream is None:
            stream = sys.stdout
        self._stream = stream
        self._subunit = subunit.TestProtocolClient(self._stream)

        # Used to track the last layer that was set up or torn down. Either
        # None or (layer_name, last_touched_time).
        self._last_layer = None
        self.UTC = Utc()
        # Content types used in the output.
        self.TRACEBACK_CONTENT_TYPE = content.ContentType(
            'text', 'x-traceback', dict(language='python', charset='utf8'))
        self.PROFILE_CONTENT_TYPE = content.ContentType(
            'application', 'x-binary-profile')
        self.PLAIN_TEXT = content.ContentType(
            'text', 'plain', {'charset': 'utf8'})

    def _emit_timestamp(self, now=None):
        """Emit a timestamp to the subunit stream.

        If 'now' is not specified, use the current time on the system clock.
        """
        if now is None:
            now = datetime.now(self.UTC)
        self._subunit.time(now)
        return now

    def _emit_tag(self, tag):
        self._stream.write('tags: %s\n' % (tag,))

    def _stop_tag(self, tag):
        self._stream.write('tags: -%s\n' % (tag,))

    def _emit_fake_test(self, message, tag, details=None):
        """Emit a successful fake test to the subunit stream.

        Use this to print tagged informative messages.
        """
        test = FakeTest(message)
        self._subunit.startTest(test)
        self._emit_tag(tag)
        self._subunit.addSuccess(test, details=details)

    def _emit_error(self, error_id, tag, exc_info):
        """Emit an error to the subunit stream.

        Use this to pass on information about errors that occur outside of
        tests.
        """
        test = FakeTest(error_id)
        self._subunit.startTest(test)
        self._emit_tag(tag)
        self._subunit.addError(test, exc_info)

    def info(self, message):
        """Print an informative message, but only if verbose."""
        # info() output is not relevant to actual test results. It only says
        # things like "Running tests" or "Tearing down left over layers",
        # things that are communicated already by the subunit stream. Just
        # suppress the info() output.
        pass

    def info_suboptimal(self, message):
        """Print an informative message about losing some of the features.

        For example, when you run some tests in a subprocess, you lose the
        ability to use the debugger.
        """
        # Used _only_ to indicate running in a subprocess.
        self._emit_fake_test(message.strip(), self.TAG_INFO_SUBOPTIMAL)

    def error(self, message):
        """Report an error."""
        # XXX: Mostly used for user errors, sometimes used for errors in the
        # test framework, sometimes used to record layer setUp failure (!!!).
        self._stream.write('%s\n' % (message,))

    def error_with_banner(self, message):
        """Report an error with a big ASCII banner."""
        # Either "Could not communicate with subprocess"
        # Or "Can't post-mortem debug when running a layer as a subprocess!"
        self._emit_fake_test(message, self.TAG_ERROR_WITH_BANNER)

    def _enter_layer(self, layer_name):
        """Signal in the subunit stream that we are entering a layer."""
        self._emit_tag('zope:layer:%s' % (layer_name,))

    def _exit_layer(self, layer_name):
        """Signal in the subunit stream that we are leaving a layer."""
        self._stop_tag('zope:layer:%s' % (layer_name,))

    def start_set_up(self, layer_name):
        """Report that we're setting up a layer.

        We do this by emitting a tag of the form 'layer:$LAYER_NAME'.
        """
        now = self._emit_timestamp()
        self._subunit.startTest(FakeTest('%s:setUp' % (layer_name,)))
        self._emit_tag(self.TAG_LAYER)
        self._last_layer = (layer_name, now)

    def stop_set_up(self, seconds):
        layer_name, start_time = self._last_layer
        self._last_layer = None
        self._emit_timestamp(start_time + timedelta(seconds=seconds))
        self._subunit.addSuccess(FakeTest('%s:setUp' % (layer_name,)))
        self._enter_layer(layer_name)

    def start_tear_down(self, layer_name):
        """Report that we're tearing down a layer.

        We do this by removing a tag of the form 'layer:$LAYER_NAME'.
        """
        self._exit_layer(layer_name)
        now = self._emit_timestamp()
        self._subunit.startTest(FakeTest('%s:tearDown' % (layer_name,)))
        self._emit_tag(self.TAG_LAYER)
        self._last_layer = (layer_name, now)

    def stop_tear_down(self, seconds):
        layer_name, start_time = self._last_layer
        self._last_layer = None
        self._emit_timestamp(start_time + timedelta(seconds=seconds))
        self._subunit.addSuccess(FakeTest('%s:tearDown' % (layer_name,)))

    def tear_down_not_supported(self):
        """Report that we could not tear down a layer.

        Should be called right after start_tear_down().
        """
        layer_name, start_time = self._last_layer
        self._last_layer = None
        self._emit_timestamp(datetime.now(self.UTC))
        self._subunit.addSkip(
            FakeTest('%s:tearDown' % (layer_name,)), "tearDown not supported")

    def summary(self, n_tests, n_failures, n_errors, n_seconds):
        """Print out a summary.

        Since subunit is a stream protocol format, it has no need for a
        summary. When the stream is finished other tools can generate a
        summary if so desired.
        """
        pass

    def start_test(self, test, tests_run, total_tests):
        """Report that we're about to run a test.

        The next output operation should be test_success(), test_error(), or
        test_failure().
        """
        self._emit_timestamp()
        # Note that this always emits newlines, so it will function as well as
        # other start_test implementations if we are running in a subprocess.
        self._subunit.startTest(test)

    def stop_test(self, test):
        """Clean up the output state after a test."""
        self._subunit.stopTest(test)
        self._stream.flush()

    def stop_tests(self):
        """Clean up the output state after a collection of tests."""
        self._stream.flush()

    def test_success(self, test, seconds):
        self._emit_timestamp()
        self._subunit.addSuccess(test)

    def import_errors(self, import_errors):
        """Report test-module import errors (if any)."""
        if not import_errors:
            return
        for error in import_errors:
            self._emit_error(
                error.module, self.TAG_IMPORT_ERROR, error.exc_info)

    def modules_with_import_problems(self, import_errors):
        """Report names of modules with import problems (if any)."""
        # This is simply a summary method, and subunit output doesn't benefit
        # from summaries.
        pass

    def _exc_info_to_details(self, exc_info):
        """Translate 'exc_info' into a details dictionary usable with subunit.
        """
        # In an ideal world, we'd use the pre-bundled 'TracebackContent' class
        # from testtools. However, 'OutputFormatter' contains special logic to
        # handle errors from doctests, so we have to use that and manually
        # create an object equivalent to an instance of 'TracebackContent'.
        formatter = OutputFormatter(None)
        traceback = formatter.format_traceback(exc_info)

        # We have no idea if the traceback is a unicode object or a bytestring
        # with non-ASCII characters.  We had best be careful when handling it.
        if isinstance(traceback, unicode):
            unicode_tb = traceback
        else:
            # Assume the traceback was utf-8 encoded, but still be careful.
            unicode_tb = traceback.decode('utf8', 'replace')

        return {
            'traceback': content.Content(
                self.TRACEBACK_CONTENT_TYPE, lambda: [unicode_tb.encode('utf8')])}

    def test_error(self, test, seconds, exc_info):
        """Report that an error occurred while running a test.

        Should be called right after start_test().

        The next output operation should be stop_test().
        """
        self._emit_timestamp()
        details = self._exc_info_to_details(exc_info)
        self._subunit.addError(test, details=details)

    def test_failure(self, test, seconds, exc_info):
        """Report that a test failed.

        Should be called right after start_test().

        The next output operation should be stop_test().
        """
        self._emit_timestamp()
        details = self._exc_info_to_details(exc_info)
        self._subunit.addFailure(test, details=details)

    def profiler_stats(self, stats):
        """Report profiler stats."""
        fd, filename = tempfile.mkstemp()
        os.close(fd)
        try:
            stats.dump_stats(filename)
            stats_dump = open(filename)
            try:
                profile_content = content.Content(
                    self.PROFILE_CONTENT_TYPE, stats_dump.read)
                details = {'profiler-stats': profile_content}
                # Name the test 'zope:profiler_stats' just like its tag.
                self._emit_fake_test(
                    self.TAG_PROFILER_STATS, self.TAG_PROFILER_STATS, details)
            finally:
                stats_dump.close()
        finally:
            os.unlink(filename)

    def tests_with_errors(self, errors):
        """Report tests with errors.

        Simply not supported by the subunit formatter. Fancy summary output
        doesn't make sense.
        """
        pass

    def tests_with_failures(self, failures):
        """Report tests with failures.

        Simply not supported by the subunit formatter. Fancy summary output
        doesn't make sense.
        """
        pass

    def totals(self, n_tests, n_failures, n_errors, n_seconds):
        """Summarize the results of all layers.

        Simply not supported by the subunit formatter. Fancy summary output
        doesn't make sense.
        """
        pass

    def list_of_tests(self, tests, layer_name):
        """Report a list of test names."""
        self._enter_layer(layer_name)
        for test in tests:
            self._subunit.startTest(test)
            self._subunit.addSuccess(test)
        self._exit_layer(layer_name)

    def _get_text_details(self, name, text):
        """Get a details dictionary that just has some plain text."""
        return {
            name: content.Content(
                self.PLAIN_TEXT, lambda: [text.encode('utf8')])}

    def garbage(self, garbage):
        """Report garbage generated by tests."""
        # XXX: Really, 'garbage', 'profiler_stats' and the 'refcounts' twins
        # ought to add extra details to a fake test that represents the
        # summary information for the whole suite. However, there's no event
        # on output formatters for "everything is really finished, honest". --
        # jml, 2010-02-14
        details = self._get_text_details('garbage', unicode(garbage))
        self._emit_fake_test(
            self.TAG_GARBAGE, self.TAG_GARBAGE, details)

    def test_garbage(self, test, garbage):
        """Report garbage generated by a test.

        Encoded in the subunit stream as a test error. Clients can filter out
        these tests based on the tag if they don't think garbage should fail
        the test run.
        """
        # XXX: Perhaps 'test_garbage' and 'test_threads' ought to be within
        # the output for the actual test, appended as details to whatever
        # result the test gets. Not an option with the present API, as there's
        # no event for "no more output for this test". -- jml, 2010-02-14
        self._subunit.startTest(test)
        self._emit_tag(self.TAG_GARBAGE)
        self._subunit.addError(
            test, details=self._get_text_details('garbage', unicode(garbage)))

    def test_threads(self, test, new_threads):
        """Report threads left behind by a test.

        Encoded in the subunit stream as a test error. Clients can filter out
        these tests based on the tag if they don't think left-over threads
        should fail the test run.
        """
        self._subunit.startTest(test)
        self._emit_tag(self.TAG_THREADS)
        self._subunit.addError(
            test, details=self._get_text_details('garbage', unicode(new_threads)))

    def refcounts(self, rc, prev):
        """Report a change in reference counts."""
        details = self._get_text_details('sys-refcounts', str(rc))
        details.update(
            self._get_text_details('changes', str(rc - prev)))
        # XXX: Emit the details dict as JSON?
        self._emit_fake_test(
            self.TAG_REFCOUNTS, self.TAG_REFCOUNTS, details)

    def detailed_refcounts(self, track, rc, prev):
        """Report a change in reference counts, with extra detail."""
        details = self._get_text_details('sys-refcounts', str(rc))
        details.update(
            self._get_text_details('changes', str(rc - prev)))
        details.update(
            self._get_text_details('track', str(track.delta)))

        self._emit_fake_test(
            self.TAG_REFCOUNTS, self.TAG_REFCOUNTS, details)
