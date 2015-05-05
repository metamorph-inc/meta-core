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
"""Sample tests with sleep and layers that can't be torn down
"""

import unittest, time

class Layer1:

    def setUp(self):
        pass
    setUp = classmethod(setUp)

    def tearDown(self):
        raise NotImplementedError
    tearDown = classmethod(tearDown)


class Layer2:

    def setUp(self):
        pass
    setUp = classmethod(setUp)

    def tearDown(self):
        raise NotImplementedError
    tearDown = classmethod(tearDown)


class TestSomething1(unittest.TestCase):

    layer = Layer1

    def test_something(self):
        pass


class TestSomething2(unittest.TestCase):

    layer = Layer2

    def test_something(self):
        time.sleep(0.5)

    def test_something2(self):
        time.sleep(0.5)


def test_suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(TestSomething1))
    suite.addTest(unittest.makeSuite(TestSomething2))
    return suite


if __name__ == '__main__':
    unittest.main()
