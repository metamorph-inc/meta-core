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

import unittest
import doctest

x=0
y=0
z=0

class TestA(unittest.TestCase):
    def setUp(self):
        global x
        x = 1
    def tearDown(self):
        global x
        x = 0
    def test_x1(self):
        self.assertEqual(x, 1)
    def test_y0(self):
        self.assertEqual(y, 0)
    def test_z0(self):
        self.assertEqual(z, 0)

class TestB(unittest.TestCase):
    def setUp(self):
        global y
        y = 1
    def tearDown(self):
        global y
        y = 0
    def test_y1(self):
        self.assertEqual(y, 1)
    def test_x0(self):
        self.assertEqual(x, 0)
    def test_z0(self):
        self.assertEqual(z, 0)


class TestNotMuch(unittest.TestCase):
    def test_1(self):
        pass
    def test_2(self):
        pass
    def test_3(self):
        pass

def setUp(test):
    test.globs['z'] = 1

def test_y0(self):
    """
    >>> y = 0
    >>> y
    0
    """

def test_x0(self):
    """
    >>> x = 0
    >>> x
    0
    """

def test_z1(self):
    """
    >>> z
    1
    """

def test_suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(TestA))
    suite.addTest(unittest.makeSuite(TestB))
    suite.addTest(unittest.makeSuite(TestNotMuch))
    suite.addTest(doctest.DocTestSuite(setUp=setUp))
    suite.addTest(doctest.DocFileSuite('../../sampletests.txt',
                                       setUp=setUp))
    return suite
