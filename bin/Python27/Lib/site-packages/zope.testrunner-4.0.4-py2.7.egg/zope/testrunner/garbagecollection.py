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
"""Garbage collection support.
"""

import gc
import sys
import zope.testrunner.feature


class Threshold(zope.testrunner.feature.Feature):

    def __init__(self, runner):
        super(Threshold, self).__init__(runner)
        self.threshold = self.runner.options.gc
        self.active = bool(self.threshold)

        if not self.active:
            return

        if len(self.threshold) > 3:
            self.runner.options.output.error("Too many --gc options")
            sys.exit(1)

    def global_setup(self):
        self.old_threshold = gc.get_threshold()

        if self.threshold[0]:
            self.runner.options.output.info(
                "Cyclic garbage collection threshold set to: %s" %
                repr(tuple(self.threshold)))
        else:
            self.runner.options.output.info(
                "Cyclic garbage collection is disabled.")

        gc.set_threshold(*self.threshold)

    def global_teardown(self):
        gc.set_threshold(*self.old_threshold)


class Debug(zope.testrunner.feature.Feature):
    """Manages garbage collection debug flags."""

    def __init__(self, runner):
        super(Debug, self).__init__(runner)
        self.flags = self.runner.options.gc_option
        self.active = bool(self.flags)

        if not self.active:
            return

    def global_setup(self):
        # Set garbage collection debug flags
        self.old_flags = gc.get_debug()
        new_flags = 0
        for op in self.flags:
            new_flags |= getattr(gc, op)
        gc.set_debug(new_flags)

    def global_teardown(self):
        gc.set_debug(self.old_flags)
