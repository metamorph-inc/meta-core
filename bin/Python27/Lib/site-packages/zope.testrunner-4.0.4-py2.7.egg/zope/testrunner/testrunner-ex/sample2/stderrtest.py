##############################################################################
#
# Copyright (c) 2003 Zope Foundation and Contributors.
# All Rights Reserved.
#
# This software is subject to the provisions of the Zope Public License,
# Version 2.0 (ZPL).  A copy of the ZPL should accompany this distribution.
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY AND ALL EXPRESS OR IMPLIED
# WARRANTIES ARE DISCLAIMED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF TITLE, MERCHANTABILITY, AGAINST INFRINGEMENT, AND FITNESS
# FOR A PARTICULAR PURPOSE.
#
##############################################################################
"""Sample tests with a layer that produces output on stderr
"""

import unittest
import doctest
import sys



class Layer:

    def setUp(self):
        pass
    setUp = classmethod(setUp)

    def tearDown(self):
        pass
    tearDown = classmethod(tearDown)


def test_something():
    """
    >>> 1 + 1
    2
    """


def test_suite():
    # Generate some text on stderr to be sure the test runner can handle it.
    sys.stderr.write('A message on stderr.'
                     '  Please ignore (expected in test output).\n')

    suite = unittest.TestSuite()
    d = doctest.DocTestSuite()
    d.layer = Layer
    suite.addTest(d)
    return suite


if __name__ == '__main__':
    unittest.main()
