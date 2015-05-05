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
"""Logging support.

This code is pretty much untested and was only mechanically refactored.

The module name is not 'logging' because of a name collision with Python's
logging module.
"""

import logging
import logging.config
import os.path

import zope.testrunner.feature


class Logging(zope.testrunner.feature.Feature):

    active = True

    def global_setup(self):
        # Get the log.ini file from the current directory instead of
        # possibly buried in the build directory.  TODO: This isn't
        # perfect because if log.ini specifies a log file, it'll be
        # relative to the build directory.  Hmm...  logini =
        # os.path.abspath("log.ini")

        logini = os.path.abspath("log.ini")
        if os.path.exists(logini):
            logging.config.fileConfig(logini)
        else:
            # If there's no log.ini, cause the logging package to be
            # silent during testing.
            root = logging.getLogger()
            root.addHandler(NullHandler())
            logging.basicConfig()

        if os.environ.has_key("LOGGING"):
            level = int(os.environ["LOGGING"])
            logging.getLogger().setLevel(level)


class NullHandler(logging.Handler):
    """Logging handler that drops everything on the floor.

    We require silence in the test environment.  Hush.
    """

    def emit(self, record):
        pass
