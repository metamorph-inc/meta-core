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
"""Test execution
"""

import subprocess

import cStringIO
import errno
import gc
import Queue
import re
import sys
import threading
import time
import traceback
import unittest

from zope.testrunner.find import import_name
from zope.testrunner.find import name_from_layer, _layer_name_cache
from zope.testrunner.refcount import TrackRefs
from zope.testrunner.options import get_options
import zope.testrunner.coverage
import zope.testrunner._doctest
import zope.testrunner.logsupport
import zope.testrunner.selftest
import zope.testrunner.profiling
import zope.testrunner.filter
import zope.testrunner.garbagecollection
import zope.testrunner.listing
import zope.testrunner.statistics
import zope.testrunner.process
import zope.testrunner.interfaces
import zope.testrunner.debug
import zope.testrunner.tb_format
import zope.testrunner.shuffle

try:
    from unittest import _UnexpectedSuccess # Python 3.1
except ImportError:
    try:
        from unittest.case import _UnexpectedSuccess # Python 2.7 and 3.2
    except ImportError:
        class _UnexpectedSuccess(Exception):
            pass


PYREFCOUNT_PATTERN = re.compile('\[[0-9]+ refs\]')

is_jython = sys.platform.startswith('java')


class SubprocessError(Exception):
    """An error occurred when running a subprocess
    """

    def __init__(self, reason, stderr):
        self.reason = reason
        self.stderr = stderr

    def __str__(self):
        return '%s: %s' % (self.reason, self.stderr)


class CanNotTearDown(Exception):
    "Couldn't tear down a test"


