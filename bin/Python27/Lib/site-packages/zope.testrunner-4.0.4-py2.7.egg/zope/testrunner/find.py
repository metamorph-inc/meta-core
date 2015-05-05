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
"""Test discovery
"""

import re
import os
import unittest
import sys

import zope.testrunner.feature
import zope.testrunner.layer
import zope.testrunner.debug

identifier = re.compile(r'[_a-zA-Z]\w*$').match


class StartUpFailure(unittest.TestCase):
    """Empty test case added to the test suite to indicate import failures.

    >>> class Options(object):
    ...    post_mortem = False
    >>> options = Options()
    
    Normally the StartUpFailure just acts as an empty test suite to satisfy
    the test runner and statistics:
    
    >>> s = StartUpFailure(options, 'fauxmodule', None)
    >>> s
    <StartUpFailure module=fauxmodule>
    >>> isinstance(s,unittest.TestCase)
    True
    >>> s.shortDescription()
    'StartUpFailure: import errors in fauxmodule.'

    However, if the post mortem option is enabled:

    >>> options.post_mortem = True

    ...then the the StartUpFailure will start the debugger and stop
    the test run after the debugger quits.
    
    To simulate this, we need an exception and its associated
    exc_info: 

    >>> import sys
    >>> try:
    ...    raise Exception()
    ... except:
    ...     exc_info = sys.exc_info()

    To simulate the user pressing 'c' and hitting return in the
    debugger, we use a FakeInputContinueGenerator:
    
    >>> from zope.testrunner.runner import FakeInputContinueGenerator
    >>> old_stdin = sys.stdin
    >>> sys.stdin = FakeInputContinueGenerator()

    Now we can see the EndRun exception that is raised by the
    postmortem debugger to indicate that debugging is finished and the
    test run should be terminated:
    
    >>> from zope.testrunner.interfaces import EndRun
    >>> try: #doctest: +ELLIPSIS
    ...   try: # try...except...finally doesn't work in Python 2.4
    ...     print "Result:" # Needed to prevent the result from starting with '...'
    ...     StartUpFailure(options, None, exc_info)
    ...   except EndRun:
    ...     print "EndRun raised"
    ... finally:
    ...   sys.stdin = old_stdin
    Result:
    ...Exception:
    <BLANKLINE>
    ...
    (Pdb) c
    <BLANKLINE>
    **********************************************************************
    Can't use pdb.set_trace when running a layer as a subprocess!
    **********************************************************************
    <BLANKLINE>
    EndRun raised

    Annoyingly, sometimes StartUpFailures occur when postmortem debugging
    is enabled but no exc_info is passed. In this case, we raise a
    sensible exception rather than letting the debugger barf with an
    AttributeError:

    >>> options.post_mortem = True
    >>> StartUpFailure(options, None, exc_info[:2]+(None,))
    Traceback (most recent call last):
    ...
    TypeError: If post_mortem is specified, full exc_info must be passed!
    """
    def __init__(self, options, module, exc_info):
        if options.post_mortem:
            for item in exc_info:
                if item is None:
                    raise TypeError('If post_mortem is specified, '
                                    'full exc_info must be passed!')
            zope.testrunner.debug.post_mortem(exc_info)
        self.module = module
        self.exc_info = exc_info

    def shortDescription(self):
        return 'StartUpFailure: import errors in %s.' % self.module

    def __repr__(self):
        return '<StartUpFailure module=%s>' % self.module


def find_tests(options, found_suites=None):
    """Creates a dictionary mapping layer name to a suite of tests to be run
    in that layer.

    Passing a list of suites using the found_suites parameter will cause
    that list of suites to be used instead of attempting to load them from
    the filesystem. This is useful for unit testing the test runner.
    """
    remove_stale_bytecode(options)
    suites = {}
    if found_suites is None:
        found_suites = find_suites(options)
    for suite in found_suites:
        for test, layer_name in tests_from_suite(suite, options):
            suite = suites.get(layer_name)
            if not suite:
                suite = suites[layer_name] = unittest.TestSuite()
            suite.addTest(test)
    return suites


