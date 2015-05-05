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


UNITTEST_LAYER = 'zope.testrunner.layer.UnitTests'


class Filter(zope.testrunner.feature.Feature):
    """Filters and orders all tests registered until now."""

    active = True

    def global_setup(self):
        layers = self.runner.tests_by_layer_name
        options = self.runner.options

        if UNITTEST_LAYER in layers:
            # We start out assuming unit tests should run and look for reasons
            # why they shouldn't be run.
            should_run = True
            if (not options.non_unit):
                if options.layer:
                    should_run = False
                    for pat in options.layer:
                        if pat(UNITTEST_LAYER):
                            should_run = True
                            break
                else:
                    should_run = True
            else:
                should_run = False

            if not should_run:
                layers.pop(UNITTEST_LAYER)

        if self.runner.options.resume_layer is not None:
            for name in list(layers):
                if name != self.runner.options.resume_layer:
                    layers.pop(name)
        elif self.runner.options.layer:
            for name in list(layers):
                for pat in self.runner.options.layer:
                    if pat(name):
                        # This layer matches a pattern selecting this layer
                        break
                else:
                    # No pattern matched this name so we remove it
                    layers.pop(name)

        if (self.runner.options.verbose and
            not self.runner.options.resume_layer):
            if self.runner.options.all:
                msg = "Running tests at all levels"
            else:
                msg = "Running tests at level %d" % self.runner.options.at_level
            self.runner.options.output.info(msg)

    def report(self):
        if not self.runner.do_run_tests:
            return
        if self.runner.options.resume_layer:
            return
        if self.runner.options.verbose:
            self.runner.options.output.tests_with_errors(self.runner.errors)
            self.runner.options.output.tests_with_failures(self.runner.failures)