class Runner(object):
    """The test runner.

    It is the central point of this package and responsible for finding and
    executing tests as well as configuring itself from the (command-line)
    options passed into it.

    """

    def __init__(self, defaults=None, args=None, found_suites=None,
                 options=None, script_parts=None):
        if defaults is None:
            self.defaults = []
        else:
            self.defaults = defaults
        self.args = args
        self.found_suites = found_suites
        self.options = options
        self.script_parts = script_parts
        self.failed = True

        self.ran = 0
        self.failures = []
        self.errors = []

        self.show_report = True
        self.do_run_tests = True

        self.features = []

        self.tests_by_layer_name = {}

    def ordered_layers(self):
        layer_names = dict([(layer_from_name(layer_name), layer_name)
                            for layer_name in self.tests_by_layer_name])
        for layer in order_by_bases(layer_names):
            layer_name = layer_names[layer]
            yield layer_name, layer, self.tests_by_layer_name[layer_name]

    def register_tests(self, tests):
        """Registers tests."""
        # XXX To support multiple features that find tests this shouldn't be
        # an update but merge the various layers individually.
        self.tests_by_layer_name.update(tests)

    def run(self):
        self.configure()
        if self.options.fail:
            return True

        # XXX Hacky to support existing code.
        self.layer_name_cache = _layer_name_cache
        self.layer_name_cache.clear()

        # Global setup
        for feature in self.features:
            feature.global_setup()

        # Late setup
        #
        # Some system tools like profilers are really bad with stack frames.
        # E.g. hotshot doesn't like it when we leave the stack frame that we
        # called start() from.
        for feature in self.features:
            feature.late_setup()

        try:
            if self.do_run_tests:
                self.run_tests()
        finally:
            # Early teardown
            for feature in reversed(self.features):
                feature.early_teardown()
            # Global teardown
            for feature in reversed(self.features):
                feature.global_teardown()

        if self.show_report:
            for feature in self.features:
                feature.report()

    def configure(self):
        if self.args is None:
            self.args = sys.argv[:]
        # Check to see if we are being run as a subprocess. If we are,
        # then use the resume-layer and defaults passed in.
        if len(self.args) > 1 and self.args[1] == '--resume-layer':
            self.args.pop(1)
            resume_layer = self.args.pop(1)
            resume_number = int(self.args.pop(1))
            self.defaults = []
            while len(self.args) > 1 and self.args[1] == '--default':
                self.args.pop(1)
                self.defaults.append(self.args.pop(1))

            sys.stdin = FakeInputContinueGenerator()
        else:
            resume_layer = resume_number = None

        options = get_options(self.args, self.defaults)

        options.testrunner_defaults = self.defaults
        options.resume_layer = resume_layer
        options.resume_number = resume_number

        self.options = options

        self.features.append(zope.testrunner.selftest.SelfTest(self))
        self.features.append(zope.testrunner.logsupport.Logging(self))
        self.features.append(zope.testrunner.coverage.Coverage(self))
        self.features.append(zope.testrunner._doctest.DocTest(self))
        self.features.append(zope.testrunner.profiling.Profiling(self))
        if is_jython:
            # Jython GC support is not yet implemented
            pass
        else:
            self.features.append(
                zope.testrunner.garbagecollection.Threshold(self))
            self.features.append(
                zope.testrunner.garbagecollection.Debug(self))

        self.features.append(zope.testrunner.find.Find(self))
        self.features.append(zope.testrunner.shuffle.Shuffle(self))
        self.features.append(zope.testrunner.process.SubProcess(self))
        self.features.append(zope.testrunner.filter.Filter(self))
        self.features.append(zope.testrunner.listing.Listing(self))
        self.features.append(
            zope.testrunner.statistics.Statistics(self))
        self.features.append(zope.testrunner.tb_format.Traceback(self))

        # Remove all features that aren't activated
        self.features = [f for f in self.features if f.active]

    def run_tests(self):
        """Run all tests that were registered.

        Returns True if there where failures or False if all tests passed.

        """
        setup_layers = {}
        layers_to_run = list(self.ordered_layers())
        should_resume = False

        while layers_to_run:
            layer_name, layer, tests = layers_to_run[0]
            for feature in self.features:
                feature.layer_setup(layer)
            try:
                self.ran += run_layer(self.options, layer_name, layer, tests,
                                      setup_layers, self.failures, self.errors)
            except zope.testrunner.interfaces.EndRun:
                self.failed = True
                return
            except CanNotTearDown:
                if not self.options.resume_layer:
                    should_resume = True
                    break

            layers_to_run.pop(0)
            if self.options.processes > 1:
                should_resume = True
                break

        if should_resume:
            if layers_to_run:
                self.ran += resume_tests(
                    self.script_parts, self.options, self.features,
                    layers_to_run, self.failures, self.errors)

        if setup_layers:
            if self.options.resume_layer is None:
                self.options.output.info("Tearing down left over layers:")
            tear_down_unneeded(self.options, (), setup_layers, True)

        self.failed = bool(self.import_errors or self.failures or self.errors)


def run_tests(options, tests, name, failures, errors):
    repeat = options.repeat or 1
    repeat_range = iter(range(repeat))
    ran = 0

    output = options.output

    if is_jython:
        # Jython has no GC suppport - set count to 0
        lgarbage = 0
    else:
        gc.collect()
        lgarbage = len(gc.garbage)

    sumrc = 0
    if options.report_refcounts:
        if options.verbose:
            # XXX This code path is untested
            track = TrackRefs()
        rc = sys.gettotalrefcount()

    for iteration in repeat_range:
        if repeat > 1:
            output.info("Iteration %d" % (iteration + 1))

        if options.verbose > 0 or options.progress:
            output.info('  Running:')
        result = TestResult(options, tests, layer_name=name)

        t = time.time()

        if options.post_mortem:
            # post-mortem debugging
            for test in tests:
                if result.shouldStop:
                    break
                result.startTest(test)
                state = test.__dict__.copy()
                try:
                    try:
                        test.debug()
                    except KeyboardInterrupt:
                        raise
                    except:
                        result.addError(
                            test,
                            sys.exc_info()[:2] + (sys.exc_info()[2].tb_next, ),
                            )
                    else:
                        result.addSuccess(test)
                finally:
                    result.stopTest(test)
                test.__dict__.clear()
                test.__dict__.update(state)

        else:
            # normal
            for test in tests:
                if result.shouldStop:
                    break
                state = test.__dict__.copy()
                test(result)
                test.__dict__.clear()
                test.__dict__.update(state)

        t = time.time() - t
        output.stop_tests()
        failures.extend(result.failures)
        if hasattr(result, 'unexpectedSuccesses'):
            # Python versions prior to 2.7 do not have the concept of
            # unexpectedSuccesses.
            failures.extend(result.unexpectedSuccesses)
        errors.extend(result.errors)
        output.summary(result.testsRun, len(failures),
            len(result.errors), t)
        ran = result.testsRun

        if is_jython:
            lgarbage = 0
        else:
            gc.collect()
            if len(gc.garbage) > lgarbage:
                output.garbage(gc.garbage[lgarbage:])
                lgarbage = len(gc.garbage)

        if options.report_refcounts:

            # If we are being tested, we don't want stdout itself to
            # foul up the numbers. :)
            try:
                sys.stdout.getvalue()
            except AttributeError:
                pass

            prev = rc
            rc = sys.gettotalrefcount()
            if options.verbose:
                track.update()
                if iteration > 0:
                    output.detailed_refcounts(track, rc, prev)
                else:
                    track.delta = None
            elif iteration > 0:
                output.refcounts(rc, prev)

    return ran


