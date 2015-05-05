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
"""Command-line option parsing
"""

import optparse
import re
import os
import sys

import pkg_resources

from zope.testrunner.profiling import available_profilers
from zope.testrunner.formatter import (
    OutputFormatter,
    ColorfulOutputFormatter,
    SubunitOutputFormatter,
    )
from zope.testrunner.formatter import terminal_has_colors


parser = optparse.OptionParser("Usage: %prog [options] [MODULE] [TEST]")

######################################################################
# Searching and filtering

searching = optparse.OptionGroup(parser, "Searching and filtering", """\
Options in this group are used to define which tests to run.
""")

searching.add_option(
    '--package', '--dir', '-s', action="append", dest='package',
    help="""\
Search the given package's directories for tests.  This can be
specified more than once to run tests in multiple parts of the source
tree.  For example, if refactoring interfaces, you don't want to see
the way you have broken setups for tests in other packages. You *just*
want to run the interface tests.

Packages are supplied as dotted names.  For compatibility with the old
test runner, forward and backward slashed in package names are
converted to dots.

(In the special case of packages spread over multiple directories,
only directories within the test search path are searched. See the
--path option.)

""")

searching.add_option(
    '--module', '-m', action="append", dest='module',
    help="""\
Specify a test-module filter as a regular expression.  This is a
case-sensitive regular expression, used in search (not match) mode, to
limit which test modules are searched for tests.  The regular
expressions are checked against dotted module names.  In an extension
of Python regexp notation, a leading "!" is stripped and causes the
sense of the remaining regexp to be negated (so "!bc" matches any
string that does not match "bc", and vice versa).  The option can be
specified multiple test-module filters.  Test modules matching any of
the test filters are searched.  If no test-module filter is specified,
then all test modules are used.
""")

searching.add_option(
    '--test', '-t', action="append", dest='test',
    help="""\
Specify a test filter as a regular expression.  This is a
case-sensitive regular expression, used in search (not match) mode, to
limit which tests are run.  In an extension of Python regexp notation,
a leading "!" is stripped and causes the sense of the remaining regexp
to be negated (so "!bc" matches any string that does not match "bc",
and vice versa).  The option can be specified multiple test filters.
Tests matching any of the test filters are included.  If no test
filter is specified, then all tests are run.
""")

searching.add_option(
    '--unit', '-u', action="store_true", dest='unit',
    help="""\
Run only unit tests, ignoring any layer options.
""")

searching.add_option(
    '--non-unit', '-f', action="store_true", dest='non_unit',
    help="""\
Run tests other than unit tests.
""")

searching.add_option(
    '--layer', action="append", dest='layer',
    help="""\
Specify a test layer to run.  The option can be given multiple times
to specify more than one layer.  If not specified, all layers are run.
It is common for the running script to provide default values for this
option.  Layers are specified regular expressions, used in search
mode, for dotted names of objects that define a layer.  In an
extension of Python regexp notation, a leading "!" is stripped and
causes the sense of the remaining regexp to be negated (so "!bc"
matches any string that does not match "bc", and vice versa).  The
layer named 'zope.testrunner.layer.UnitTests' is reserved for
unit tests, however, take note of the --unit and non-unit options.
""")

searching.add_option(
    '-a', '--at-level', type='int', dest='at_level',
    help="""\
Run the tests at the given level.  Any test at a level at or below
this is run, any test at a level above this is not run.  Level 0
runs all tests.
""")

searching.add_option(
    '--all', action="store_true", dest='all',
    help="Run tests at all levels.")

searching.add_option(
    '--list-tests', action="store_true", dest='list_tests',
    help="List all tests that matched your filters.  Do not run any tests.")

parser.add_option_group(searching)

######################################################################
# Reporting

reporting = optparse.OptionGroup(parser, "Reporting", """\
Reporting options control basic aspects of test-runner output
""")

reporting.add_option(
    '--verbose', '-v', action="count", dest='verbose',
    help="""\
Make output more verbose.
Increment the verbosity level.
""")