def find_suites(options):
    for fpath, package in find_test_files(options):
        for (prefix, prefix_package) in options.prefix:
            if fpath.startswith(prefix) and package == prefix_package:
                # strip prefix, strip .py suffix and convert separator to dots
                noprefix = fpath[len(prefix):]
                noext = strip_py_ext(options, noprefix)
                assert noext is not None
                module_name = noext.replace(os.path.sep, '.')
                if package:
                    module_name = package + '.' + module_name

                for filter_ in options.module:
                    if filter_(module_name):
                        break
                else:
                    continue

                try:
                    module = import_name(module_name)
                except KeyboardInterrupt:
                    raise
                except:
                    suite = StartUpFailure(
                        options, module_name,
                        sys.exc_info()[:2]
                        + (sys.exc_info()[2].tb_next.tb_next,),
                        )
                else:
                    try:
                        if hasattr(module, options.suite_name):
                            suite = getattr(module, options.suite_name)()
                        else:
                            suite = unittest.defaultTestLoader.loadTestsFromModule(module)
                            if suite.countTestCases() == 0:
                                raise TypeError(
                                    "Module %s does not define any tests"
                                    % module_name)

                        if isinstance(suite, unittest.TestSuite):
                            check_suite(suite, module_name)
                        else:
                            raise TypeError(
                                "Invalid test_suite, %r, in %s"
                                % (suite, module_name)
                                )
                    except KeyboardInterrupt:
                        raise
                    except:
                        suite = StartUpFailure(
                            options, module_name, sys.exc_info()[:2]+(None,))

                yield suite
                break


def find_test_files(options):
    found = {}
    for f, package in find_test_files_(options):
        if f not in found:
            found[f] = 1
            yield f, package


def find_test_files_(options):
    tests_pattern = options.tests_pattern
    test_file_pattern = options.test_file_pattern

    # If options.usecompiled, we can accept .pyc or .pyo files instead
    # of .py files.  We'd rather use a .py file if one exists.  `root2ext`
    # maps a test file path, sans extension, to the path with the best
    # extension found (.py if it exists, else .pyc or .pyo).
    # Note that "py" < "pyc" < "pyo", so if more than one extension is
    # found, the lexicographically smaller one is best.

    # Found a new test file, in directory `dirname`.  `noext` is the
    # file name without an extension, and `withext` is the file name
    # with its extension.
    def update_root2ext(dirname, noext, withext):
        key = os.path.join(dirname, noext)
        new = os.path.join(dirname, withext)
        if key in root2ext:
            root2ext[key] = min(root2ext[key], new)
        else:
            root2ext[key] = new

    for (p, package) in test_dirs(options, {}):
        for dirname, dirs, files in walk_with_symlinks(options, p):
            if dirname != p and not contains_init_py(options, files):
                # This is not a plausible test directory. Avoid descending
                # further.
                del dirs[:]
                continue
            root2ext = {}
            dirs[:] = filter(identifier, dirs)
            d = os.path.split(dirname)[1]
            if tests_pattern(d) and contains_init_py(options, files):
                # tests directory
                for file in files:
                    noext = strip_py_ext(options, file)
                    if noext and test_file_pattern(noext):
                        update_root2ext(dirname, noext, file)

            for file in files:
                noext = strip_py_ext(options, file)
                if noext and tests_pattern(noext):
                    update_root2ext(dirname, noext, file)

            winners = root2ext.values()
            winners.sort()
            for file in winners:
                yield file, package


def strip_py_ext(options, path):
    """Return path without its .py (or .pyc or .pyo) extension, or None.

    If options.usecompiled is false:
        If path ends with ".py", the path without the extension is returned.
        Else None is returned.

    If options.usecompiled is true:
        If Python is running with -O, a .pyo extension is also accepted.
        If Python is running without -O, a .pyc extension is also accepted.
    """
    if path.endswith(".py"):
        return path[:-3]
    if options.usecompiled:
        if __debug__:
            # Python is running without -O.
            ext = ".pyc"
        else:
            # Python is running with -O.
            ext = ".pyo"
        if path.endswith(ext):
            return path[:-len(ext)]
    return None


def test_dirs(options, seen):
    if options.package:
        for p in options.package:
            p = import_name(p)
            for p in p.__path__:
                p = os.path.abspath(p)
                if p in seen:
                    continue
                for (prefix, package) in options.prefix:
                    if p.startswith(prefix) or p == prefix[:-1]:
                        seen[p] = 1
                        yield p, package
                        break
    else:
        for dpath in options.test_path:
            yield dpath


def walk_with_symlinks(options, dir):
    # TODO -- really should have test of this that uses symlinks
    #         this is hard on a number of levels ...
    for dirpath, dirs, files in os.walk(dir):
        dirs.sort()
        files.sort()
        dirs[:] = [d for d in dirs if d not in options.ignore_dir]
        yield (dirpath, dirs, files)
        for d in dirs:
            p = os.path.join(dirpath, d)
            if os.path.islink(p):
                for sdirpath, sdirs, sfiles in walk_with_symlinks(options, p):
                    yield (sdirpath, sdirs, sfiles)


