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

def f():
    g()

def g():
    x = 1
    x = x + 1
    __traceback_info__ = "I don't know what Y should be."
    x = y + 1
    x = x + 1


def eek(self):
    """
    >>> f()
    1
    """

class Test(unittest.TestCase):

    def test1(self):
        pass

    def test2(self):
        pass

    def test3(self):
        f()

    def test4(self):
        pass

    def test5(self):
        pass

def test_suite():
    suite = unittest.TestSuite()
    suite.addTest(doctest.DocTestSuite())
    suite.addTest(unittest.makeSuite(Test))
    suite.addTest(doctest.DocFileSuite('e.txt'))
    return suite
