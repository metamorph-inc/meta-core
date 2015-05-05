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
"""Sample tests with a layer that can't be torn down
"""

import unittest
import doctest

class Layer:

    def setUp(self):
        pass
    setUp = classmethod(setUp)

    def tearDown(self):
        raise NotImplementedError
    tearDown = classmethod(tearDown)

class TestSomething(unittest.TestCase):

    layer = Layer

    def test_something(self):
        import pdb; pdb.set_trace()

    def test_something2(self):
        import pdb; pdb.set_trace()

    def test_something3(self):
        import pdb; pdb.set_trace()

    def test_something4(self):
        import pdb; pdb.set_trace()

    def test_something5(self):
        f()

def f():
    import pdb; pdb.set_trace()

def test_set_trace():
    """
    >>> if 1:
    ...     x = 1
    ...     import pdb; pdb.set_trace()
    """
    
def test_set_trace2():
    """
    >>> f()
    """


def test_suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(TestSomething))
    d = doctest.DocTestSuite()
    d.layer = Layer
    suite.addTest(d)
    return suite


if __name__ == '__main__':
    unittest.main()