compiled_suffixes = '.pyc', '.pyo'
def remove_stale_bytecode(options):
    if options.keepbytecode:
        return
    for (p, _) in options.test_path:
        for dirname, dirs, files in walk_with_symlinks(options, p):
            if '__pycache__' in dirs:
                cached_files = os.listdir(os.path.join(dirname, '__pycache__'))
                for file in cached_files:
                    pyfile = file[:-14] + 'py'
                    if pyfile not in files:
                        fullname = os.path.join(dirname, '__pycache__', file)
                        options.output.info("Removing stale bytecode file %s"
                                            % fullname)
                        os.unlink(fullname)
            
            if '__pycache__' in dirname:
                # Already cleaned
                continue

            for file in files:
                if file[-4:] in compiled_suffixes and file[:-1] not in files:
                    fullname = os.path.join(dirname, file)
                    options.output.info("Removing stale bytecode file %s"
                                        % fullname)
                    os.unlink(fullname)

def contains_init_py(options, fnamelist):
    """Return true iff fnamelist contains a suitable spelling of __init__.py.

    If options.usecompiled is false, this is so iff "__init__.py" is in
    the list.

    If options.usecompiled is true, then "__init__.pyo" is also acceptable
    if Python is running with -O, and "__init__.pyc" is also acceptable if
    Python is running without -O.
    """
    if "__init__.py" in fnamelist:
        return True
    if options.usecompiled:
        if __debug__:
            # Python is running without -O.
            return "__init__.pyc" in fnamelist
        else:
            # Python is running with -O.
            return "__init__.pyo" in fnamelist
    return False


def import_name(name):
    __import__(name)
    return sys.modules[name]


def tests_from_suite(suite, options, dlevel=1,
                     dlayer=zope.testrunner.layer.UnitTests):
    """Returns a sequence of (test, layer_name)

    The tree of suites is recursively visited, with the most specific
    layer taking precedence. So if a TestCase with a layer of 'foo' is
    contained in a TestSuite with a layer of 'bar', the test case would be
    returned with 'foo' as the layer.

    Tests are also filtered out based on the test level and test selection
    filters stored in the options.
    """
    level = getattr(suite, 'level', dlevel)
    layer = getattr(suite, 'layer', dlayer)
    if not isinstance(layer, basestring):
        layer = name_from_layer(layer)

    if isinstance(suite, unittest.TestSuite):
        for possible_suite in suite:
            for r in tests_from_suite(possible_suite, options, level, layer):
                yield r
    elif isinstance(suite, StartUpFailure):
        yield (suite, None)
    else:
        if level <= options.at_level:
            for pat in options.test:
                if pat(str(suite)):
                    yield (suite, layer)
                    break


def check_suite(suite, module_name):

    """Check for bad tests in a test suite.

    "Bad tests" are those that do not inherit from unittest.TestCase.

    Note that this function is pointless on Python 2.5, because unittest itself
    checks for this in TestSuite.addTest.  It is, however, useful on earlier
    Pythons.
    """
    for x in suite:
        if isinstance(x, unittest.TestSuite):
            check_suite(x, module_name)
        elif not isinstance(x, unittest.TestCase):
            raise TypeError(
                "Invalid test, %r,\nin test_suite from %s"
                % (x, module_name)
                )


_layer_name_cache = {}

def name_from_layer(layer):
    """Determine a name for the Layer using the namespace to avoid conflicts.

    We also cache a name -> layer mapping to enable layer_from_name to work
    in cases where the layer cannot be imported (such as layers defined
    in doctests)
    """
    if layer.__module__ == '__builtin__':
        name = layer.__name__
    else:
        name = layer.__module__ + '.' + layer.__name__
    _layer_name_cache[name] = layer
    return name


class Find(zope.testrunner.feature.Feature):
    """Finds tests and registers them with the test runner."""

    active = True

    def global_setup(self):
        # Add directories to the path
        for path in self.runner.options.path:
            if path not in sys.path:
                sys.path.append(path)

        tests = find_tests(self.runner.options, self.runner.found_suites)
        self.import_errors = tests.pop(None, None)
        self.runner.register_tests(tests)

        # XXX move to reporting ???
        self.runner.options.output.import_errors(self.import_errors)
        self.runner.import_errors = bool(self.import_errors)

    def report(self):
        self.runner.options.output.modules_with_import_problems(
            self.import_errors)