reporting.add_option(
    '--quiet', '-q', action="store_true", dest='quiet',
    help="""\
Make the output minimal, overriding any verbosity options.
""")

reporting.add_option(
    '--progress', '-p', action="store_true", dest='progress',
    help="""\
Output progress status
""")

reporting.add_option(
    '--no-progress',action="store_false", dest='progress',
    help="""\
Do not output progress status.  This is the default, but can be used to
counter a previous use of --progress or -p.
""")

# We use a noop callback because the actual processing will be done in the
# get_options function, but we want optparse to generate appropriate help info
# for us, so we add an option anyway.
reporting.add_option(
    '--auto-progress', action="callback", callback=lambda *args: None,
    help="""\
Output progress status, but only when stdout is a terminal.
""")

reporting.add_option(
    '--color', '-c', action="store_true", dest='color',
    help="""\
Colorize the output.
""")

reporting.add_option(
    '--no-color', '-C', action="store_false", dest='color',
    help="""\
Do not colorize the output.  This is the default, but can be used to
counter a previous use of --color or -c.
""")

# We use a noop callback because the actual processing will be done in the
# get_options function, but we want optparse to generate appropriate help info
# for us, so we add an option anyway.
reporting.add_option(
    '--auto-color', action="callback", callback=lambda *args: None,
    help="""\
Colorize the output, but only when stdout is a terminal.
""")

reporting.add_option(
    '--subunit', action="store_true", dest='subunit',
    help="""\
Use subunit output. Will not be colorized.
""")

reporting.add_option(
    '--slow-test', type='float', dest='slow_test_threshold', metavar='N',
    help="""\
With -c and -vvv, highlight tests that take longer than N seconds (default:
%default).
""")

reporting.add_option(
    '-1', '--hide-secondary-failures',
    action="store_true", dest='report_only_first_failure',
    help="""\
Report only the first failure in a doctest. (Examples after the
failure are still executed, in case they do any cleanup.)
""")

reporting.add_option(
    '--show-secondary-failures',
    action="store_false", dest='report_only_first_failure',
    help="""\
Report all failures in a doctest.  This is the default, but can
be used to counter a default use of -1 or --hide-secondary-failures.
""")

reporting.add_option(
    '--ndiff', action="store_true", dest="ndiff",
    help="""\
When there is a doctest failure, show it as a diff using the ndiff.py utility.
""")

reporting.add_option(
    '--udiff', action="store_true", dest="udiff",
    help="""\
When there is a doctest failure, show it as a unified diff.
""")

reporting.add_option(
    '--cdiff', action="store_true", dest="cdiff",
    help="""\
When there is a doctest failure, show it as a context diff.
""")

parser.add_option_group(reporting)

######################################################################
# Analysis

analysis = optparse.OptionGroup(parser, "Analysis", """\
Analysis options provide tools for analysing test output.
""")


analysis.add_option(
    '--stop-on-error', '--stop', '-x', action="store_true",
    dest='stop_on_error',
    help="Stop running tests after first test failure or error."
    )

analysis.add_option(
    '--post-mortem', '--pdb', '-D', action="store_true", dest='post_mortem',
    help="Enable post-mortem debugging of test failures"
    )


analysis.add_option(
    '--gc', '-g', action="append", dest='gc', type="int",
    help="""\
Set the garbage collector generation threshold.  This can be used
to stress memory and gc correctness.  Some crashes are only
reproducible when the threshold is set to 1 (aggressive garbage
collection).  Do "--gc 0" to disable garbage collection altogether.

The --gc option can be used up to 3 times to specify up to 3 of the 3
Python gc_threshold settings.

""")

analysis.add_option(
    '--gc-option', '-G', action="append", dest='gc_option', type="choice",
    choices=['DEBUG_STATS', 'DEBUG_COLLECTABLE', 'DEBUG_UNCOLLECTABLE',
             'DEBUG_INSTANCES', 'DEBUG_OBJECTS', 'DEBUG_SAVEALL',
             'DEBUG_LEAK'],
    help="""\
Set a Python gc-module debug flag.  This option can be used more than
once to set multiple flags.
""")

