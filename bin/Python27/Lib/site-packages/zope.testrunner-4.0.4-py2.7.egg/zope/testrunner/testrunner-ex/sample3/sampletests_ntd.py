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
        pass

    def test_something_else(self):
        pass

    def test_error1(self):
        raise TypeError("Can we see errors")

    def test_error2(self):
        raise TypeError("I hope so")

    def test_fail1(self):
        self.assertEqual(1, 2)

    def test_fail2(self):
        self.assertEqual(1, 3)


def test_suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(TestSomething))
    return suite


if __name__ == '__main__':
    unittest.main()
