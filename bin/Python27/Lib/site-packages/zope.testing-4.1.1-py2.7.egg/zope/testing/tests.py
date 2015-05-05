##############################################################################
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
"""Tests for the testing framework.
"""

import doctest
import sys
import re
import unittest
import warnings
from zope.testing import renormalizing

if sys.version < '3':
    # Yes, it is deprecated, but we want to run tests on it here.
    warnings.filterwarnings("ignore", "zope.testing.doctest is deprecated",
                            DeprecationWarning, __name__, 0)

    from zope.testing import doctest

def print_(*args):
    sys.stdout.write(' '.join(map(str, args))+'\n')

def setUp(test):
    test.globs['print_'] = print_

def test_suite():
    suite = unittest.TestSuite((
        doctest.DocFileSuite(
            'module.txt',
            # when this test is run in isolation, the error message shows the
            # module name as fully qualified; when it is run as part of the
            # full test suite, the error message shows the module name as
            # relative.
            checker=renormalizing.RENormalizing([
                (re.compile('No module named zope.testing.unlikelymodulename'),
                 'No module named unlikelymodulename')])),
        doctest.DocFileSuite('loggingsupport.txt', setUp=setUp),
        doctest.DocFileSuite('renormalizing.txt', setUp=setUp),
        doctest.DocFileSuite('setupstack.txt', setUp=setUp),
        doctest.DocFileSuite(
            'wait.txt', setUp=setUp,
            checker=renormalizing.RENormalizing([
                (re.compile('zope.testing.wait.TimeOutWaitingFor: '),
                 'TimeOutWaitingFor: '),
                ])
            ),
        ))

    if sys.version < '3':
        suite.addTests(doctest.DocFileSuite('doctest.txt'))
        suite.addTests(doctest.DocFileSuite('unicode.txt'))
        suite.addTests(doctest.DocTestSuite('zope.testing.server'))
        suite.addTests(doctest.DocFileSuite('formparser.txt'))
    return suite