analysis.add_option(
    '--repeat', '-N', action="store", type="int", dest='repeat',
    help="""\
Repeat the tests the given number of times.  This option is used to
make sure that tests leave their environment in the state they found
it and, with the --report-refcounts option to look for memory leaks.
""")

analysis.add_option(
    '--report-refcounts', '-r', action="store_true", dest='report_refcounts',
    help="""\
After each run of the tests, output a report summarizing changes in
refcounts by object type.  This option that requires that Python was
built with the --with-pydebug option to configure.
""")

analysis.add_option(
    '--coverage', action="store", type='string', dest='coverage',
    help="""\
Perform code-coverage analysis, saving trace data to the directory
with the given name.  A code coverage summary is printed to standard
out.
""")

analysis.add_option(
    '--profile', action="store", dest='profile', type="choice",
    choices=available_profilers.keys(),
    help="""\
Run the tests under cProfiler or hotshot and display the top 50 stats, sorted
by cumulative time and number of calls.
""")

def do_pychecker(*args):
    if not os.environ.get("PYCHECKER"):
        os.environ["PYCHECKER"] = "-q"
    import pychecker.checker

analysis.add_option(
    '--pychecker', action="callback", callback=do_pychecker,
    help="""\
Run the tests under pychecker
""")

parser.add_option_group(analysis)

######################################################################
# Setup

setup = optparse.OptionGroup(parser, "Setup", """\
Setup options are normally supplied by the testrunner script, although
they can be overridden by users.
""")

setup.add_option(
    '--path', action="append", dest='path',
    help="""\
Specify a path to be added to Python's search path.  This option can
be used multiple times to specify multiple search paths.  The path is
usually specified by the test-runner script itself, rather than by
users of the script, although it can be overridden by users.  Only
tests found in the path will be run.

This option also specifies directories to be searched for tests.
See the search_directory.
""")

setup.add_option(
    '--test-path', action="append", dest='test_path',
    help="""\
Specify a path to be searched for tests, but not added to the Python
search path.  This option can be used multiple times to specify
multiple search paths.  The path is usually specified by the
test-runner script itself, rather than by users of the script,
although it can be overridden by users.  Only tests found in the path
will be run.
""")

setup.add_option(
    '--package-path', action="append", dest='package_path', nargs=2,
    help="""\
Specify a path to be searched for tests, but not added to the Python
search path.  Also specify a package for files found in this path.
This is used to deal with directories that are stitched into packages
that are not otherwise searched for tests.

This option takes 2 arguments.  The first is a path name. The second is
the package name.

This option can be used multiple times to specify
multiple search paths.  The path is usually specified by the
test-runner script itself, rather than by users of the script,
although it can be overridden by users.  Only tests found in the path
will be run.
""")

setup.add_option(
    '--tests-pattern', action="store", dest='tests_pattern',
    help="""\
The test runner looks for modules containing tests.  It uses this
pattern to identify these modules.  The modules may be either packages
or python files.

If a test module is a package, it uses the value given by the
test-file-pattern to identify python files within the package
containing tests.
""")

setup.add_option(
    '--suite-name', action="store", dest='suite_name',
    help="""\
Specify the name of the object in each test_module that contains the
module's test suite.
""")

setup.add_option(
    '--test-file-pattern', action="store", dest='test_file_pattern',
    help="""\
Specify a pattern for identifying python files within a tests package.
See the documentation for the --tests-pattern option.
""")

setup.add_option(
    '--ignore_dir', action="append", dest='ignore_dir',
    help="""\
Specifies the name of a directory to ignore when looking for tests.
""")

setup.add_option(
    '--shuffle', action="store_true", dest='shuffle',
    help="""\
Shuffles the order in which tests are ran.
""")

setup.add_option(
    '--shuffle-seed', action="store", dest='shuffle_seed', type="int",
    help="""\
Value used to initialize the tests shuffler. Specify a value to create
repeatable random ordered tests.
""")

parser.add_option_group(setup)

######################################################################
# Other

other = optparse.OptionGroup(parser, "Other", "Other options")

