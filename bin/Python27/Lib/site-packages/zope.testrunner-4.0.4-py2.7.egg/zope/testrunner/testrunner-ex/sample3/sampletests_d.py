##############################################################################
#
# Copyright (c) 2004 Zope Foundation and Contributors.
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

class TestSomething(unittest.TestCase):

    def test_set_trace1(self):
        x = 1
        import pdb; pdb.set_trace()
        y = x

    def test_set_trace2(self):
        f()

    def test_post_mortem1(self):
        x = 1
        raise ValueError

    def test_post_mortem2(self):
        g()

    def test_post_mortem_failure1(self):
        x = 1
        y = 2
        assert x == y

def f():
    x = 1
    import pdb; pdb.set_trace()
    y = x

def g():
    x = 1
    raise ValueError

def set_trace3(self):
    """
    >>> x = 1
    >>> if 1:
    ...     import pdb; pdb.set_trace()
    ...     y = x
    """

def set_trace4(self):
    """
    >>> f()
    """

def post_mortem3(self):
    """
    >>> x = 1
    >>> raise ValueError
    """

def post_mortem4(self):
    """
    >>> g()
    """


def post_mortem_failure2():
    """
    >>> x = 1
    >>> x
    2
    """


def test_suite():
    return unittest.TestSuite((
        doctest.DocTestSuite(),
        unittest.makeSuite(TestSomething),
        doctest.DocFileSuite('set_trace5.txt'),
        doctest.DocFileSuite('set_trace6.txt'),
        doctest.DocFileSuite('post_mortem5.txt'),
        doctest.DocFileSuite('post_mortem6.txt'),
        doctest.DocFileSuite('post_mortem_failure.txt'),
        ))

if __name__ == '__main__':
    unittest.main(defaultTest='test_suite')

