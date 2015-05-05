Test Runner
===========

The testrunner module is used to run automated tests defined using the
unittest framework.  Its primary feature is that it *finds* tests by
searching directory trees.  It doesn't require the manual
concatenation of specific test suites.  It is highly customizable and
should be usable with any project.  In addition to finding and running
tests, it provides the following additional features:

- Test filtering using specifications of:

  o test packages within a larger tree

  o regular expression patterns for test modules

  o regular expression patterns for individual tests

- Organization of tests into levels and layers

  Sometimes, tests take so long to run that you don't want to run them
  on every run of the test runner.  Tests can be defined at different
  levels.  The test runner can be configured to only run tests at a
  specific level or below by default.  Command-line options can be
  used to specify a minimum level to use for a specific run, or to run
  all tests.  Individual tests or test suites can specify their level
  via a 'level' attribute. where levels are integers increasing from 1.

  Most tests are unit tests.  They don't depend on other facilities, or
  set up whatever dependencies they have.  For larger applications,
  it's useful to specify common facilities that a large number of
  tests share.  Making each test set up and and tear down these
  facilities is both ineffecient and inconvenient.  For this reason,
  we've introduced the concept of layers, based on the idea of layered
  application architectures.  Software build for a layer should be
  able to depend on the facilities of lower layers already being set
  up.  For example, Zope defines a component architecture.  Much Zope
  software depends on that architecture.  We should be able to treat
  the component architecture as a layer that we set up once and reuse.
  Similarly, Zope application software should be able to depend on the
  Zope application server without having to set it up in each test.

  The test runner introduces test layers, which are objects that can
  set up environments for tests within the layers to use.  A layer is
  set up before running the tests in it.  Individual tests or test
  suites can define a layer by defining a `layer` attribute, which is
  a test layer.

- Reporting

  - progress meter

  - summaries of tests run

- Analysis of test execution

  - post-mortem debugging of test failures

  - memory leaks

  - code coverage

  - source analysis using pychecker

  - memory errors

  - execution times

  - profiling