other.add_option(
    '--version', action="store_true", dest='showversion',
    help="Print the version of the testrunner, and exit.")

other.add_option(
    '-j', action="store", type="int", dest='processes',
    help="""\
Use up to given number of parallel processes to execute tests.  May decrease
test run time substantially.  Defaults to %default.
""")

other.add_option(
    '--keepbytecode', '-k', action="store_true", dest='keepbytecode',
    help="""\
Normally, the test runner scans the test paths and the test
directories looking for and deleting pyc or pyo files without
corresponding py files.  This is to prevent spurious test failures due
to finding compiled modules where source modules have been deleted.
This scan can be time consuming.  Using this option disables this
scan.  If you know you haven't removed any modules since last running
the tests, can make the test run go much faster.
""")

other.add_option(
    '--usecompiled', action="store_true", dest='usecompiled',
    help="""\
Normally, a package must contain an __init__.py file, and only .py files
can contain test code.  When this option is specified, compiled Python
files (.pyc and .pyo) can be used instead:  a directory containing
__init__.pyc or __init__.pyo is also considered to be a package, and if
file XYZ.py contains tests but is absent while XYZ.pyc or XYZ.pyo exists
then the compiled files will be used.  This is necessary when running
tests against a tree where the .py files have been removed after
compilation to .pyc/.pyo.  Use of this option implies --keepbytecode.
""")

other.add_option(
    '--exit-with-status', action="store_true", dest='exitwithstatus',
    help="""DEPRECATED: The test runner will always exit with a status.\
""")


parser.add_option_group(other)

######################################################################
# Default values

parser.set_defaults(
    ignore_dir=['.svn', 'CVS', '{arch}', '.arch-ids', '_darcs'],
    tests_pattern='^tests$',
    at_level=1,
    test_file_pattern='^test',
    suite_name='test_suite',
    list_tests=False,
    slow_test_threshold=10,
    processes=1,
    verbose=0,
    repeat=1,
    )


######################################################################
# Command-line processing

def compile_filter(pattern):
    if pattern.startswith('!'):
        pattern = re.compile(pattern[1:]).search
        return (lambda s: not pattern(s))
    return re.compile(pattern).search

def merge_options(options, defaults):
    odict = options.__dict__
    for name, value in defaults.__dict__.items():
        if (value is not None) and (odict[name] is None):
            odict[name] = value

