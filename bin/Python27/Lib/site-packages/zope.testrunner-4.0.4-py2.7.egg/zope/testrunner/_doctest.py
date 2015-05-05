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
"""Doc test support for the test runner.
"""

import sys
import doctest
import zope.testrunner.feature


class DocTest(zope.testrunner.feature.Feature):

    active = True

    def global_setup(self):
        options = self.runner.options
        output = options.output

        self.old_reporting_flags = doctest.set_unittest_reportflags(0)

        reporting_flags = 0
        if options.ndiff:
            reporting_flags = doctest.REPORT_NDIFF
        if options.udiff:
            if reporting_flags:
                output.error("Can only give one of --ndiff, --udiff, or --cdiff")
                sys.exit(1)
            reporting_flags = doctest.REPORT_UDIFF
        if options.cdiff:
            if reporting_flags:
                output.error("Can only give one of --ndiff, --udiff, or --cdiff")
                sys.exit(1)
            reporting_flags = doctest.REPORT_CDIFF
        if options.report_only_first_failure:
            reporting_flags |= doctest.REPORT_ONLY_FIRST_FAILURE

        if reporting_flags:
            doctest.set_unittest_reportflags(reporting_flags)

    def global_shutdown(self):
        doctest.set_unittest_reportflags(self.old_reporting_flags)

# Use a special exception for the test runner.
from zope.testrunner.exceptions import DocTestFailureException
doctest.DocTestCase.failureException = DocTestFailureException
