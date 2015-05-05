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
"""Shuffle tests discovered before executing them.
"""

import time
import random
import unittest
import zope.testrunner.feature


class Shuffle(zope.testrunner.feature.Feature):
    """Take the tests found so far and shuffle them."""

    def __init__(self, runner):
        super(Shuffle, self).__init__(runner)
        self.active = runner.options.shuffle
        self.seed = runner.options.shuffle_seed
        if self.seed is None:
            # We can't rely on the random modules seed initialization because
            # we can't introspect the seed later for reporting.  This is a
            # simple emulation of what random.Random.seed does anyway.
            self.seed = long(time.time() * 256) # use fractional seconds

    def global_setup(self):
        rng = random.Random(self.seed)
        for layer, suite in list(self.runner.tests_by_layer_name.items()):
            # Test suites cannot be modified through a public API.  We thus
            # take a mutable copy of the list of tests of that suite, shuffle
            # that and replace the test suite instance with a new one of the
            # same class.
            tests = list(suite)
            rng.shuffle(tests)
            self.runner.tests_by_layer_name[layer] = suite.__class__(tests)

    def report(self):
        msg = "Tests were shuffled using seed number %d." % self.seed
        self.runner.options.output.info(msg)
