Debugging Memory Leaks with subunit output
==========================================

The --report-refcounts (-r) option can be used with the --repeat (-N)
option to detect and diagnose memory leaks.  To use this option, you
must configure Python with the --with-pydebug option. (On Unix, pass
this option to configure and then build Python.)

For more detailed documentation, see testrunner-leaks.txt.

    >>> import os.path, sys
    >>> directory_with_tests = os.path.join(this_directory, 'testrunner-ex')
    >>> defaults = [
    ...     '--path', directory_with_tests,
    ...     '--tests-pattern', '^sampletestsf?$',
    ...     ]

    >>> from zope import testrunner

Each layer is repeated the requested number of times.  For each
iteration after the first, the system refcount and change in system
refcount is shown. The system refcount is the total of all refcount in
the system.  When a refcount on any object is changed, the system
refcount is changed by the same amount.  Tests that don't leak show
zero changes in system refcount.

Let's look at an example test that leaks:

    >>> sys.argv = 'test --subunit --tests-pattern leak -N2 -r'.split()
    >>> _ = testrunner.run_internal(defaults)
    time: YYYY-MM-DD HH:MM:SS.mmmmmmZ
    test: zope.testrunner.layer.UnitTests:setUp
    tags: zope:layer
    time: YYYY-MM-DD HH:MM:SS.mmmmmmZ
    successful: zope.testrunner.layer.UnitTests:setUp
    tags: zope:layer:zope.testrunner.layer.UnitTests
    test: leak.TestSomething.testleak
    successful: leak.TestSomething.testleak
    test: leak.TestSomething.testleak
    successful: leak.TestSomething.testleak
    test: zope:refcounts
    tags: zope:refcounts
    successful: zope:refcounts [ multipart
    Content-Type: text/plain;charset=utf8
    ...
    ...\r
    <BLANKLINE>
    ...\r
    <BLANKLINE>
    Content-Type: text/plain;charset=utf8
    ...
    ...\r
    <BLANKLINE>
    ...\r
    <BLANKLINE>
    ]
    tags: -zope:layer:zope.testrunner.layer.UnitTests
    time: YYYY-MM-DD HH:MM:SS.mmmmmmZ
    test: zope.testrunner.layer.UnitTests:tearDown
    tags: zope:layer
    time: YYYY-MM-DD HH:MM:SS.mmmmmmZ
    successful: zope.testrunner.layer.UnitTests:tearDown

Here we see that the system refcount is increasing.  If we specify a
verbosity greater than one, we can get details broken out by object
type (or class):

    >>> sys.argv = 'test --subunit --tests-pattern leak -N2 -v -r'.split()
    >>> _ = testrunner.run_internal(defaults)
    time: YYYY-MM-DD HH:MM:SS.mmmmmmZ
    test: zope.testrunner.layer.UnitTests:setUp
    tags: zope:layer
    time: YYYY-MM-DD HH:MM:SS.mmmmmmZ
    successful: zope.testrunner.layer.UnitTests:setUp
    tags: zope:layer:zope.testrunner.layer.UnitTests
    test: leak.TestSomething.testleak
    successful: leak.TestSomething.testleak
    test: leak.TestSomething.testleak
    successful: leak.TestSomething.testleak
    test: zope:refcounts
    tags: zope:refcounts
    successful: zope:refcounts [ multipart
    Content-Type: text/plain;charset=utf8
    ...
    ...\r
    <BLANKLINE>
    ...\r
    <BLANKLINE>
    Content-Type: text/plain;charset=utf8
    ...
    ...\r
    <BLANKLINE>
    ...\r
    <BLANKLINE>
    Content-Type: text/plain;charset=utf8
    ...
    ...\r
    <BLANKLINE>
    ...
    <BLANKLINE>
    ]
    tags: -zope:layer:zope.testrunner.layer.UnitTests
    time: YYYY-MM-DD HH:MM:SS.mmmmmmZ
    test: zope.testrunner.layer.UnitTests:tearDown
    tags: zope:layer
    time: YYYY-MM-DD HH:MM:SS.mmmmmmZ
    successful: zope.testrunner.layer.UnitTests:tearDown
