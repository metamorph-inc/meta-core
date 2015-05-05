Code Coverage
=============

On Windows drive names can be upper and lower case, these can be
randomly passed to TestIgnore.names.
Watch out for the case of the R drive!

  >>> from zope.testrunner.coverage import TestIgnore
  >>> ignore = TestIgnore((('r:\\winproject\\src\\blah\\foo', ''),
  ...                      ('R:\\winproject\\src\\blah\\bar', '')))
  >>> ignore._test_dirs
  ['r:\\winproject\\src\\blah\\foo\\', 'R:\\winproject\\src\\blah\\bar\\']

We can now ask whether a particular module should be ignored:

  >>> ignore.names('r:\\winproject\\src\\blah\\foo\\baz.py', 'baz')
  False
  >>> ignore.names('R:\\winproject\\src\\blah\\foo\\baz.py', 'baz')
  False
  >>> ignore.names('r:\\winproject\\src\\blah\\bar\\zab.py', 'zab')
  False
  >>> ignore.names('R:\\winproject\\src\\blah\\bar\\zab.py', 'zab')
  False
  >>> ignore.names('r:\\winproject\\src\\blah\\hello.py', 'hello')
  True
  >>> ignore.names('R:\\winproject\\src\\blah\\hello.py', 'hello')
  True