def run_layer(options, layer_name, layer, tests, setup_layers,
              failures, errors):

    output = options.output
    gathered = []
    gather_layers(layer, gathered)
    needed = dict([(l, 1) for l in gathered])
    if options.resume_number != 0:
        output.info("Running %s tests:" % layer_name)
    tear_down_unneeded(options, needed, setup_layers)

    if options.resume_layer is not None:
        output.info_suboptimal("  Running in a subprocess.")

    try:
        setup_layer(options, layer, setup_layers)
    except zope.testrunner.interfaces.EndRun:
        raise
    except Exception:
        f = cStringIO.StringIO()
        traceback.print_exc(file=f)
        output.error(f.getvalue())
        errors.append((SetUpLayerFailure(layer), sys.exc_info()))
        return 0
    else:
        return run_tests(options, tests, layer_name, failures, errors)


class SetUpLayerFailure(unittest.TestCase):

    def __init__(self, layer):
        super(SetUpLayerFailure, self).__init__()
        self.layer = layer

    def runTest(self):
        pass

    def __str__(self):
        return "Layer: %s.%s" % (self.layer.__module__, self.layer.__name__)



def spawn_layer_in_subprocess(result, script_parts, options, features,
                              layer_name, layer, failures, errors,
                              resume_number):
    output = options.output

    try:
        # BBB
        if script_parts is None:
            script_parts = sys.argv[0:1]
        args = [sys.executable]
        args.extend(script_parts)
        args.extend(['--resume-layer', layer_name, str(resume_number)])
        for d in options.testrunner_defaults:
            args.extend(['--default', d])

        args.extend(options.original_testrunner_args[1:])

        # this is because of a bug in Python (http://www.python.org/sf/900092)
        if (options.profile == 'hotshot'
            and sys.version_info[:3] <= (2, 4, 1)):
            args.insert(1, '-O')

        if sys.platform.startswith('win'):
            args = args[0] + ' ' + ' '.join([
                ('"' + a.replace('\\', '\\\\').replace('"', '\\"') + '"')
                for a in args[1:]])

        for feature in features:
            feature.layer_setup(layer)

        child = subprocess.Popen(args, shell=False, stdin=subprocess.PIPE,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            close_fds=not sys.platform.startswith('win'))

        while True:
            try:
                while True:
                    # We use readline() instead of iterating over stdout
                    # because it appears that iterating over stdout causes a
                    # lot more buffering to take place (probably so it can
                    # return its lines as a batch). We don't want too much
                    # buffering because this foils automatic and human monitors
                    # trying to verify that the subprocess is still alive.
                    l = child.stdout.readline()
                    if not l:
                        break
                    result.write(l)
            except IOError, e:
                if e.errno == errno.EINTR:
                    # If the subprocess dies before we finish reading its
                    # output, a SIGCHLD signal can interrupt the reading.
                    # The correct thing to to in that case is to retry.
                    continue
                output.error(
                    "Error reading subprocess output for %s" % layer_name)
                output.info(str(e))
            else:
                break

        # Now stderr should be ready to read the whole thing.
        erriter = iter(child.stderr.read().splitlines())
        nfail = nerr = 0
        for line in erriter:
            try:
                result.num_ran, nfail, nerr = map(int, line.strip().split())
            except ValueError:
                continue
            else:
                break
        else:
            output.error_with_banner("Could not communicate with subprocess")

        while nfail > 0:
            nfail -= 1
            # Doing erriter.next().strip() confuses the 2to3 fixer, so
            # we need to do it on a separate line. Also, in python 3 this
            # returns bytes, so we decode it.
            next_fail = erriter.next()
            failures.append((next_fail.strip().decode(), None))
        while nerr > 0:
            nerr -= 1
            # Doing erriter.next().strip() confuses the 2to3 fixer, so
            # we need to do it on a separate line. Also, in python 3 this
            # returns bytes, so we decode it.
            next_err = erriter.next()
            errors.append((next_err.strip().decode(), None))

    finally:
        result.done = True


