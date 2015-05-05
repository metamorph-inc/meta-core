##############################################################################
#
# Copyright (c) 2001, 2002 Zope Foundation and Contributors.
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
"""Set up testing environment
"""

import sys
import traceback
import zope.exceptions.exceptionformatter
import zope.testrunner.feature


def format_exception(t, v, tb, limit=None, chain=None):
    if chain:
        values = traceback._iter_chain(v, tb)
    else:
        values = [(v, tb)]
    fmt = zope.exceptions.exceptionformatter.TextExceptionFormatter(
        limit=None, with_filenames=True)
    for v, tb in values:
        return fmt.formatException(t, v, tb)


def print_exception(t, v, tb, limit=None, file=None, chain=None):
    if chain:
        values = traceback._iter_chain(v, tb)
    else:
        values = [(v, tb)]
    if file is None:
        file = sys.stdout
    for v, tb in values:
        file.writelines(format_exception(t, v, tb, limit))


class Traceback(zope.testrunner.feature.Feature):

    active = True

    def global_setup(self):
        self.old_format = traceback.format_exception
        traceback.format_exception = format_exception

        self.old_print = traceback.print_exception
        traceback.print_exception = print_exception

    def global_teardown(self):
        traceback.format_exception = self.old_format
        traceback.print_exception = self.old_print
