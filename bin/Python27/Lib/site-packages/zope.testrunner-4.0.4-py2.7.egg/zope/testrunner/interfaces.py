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
"""Test runner interfaces

XXX Note: These interfaces are still being sketched out. Please do not rely on
them, yet.

"""

import zope.interface


class EndRun(Exception):
    """Indicate that the existing run call should stop

    Used to prevent additional test output after post-mortem debugging.

    """


class IFeature(zope.interface.Interface):
    """Features extend the test runners functionality in a pipe-lined
    order.
    """

    active = zope.interface.Attribute(
      "Flag whether this feature is activated. If it is not activated than "
      "its methods won't be called by the runner.")

    def global_setup():
        """Executed once when the test runner is being set up."""

    def late_setup():
        """Executed once right before the actual tests get executed and after
        all global setups have happened.

        Should do as little work as possible to avoid timing interferences
        with other features.

        It is guaranteed that the calling stack frame is not left until
        early_teardown was called.

        """

    def layer_setup(layer):
        """Executed once after a layer was set up."""

    def layer_teardown(layer):
        """Executed once after a layer was run."""

    def test_setup(test):
        """Executed once before each test."""

    def test_teardown(test):
        """Executed once after each test."""

    def early_teardown():
        """Executed once directly after all tests.

        This method should do as little as possible to avoid timing issues.

        It is guaranteed to be called directly from the same stack frame that
        called `late_setup`.

        """

    def global_teardown():
        """Executed once after all tests where run and early teardowns have
        happened.

        """

    def report():
        """Executed once after all tests have been run and all setup was
        torn down.

        This is the only method that should produce output.

        """


class ITestRunner(zope.interface.Interface):
    """The test runner manages test layers and their execution.

    The functionality of a test runner can be extended by configuring
    features.

    """

    options = zope.interface.Attribute(
      "Provides access to configuration options.")
