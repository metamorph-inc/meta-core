__all__ = [
    # 0, Option Flags
    'register_optionflag',
    'DONT_ACCEPT_TRUE_FOR_1',
    'DONT_ACCEPT_BLANKLINE',
    'NORMALIZE_WHITESPACE',
    'ELLIPSIS',
    'IGNORE_EXCEPTION_DETAIL',
    'COMPARISON_FLAGS',
    'REPORT_UDIFF',
    'REPORT_CDIFF',
    'REPORT_NDIFF',
    'REPORT_ONLY_FIRST_FAILURE',
    'REPORTING_FLAGS',
    # 1. Utility Functions
    # 2. Example & DocTest
    'Example',
    'DocTest',
    # 3. Doctest Parser
    'DocTestParser',
    # 4. Doctest Finder
    'DocTestFinder',
    # 5. Doctest Runner
    'DocTestRunner',
    'OutputChecker',
    'DocTestFailure',
    'UnexpectedException',
    'DebugRunner',
    # 6. Test Functions
    'testmod',
    'testfile',
    'run_docstring_examples',
    # 7. Tester
    'Tester',
    # 8. Unittest Support
    'DocTestSuite',
    'DocFileSuite',
    'DocFileTest',
    'set_unittest_reportflags',
    # 9. Debugging Support
    'script_from_examples',
    'testsource',
    'debug_src',
    'debug',
]

import sys
if sys.version > '2.5':
    __all__.append('SKIP')
# Tell people to use the builtin module instead.
import warnings
warnings.warn('zope.testing.doctest is deprecated in favour of '
              'the Python standard library doctest module', DeprecationWarning,
               stacklevel=2)


# Patch to fix an error that makes subsequent tests fail after you have
# returned unicode in a test. This is obviously not an issue in Python 3.

# Reported as #8471: http://bugs.python.org/issue8471
import doctest

if sys.version < '3':
    _org_SpoofOut = doctest._SpoofOut
    class _patched_SpoofOut(_org_SpoofOut):
        def truncate(self,   size=None):
            _org_SpoofOut.truncate(self, size)
            if not self.buf:
                self.buf = ''
    
    doctest._SpoofOut = _patched_SpoofOut


# Patching a unicode error that has been fixed in Python 2.6.5:
import sys
if sys.version < '2.6.5':    
    import re
    doctest._encoding = getattr(sys.__stdout__, 'encoding', None) or 'utf-8'
    
    def _indent(s, indent=4):
        """
        Add the given number of space characters to the beginning of
        every non-blank line in `s`, and return the result.
        If the string `s` is Unicode, it is encoded using the stdout
        encoding and the `backslashreplace` error handler.
        """
        if isinstance(s, unicode):
            s = s.encode(doctest._encoding, 'backslashreplace')
        # This regexp matches the start of non-blank lines:
        return re.sub('(?m)^(?!$)', indent*' ', s)
    
    doctest._indent = _indent

# Patch to fix tests that has mixed line endings:
# Reported as #8473: http://bugs.python.org/issue8473
import os