class AbstractSubprocessResult(object):
    """A result of a subprocess layer run."""

    num_ran = 0
    done = False

    def __init__(self, layer_name, queue):
        self.layer_name = layer_name
        self.queue = queue
        self.stdout = []

    def write(self, out):
        """Receive a line of the subprocess out."""


class DeferredSubprocessResult(AbstractSubprocessResult):
    """Keeps stdout around for later processing,"""

    def write(self, out):
        if not isinstance(out, str): # It's binary, which means it's Python 3
            out = out.decode()
        if not _is_dots(out):
            self.stdout.append(out)


class ImmediateSubprocessResult(AbstractSubprocessResult):
    """Sends complete output to queue."""

    def write(self, out):
        if not isinstance(out, str):
            # In Python 3, a Popen process stdout uses bytes,
            # While normal stdout uses strings. So we need to convert
            # from bytes to strings here.
            out = out.decode()
        sys.stdout.write(out)
        # Help keep-alive monitors (human or automated) keep up-to-date.
        sys.stdout.flush()


_is_dots = re.compile(r'\.+(\r\n?|\n)').match # Windows sneaks in a \r\n.
class KeepaliveSubprocessResult(AbstractSubprocessResult):
    "Keeps stdout for later processing; sends marks to queue to show activity."

    _done = False

    def _set_done(self, value):
        self._done = value
        assert value, 'Internal error: unexpectedly setting done to False'
        self.queue.put((self.layer_name, ' LAYER FINISHED'))
    done = property(lambda self: self._done, _set_done)

    def write(self, out):
        if not isinstance(out, str): # It's binary, which means it's Python 3
            out = out.decode()
        if _is_dots(out):
            self.queue.put((self.layer_name, out.strip()))
        else:
            self.stdout.append(out)


def resume_tests(script_parts, options, features, layers, failures, errors):
    results = []
    stdout_queue = None
    if options.processes == 1:
        result_factory = ImmediateSubprocessResult
    elif options.verbose > 1:
        result_factory = KeepaliveSubprocessResult
        stdout_queue = Queue.Queue()
    else:
        result_factory = DeferredSubprocessResult
    resume_number = int(options.processes > 1)
    ready_threads = []
    for layer_name, layer, tests in layers:
        result = result_factory(layer_name, stdout_queue)
        results.append(result)
        ready_threads.append(threading.Thread(
            target=spawn_layer_in_subprocess,
            args=(result, script_parts, options, features, layer_name, layer,
                  failures, errors, resume_number)))
        resume_number += 1

    # Now start a few threads at a time.
    running_threads = []
    results_iter = iter(results)
    current_result = results_iter.next()
    last_layer_intermediate_output = None
    output = None
    while ready_threads or running_threads:
        while len(running_threads) < options.processes and ready_threads:
            thread = ready_threads.pop(0)
            thread.start()
            running_threads.append(thread)

        for index, thread in reversed(list(enumerate(running_threads))):
            if not thread.isAlive():
                del running_threads[index]

        # Clear out any messages in queue
        while stdout_queue is not None:
            previous_output = output
            try:
                layer_name, output = stdout_queue.get(False)
            except Queue.Empty:
                break
            if layer_name != last_layer_intermediate_output:
                # Clarify what layer is reporting activity.
                if previous_output is not None:
                    sys.stdout.write(']\n')
                sys.stdout.write(
                    '[Parallel tests running in %s:\n  ' % (layer_name,))
                last_layer_intermediate_output = layer_name
            sys.stdout.write(output)
        # Display results in the order they would have been displayed, had the
        # work not been done in parallel.
        while current_result and current_result.done:
            if output is not None:
                sys.stdout.write(']\n')
                output = None
            map(sys.stdout.write, current_result.stdout)

            try:
                current_result = results_iter.next()
            except StopIteration:
                current_result = None

        # Help keep-alive monitors (human or automated) keep up-to-date.
        sys.stdout.flush()
        time.sleep(0.01) # Keep the loop from being too tight.

    # Return the total number of tests run.
    return sum(r.num_ran for r in results)


