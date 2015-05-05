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
"""Filter which tests to run.
"""

import zope.testrunner.feature


class Listing(zope.testrunner.feature.Feature):
    """Lists all tests in the report instead of running the tests."""

    def __init__(self, runner):
        super(Listing, self).__init__(runner)
        self.active = bool(runner.options.list_tests)

    def global_setup(self):
        self.runner.do_run_tests = False
        self.runner.failed = False

    def report(self):
        layers = self.runner.tests_by_layer_name
        for layer_name, layer, tests in self.runner.ordered_layers():
            self.runner.options.output.list_of_tests(tests, layer_name)
