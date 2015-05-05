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
"""Subprocess support.
"""

import sys
import zope.testrunner.feature


class SubProcess(zope.testrunner.feature.Feature):
    """Lists all tests in the report instead of running the tests."""

    def __init__(self, runner):
        super(SubProcess, self).__init__(runner)
        self.active = bool(runner.options.resume_layer)

    def global_setup(self):
        self.original_stderr = sys.stderr
        sys.stderr = sys.stdout
        if self.runner.options.processes > 1:
            # If we only have one subprocess, there's absolutely
            # no reason to squelch.  We will let the messages through in a
            # timely manner, if they have been requested. On the other hand, if
            # there are multiple processes, we do squelch to 0.
            self.runner.options.verbose = 0
        self.progress = False

    def report(self):
        sys.stdout.close()
        # Communicate with the parent.  The protocol is obvious:
        print >> self.original_stderr, self.runner.ran, \
                len(self.runner.failures), len(self.runner.errors)
        for test, exc_info in self.runner.failures:
            print >> self.original_stderr, ' '.join(str(test).strip().split('\n'))
        for test, exc_info in self.runner.errors:
            print >> self.original_stderr, ' '.join(str(test).strip().split('\n'))
        # You need to flush in Python 3, and it doesn't hurt in Python 2:
        self.original_stderr.flush()
