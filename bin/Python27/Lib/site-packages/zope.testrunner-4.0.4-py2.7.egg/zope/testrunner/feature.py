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
"""Generic features for the test runner.
"""

import zope.interface
import zope.testrunner.interfaces


class Feature(object):
    """A base class implementing no-op methods for the IFeature interface."""

    zope.interface.implements(zope.testrunner.interfaces.IFeature)

    active = False

    def __init__(self, runner):
        self.runner = runner

    def global_setup(self):
        """Executed once when the test runner is being set up."""
        pass

    def late_setup(self):
        """Executed once right before the actual tests get executed and after
        all global setups have happened.
        """
        pass

    def layer_setup(self, layer):
        """Executed once after a layer was set up."""
        pass

    def layer_teardown(self, layer):
        """Executed once after a layer was run."""
        pass

    def test_setup(self):
        """Executed once before each test."""
        pass

    def test_teardown(self):
        """Executed once after each test."""
        pass

    def early_teardown(self):
        """Executed once directly after all tests."""
        pass

    def global_teardown(self):
        """Executed once after all tests where run and early teardowns have
        happened."""
        pass

    def report(self):
        """Executed once after all tests have been run and all setup was
        torn down."""
        pass