def tear_down_unneeded(options, needed, setup_layers, optional=False):
    # Tear down any layers not needed for these tests. The unneeded layers
    # might interfere.
    unneeded = [l for l in setup_layers if l not in needed]
    unneeded = order_by_bases(unneeded)
    unneeded.reverse()
    output = options.output
    for l in unneeded:
        output.start_tear_down(name_from_layer(l))
        t = time.time()
        try:
            try:
                if hasattr(l, 'tearDown'):
                    l.tearDown()
            except NotImplementedError:
                output.tear_down_not_supported()
                if not optional:
                    raise CanNotTearDown(l)
            else:
                output.stop_tear_down(time.time() - t)
        finally:
            del setup_layers[l]


cant_pm_in_subprocess_message = """
Can't post-mortem debug when running a layer as a subprocess!
Try running layer %r by itself.
"""


def setup_layer(options, layer, setup_layers):
    assert layer is not object
    output = options.output
    if layer not in setup_layers:
        for base in layer.__bases__:
            if base is not object:
                setup_layer(options, base, setup_layers)
        output.start_set_up(name_from_layer(layer))
        t = time.time()
        if hasattr(layer, 'setUp'):
            try:
                layer.setUp()
            except Exception:
                if options.post_mortem:
                    if options.resume_layer:
                        options.output.error_with_banner(
                            cant_pm_in_subprocess_message
                            % options.resume_layer)
                        raise
                    else:
                        zope.testrunner.debug.post_mortem(
                            sys.exc_info())
                else:
                    raise

        output.stop_set_up(time.time() - t)
        setup_layers[layer] = 1


