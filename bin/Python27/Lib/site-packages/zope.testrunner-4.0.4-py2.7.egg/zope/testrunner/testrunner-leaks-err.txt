Debugging Memory Leaks without a debug build of Python 
======================================================

To use the --report-refcounts (-r) to detect or debug memory leaks,
you must have a debug build of Python. Without a debug build, you will
get an error message:

    >>> import os.path, sys
    >>> directory_with_tests = os.path.join(this_directory, 'testrunner-ex')
    >>> defaults = [
    ...     '--path', directory_with_tests,
    ...     '--tests-pattern', '^sampletestsf?$',
    ...     ]

    >>> from zope import testrunner
    
    >>> sys.argv = 'test -r -N6'.split()
    >>> _ = testrunner.run_internal(defaults)
            The Python you are running was not configured
            with --with-pydebug. This is required to use
            the --report-refcounts option.
    <BLANKLINE>