def get_options(args=None, defaults=None):
    # Because we want to inspect stdout and decide to colorize or not, we
    # replace the --auto-color option with the appropriate --color or
    # --no-color option.  That way the subprocess doesn't have to decide (which
    # it would do incorrectly anyway because stdout would be a pipe).
    def apply_auto_color(args):
        if args and '--auto-color' in args:
            if sys.stdout.isatty() and terminal_has_colors():
                colorization = '--color'
            else:
                colorization = '--no-color'

            args[:] = [arg.replace('--auto-color', colorization)
                       for arg in args]

    # The comment of apply_auto_color applies here as well
    def apply_auto_progress(args):
        if args and '--auto-progress' in args:
            if sys.stdout.isatty():
                progress = '--progress'
            else:
                progress = '--no-progress'

            args[:] = [arg.replace('--auto-progress', progress)
                       for arg in args]

    apply_auto_color(args)
    apply_auto_color(defaults)
    apply_auto_progress(args)
    apply_auto_progress(defaults)

    if defaults:
        defaults, _ = parser.parse_args(defaults)
        assert not _
    else:
        defaults = None

    if args is None:
        args = sys.argv

    options, positional = parser.parse_args(args[1:], defaults)
    options.original_testrunner_args = args

    if options.showversion:
        dist = pkg_resources.require('zope.testrunner')[0]
        print 'zope.app.testrunner version %s' % dist.version
        options.fail = True
        return options

    if options.subunit:
        try:
            import subunit
        except ImportError:
            print """\
        Subunit is not installed. Please install Subunit
        to generate subunit output.
        """
            options.fail = True
            return options
        else:
            options.output = SubunitOutputFormatter(options)
    elif options.color:
        options.output = ColorfulOutputFormatter(options)
        options.output.slow_test_threshold = options.slow_test_threshold
    else:
        options.output = OutputFormatter(options)

    options.fail = False

    if positional:
        module_filter = positional.pop(0)
        if module_filter != '.':
            if options.module:
                options.module.append(module_filter)
            else:
                options.module = [module_filter]

        if positional:
            test_filter = positional.pop(0)
            if options.test:
                options.test.append(test_filter)
            else:
                options.test = [test_filter]

            if positional:
                parser.error("Too many positional arguments")

    options.ignore_dir = dict([(d,1) for d in options.ignore_dir])
    options.test_file_pattern = re.compile(options.test_file_pattern).search
    options.tests_pattern = re.compile(options.tests_pattern).search
    options.test = map(compile_filter, options.test or ('.'))
    options.module = map(compile_filter, options.module or ('.'))

    options.path = map(os.path.abspath, options.path or ())
    options.test_path = map(os.path.abspath, options.test_path or ())
    options.test_path += options.path

    options.test_path = ([(path, '') for path in options.test_path]
                         +
                         [(os.path.abspath(path), package)
                          for (path, package) in options.package_path or ()
                          ])

    if options.package:
        pkgmap = dict(options.test_path)
        options.package = [normalize_package(p, pkgmap)
                           for p in options.package]

    options.prefix = [(path + os.path.sep, package)
                      for (path, package) in options.test_path]
    if options.all:
        options.at_level = sys.maxint

    if options.unit and options.non_unit:
        # The test runner interprets this as "run only those tests that are
        # both unit and non-unit at the same time".  The user, however, wants
        # to run both unit and non-unit tests.  Disable the filtering so that
        # the user will get what she wants:
        options.unit = options.non_unit = False

    if options.unit:
        # XXX Argh.
        options.layer = ['zope.testrunner.layer.UnitTests']
    if options.layer:
        options.layer = map(compile_filter, options.layer)

    options.layer = options.layer and dict([(l, 1) for l in options.layer])

    if options.usecompiled:
        options.keepbytecode = options.usecompiled

    if options.quiet:
        options.verbose = 0

    if options.report_refcounts and options.repeat < 2:
        print """\
        You must use the --repeat (-N) option to specify a repeat
        count greater than 1 when using the --report_refcounts (-r)
        option.
        """
        options.fail = True
        return options


    if options.report_refcounts and not hasattr(sys, "gettotalrefcount"):
        print """\
        The Python you are running was not configured
        with --with-pydebug. This is required to use
        the --report-refcounts option.
        """
        options.fail = True
        return options

    return options

def normalize_package(package, package_map={}):
    r"""Normalize package name passed to the --package option.

        >>> normalize_package('zope.testrunner')
        'zope.testrunner'

    Converts path names into package names for compatibility with the old
    test runner.

        >>> normalize_package('zope/testrunner')
        'zope.testrunner'
        >>> normalize_package('zope/testrunner/')
        'zope.testrunner'
        >>> normalize_package('zope\\testrunner')
        'zope.testrunner'

    Can use a map of absolute pathnames to package names

        >>> a = os.path.abspath
        >>> normalize_package('src/zope/testrunner/',
        ...                   {a('src'): ''})
        'zope.testrunner'
        >>> normalize_package('src/zope_testrunner/',
        ...                   {a('src/zope_testrunner'): 'zope.testrunner'})
        'zope.testrunner'
        >>> normalize_package('src/zope_something/tests',
        ...                   {a('src/zope_something'): 'zope.something',
        ...                    a('src'): ''})
        'zope.something.tests'

    """
    package = package.replace('\\', '/')
    if package.endswith('/'):
        package = package[:-1]
    bits = package.split('/')
    for n in range(len(bits), 0, -1):
        pkg = package_map.get(os.path.abspath('/'.join(bits[:n])))
        if pkg is not None:
            bits = bits[n:]
            if pkg:
                bits = [pkg] + bits
            return '.'.join(bits)
    return package.replace('/', '.')
