##############################################################################
#
# Copyright (c) 2004-2008 Zope Foundation and Contributors.
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
"""Self-test support.

Provides setup routines that enable the test runner to test itself.
"""

import pdb
import zope.testrunner.feature

real_pdb_set_trace = pdb.set_trace


class SelfTest(zope.testrunner.feature.Feature):

    active = True

    def global_setup(self):
        # Make sure we start with real pdb.set_trace.
        pdb.set_trace = real_pdb_set_trace