if sys.version < '2.5':
    from doctest import DocTestParser, master
    def _patched_testfile(filename, module_relative=True, name=None, package=None,
                 globs=None, verbose=None, report=True, optionflags=0,
                 extraglobs=None, raise_on_error=False, parser=DocTestParser()):
        global master
    
        if package and not module_relative:
            raise ValueError("Package may only be specified for module-"
                             "relative paths.")
    
        # Relativize the path
        if module_relative:
            package = _normalize_module(package)
            filename = _module_relative_path(package, filename)
    
        # If no name was given, then use the file's name.
        if name is None:
            name = os.path.basename(filename)
    
        # Assemble the globals.
        if globs is None:
            globs = {}
        else:
            globs = globs.copy()
        if extraglobs is not None:
            globs.update(extraglobs)
    
        if raise_on_error:
            runner = DebugRunner(verbose=verbose, optionflags=optionflags)
        else:
            runner = DocTestRunner(verbose=verbose, optionflags=optionflags)
    
        # Read the file, convert it to a test, and run it.
        s = open(filename, 'U').read()
        test = parser.get_doctest(s, globs, name, filename, 0)
        runner.run(test)
    
        if report:
            runner.summarize()
    
        if master is None:
            master = runner
        else:
            master.merge(runner)
    
        return runner.failures, runner.tries
    doctest.testfile = _patched_testfile
    
    from doctest import _normalize_module, _module_relative_path, DocFileCase
    def _patched_DocFileTest(path, module_relative=True, package=None,
                    globs=None, parser=DocTestParser(), **options):
        if globs is None:
            globs = {}
    
        if package and not module_relative:
            raise ValueError("Package may only be specified for module-"
                             "relative paths.")
    
        # Relativize the path.
        if module_relative:
            package = _normalize_module(package)
            path = _module_relative_path(package, path)
    
        # Find the file and read it.
        name = os.path.basename(path)
        doc = open(path, 'U').read()
    
        # Convert it to a test, and wrap it in a DocFileCase.
        test = parser.get_doctest(doc, globs, name, path, 0)
        return DocFileCase(test, **options)    
    doctest.DocFileTest = _patched_DocFileTest
else:
    
    def _patched_load_testfile(filename, package, module_relative, encoding=None):
        if module_relative:
            package = doctest._normalize_module(package, 3)
            filename = doctest._module_relative_path(package, filename)
            if hasattr(package, '__loader__'):
                if hasattr(package.__loader__, 'get_data'):
                    file_contents = package.__loader__.get_data(filename)
                    if encoding is not None: # Python 3
                        file_contents = file_contents.decode(encoding)
                    # get_data() opens files as 'rb', so one must do the equivalent
                    # conversion as universal newlines would do.
                    return file_contents.replace(os.linesep, '\n'), filename
        if encoding: # Python 3:
            return open(filename, encoding=encoding).read(), filename
        else:
            return open(filename, 'U').read(), filename
    
    doctest._load_testfile = _patched_load_testfile


# Patch to let the doctest have the globals of the testcase. This is slightly
# evil, but Zopes doctests did this, and if we change it everything breaks.
import unittest

def _patched_setUp(self):
    test = self._dt_test
    self._dt_globs = test.globs.copy()

    if self._dt_setUp is not None:
        self._dt_setUp(test)

def _patched_tearDown(self):
    test = self._dt_test

    if self._dt_tearDown is not None:
        self._dt_tearDown(test)

    test.globs.clear()
    test.globs.update(self._dt_globs)

doctest.DocTestCase.setUp = _patched_setUp
doctest.DocTestCase.tearDown = _patched_tearDown


# Patch so you can set REPORT_ONLY_FIRST_FAILURE even if you have a DIFF flag
# on the test.
import sys
from StringIO import StringIO

def _patched_runTest(self):
    test = self._dt_test
    old = sys.stdout
    new = StringIO()
    optionflags = self._dt_optionflags

    if not (optionflags & doctest.REPORTING_FLAGS):
        # The option flags don't include any reporting flags,
        # so add the default reporting flags
        optionflags |= doctest._unittest_reportflags
        
    # This should work even if you have a diff flag:
    if doctest._unittest_reportflags & doctest.REPORT_ONLY_FIRST_FAILURE:
        optionflags |= doctest.REPORT_ONLY_FIRST_FAILURE

    runner = DocTestRunner(optionflags=optionflags,
                           checker=self._dt_checker, verbose=False)

    try:
        runner.DIVIDER = "-"*70
        failures, tries = runner.run(
            test, out=new.write, clear_globs=False)
    finally:
        sys.stdout = old

    if failures:
        raise self.failureException(self.format_failure(new.getvalue()))
    
doctest.DocTestCase.runTest = _patched_runTest
from doctest import *
from doctest import DocFileTest # Not in doctests.__all__ for some reason.
