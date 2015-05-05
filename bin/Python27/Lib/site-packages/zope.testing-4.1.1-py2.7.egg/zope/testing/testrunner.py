import warnings
warnings.warn('zope.testing.testrunner is deprecated in favour of '
              'zope.testrunner', DeprecationWarning, stacklevel=2)

try:
    from zope import testrunner
    import zope.testing
    # Now replace this module with the right one:
    zope.testing.testrunner = testrunner
except ImportError:
    pass