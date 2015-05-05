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
import doctest

pledge_template = """\
I give my pledge, as %s,
to save, and faithfully, to defend from waste,
the natural resources of my %s.
It's soils, minerals, forests, waters, and wildlife.
"""

def pledge():
    """
    >>> def print_pledge():
    ...     print pledge_template % ('and earthling', 'planet')
    >>> print_pledge()
    I give my pledge, as an earthling,
    to save, and faithfully, to defend from waste,
    the natural resources of my planet.
    It's soils, minerals, forests, waters, and wildlife.
    <BLANKLINE>
    """

def test_suite():
    return doctest.DocTestSuite()

