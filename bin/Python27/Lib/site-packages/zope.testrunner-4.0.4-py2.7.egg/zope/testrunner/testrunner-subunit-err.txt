Using subunit output without subunit installed
==============================================

To use the --subunit reporting option, you must have subunit installed. If you
do not, you will get an error message:

    >>> import os.path, sys
    >>> directory_with_tests = os.path.join(this_directory, 'testrunner-ex')
    >>> defaults = [
    ...     '--path', directory_with_tests,
    ...     '--tests-pattern', '^sampletestsf?$',
    ...     ]

    >>> from zope import testrunner

    >>> sys.argv = 'test --subunit'.split()
    >>> _ = testrunner.run_internal(defaults)
            Subunit is not installed. Please install Subunit
            to generate subunit output.
    <BLANKLINE>
