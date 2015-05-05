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
"""Test runner statistics
"""

import time
import zope.testrunner.feature


class Statistics(zope.testrunner.feature.Feature):

    active = True
    layers_run = 0
    tests_run = 0

    def late_setup(self):
        self.start_time = time.time()

    def early_teardown(self):
        self.end_time = time.time()

    def global_teardown(self):
        self.total_time = self.end_time - self.start_time

    def layer_setup(self, layer):
        self.layers_run += 1

    def report(self):
        if not self.runner.do_run_tests:
            return
        if self.layers_run == 1:
            return
        self.runner.options.output.totals(
            self.runner.ran, len(self.runner.failures), len(self.runner.errors),
            self.total_time)
