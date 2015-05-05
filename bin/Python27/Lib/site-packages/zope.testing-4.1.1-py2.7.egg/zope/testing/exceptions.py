##############################################################################
#
# Copyright (c) 2004 Zope Foundation and Contributors.
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
"""Exceptions for zope.testing
"""

import warnings
# Tell people to use the builtin module instead.
warnings.warn('zope.testing.exceptions is deprecated in favour of '
              'zope.testrunner.exceptions', DeprecationWarning,
               stacklevel=2)

class DocTestFailureException(AssertionError):
    """Use custom exception for doctest unit test failures"""