class TestResult(unittest.TestResult):

    def __init__(self, options, tests, layer_name=None):
        unittest.TestResult.__init__(self)
        self.options = options
        # Calculate our list of relevant layers we need to call testSetUp
        # and testTearDown on.
        layers = []
        gather_layers(layer_from_name(layer_name), layers)
        self.layers = order_by_bases(layers)
        count = 0
        for test in tests:
            count += test.countTestCases()
        self.count = count

    def testSetUp(self):
        """A layer may define a setup method to be called before each
        individual test.
        """
        for layer in self.layers:
            if hasattr(layer, 'testSetUp'):
                layer.testSetUp()

    def testTearDown(self):
        """A layer may define a teardown method to be called after each
           individual test.

           This is useful for clearing the state of global
           resources or resetting external systems such as relational
           databases or daemons.
        """
        for layer in self.layers[-1::-1]:
            if hasattr(layer, 'testTearDown'):
                layer.testTearDown()

    def startTest(self, test):
        self.testSetUp()
        unittest.TestResult.startTest(self, test)
        testsRun = self.testsRun - 1 # subtract the one the base class added
        count = test.countTestCases()
        self.testsRun = testsRun + count

        self.options.output.start_test(test, self.testsRun, self.count)

        self._threads = threading.enumerate()
        self._start_time = time.time()

    def addSuccess(self, test):
        t = max(time.time() - self._start_time, 0.0)
        self.options.output.test_success(test, t)

    def addError(self, test, exc_info):
        self.options.output.test_error(test, time.time() - self._start_time,
                                       exc_info)

        unittest.TestResult.addError(self, test, exc_info)

        if self.options.post_mortem:
            if self.options.resume_layer:
                self.options.output.error_with_banner("Can't post-mortem debug"
                                                      " when running a layer"
                                                      " as a subprocess!")
            else:
                zope.testrunner.debug.post_mortem(exc_info)
        elif self.options.stop_on_error:
            self.stop()

    def addFailure(self, test, exc_info):
        self.options.output.test_failure(test, time.time() - self._start_time,
                                         exc_info)

        unittest.TestResult.addFailure(self, test, exc_info)

        if self.options.post_mortem:
            # XXX: mgedmin: why isn't there a resume_layer check here like
            # in addError?
            zope.testrunner.debug.post_mortem(exc_info)
        elif self.options.stop_on_error:
            self.stop()

    def addExpectedFailure(self, test, exc_info):
        t = max(time.time() - self._start_time, 0.0)
        self.options.output.test_success(test, t)

        unittest.TestResult.addExpectedFailure(self, test, exc_info)

    def addUnexpectedSuccess(self, test):
        self.options.output.test_error(test, time.time() - self._start_time,
                                       (_UnexpectedSuccess, None, None))

        unittest.TestResult.addUnexpectedSuccess(self, test)

        if self.options.post_mortem:
            if self.options.resume_layer:
                self.options.output.error_with_banner("Can't post-mortem debug"
                                                      " when running a layer"
                                                      " as a subprocess!")
            else:
                zope.testrunner.debug.post_mortem(exc_info)
        elif self.options.stop_on_error:
            self.stop()

    def stopTest(self, test):
        self.testTearDown()
        self.options.output.stop_test(test)

        if is_jython:
            pass
        else:
            if gc.garbage:
                self.options.output.test_garbage(test, gc.garbage)
                # TODO: Perhaps eat the garbage here, so that the garbage isn't
                #       printed for every subsequent test.

        # Did the test leave any new threads behind?
        new_threads = [t for t in threading.enumerate()
                         if (t.isAlive()
                             and
                             t not in self._threads)]
        if new_threads:
            self.options.output.test_threads(test, new_threads)


def layer_from_name(layer_name):
    """Return the layer for the corresponding layer_name by discovering
       and importing the necessary module if necessary.

       Note that a name -> layer cache is maintained by name_from_layer
       to allow locating layers in cases where it would otherwise be
       impossible.
    """
    if layer_name in _layer_name_cache:
        return _layer_name_cache[layer_name]
    layer_names = layer_name.split('.')
    layer_module, module_layer_name = layer_names[:-1], layer_names[-1]
    module_name = '.'.join(layer_module)
    module = import_name(module_name)
    try:
        return getattr(module, module_layer_name)
    except AttributeError, e:
        # the default error is very uninformative:
        #   AttributeError: 'module' object has no attribute 'DemoLayer'
        # it doesn't say *which* module
        raise AttributeError('module %r has no attribute %r'
                             % (module_name, module_layer_name))


def order_by_bases(layers):
    """Order the layers from least to most specific (bottom to top)
    """
    named_layers = [(name_from_layer(layer), layer) for layer in layers]
    named_layers.sort()
    named_layers.reverse()
    gathered = []
    for name, layer in named_layers:
        gather_layers(layer, gathered)
    gathered.reverse()
    seen = {}
    result = []
    for layer in gathered:
        if layer not in seen:
            seen[layer] = 1
            if layer in layers:
                result.append(layer)
    return result


def gather_layers(layer, result):
    if layer is not object:
        result.append(layer)
    for b in layer.__bases__:
        gather_layers(b, result)


class FakeInputContinueGenerator:

    def readline(self):
        print  'c\n'
        print '*'*70
        print ("Can't use pdb.set_trace when running a layer"
               " as a subprocess!")
        print '*'*70
        print
        return 'c\n'
