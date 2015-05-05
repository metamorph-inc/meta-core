##############################################################################
#
# Copyright (c) 2004 Zope Foundation and Contributors.
# All Rights Reserved.
#
# This software is subject to the provisions of the Zope Public License,
# Version 2.0 (ZPL).  A copy of the ZPL should accompany this distribution.
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY AND ALL EXPRESS OR IMPLIED
# WARRANTIES ARE DISCLAIMED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF TITLE, MERCHANTABILITY, AGAINST INFRINGEMENT, AND FITNESS
# FOR A PARTICULAR PURPOSE.
#
##############################################################################
"""Sample test layers
"""

layer = '0' # Internal to samples. Not part of layer API
layerx = '0'

class Layer1:
    # Internal to samples. Not part of layer API:
    layer = '1'
    base = '0'
    layerx = '0'

    def setUp(self):
        global layer
        if layer != self.base:
            raise ValueError("Bad layer, %s, for %s." % (layer, self))
        layer = self.layer
    setUp = classmethod(setUp)

    def tearDown(self):
        global layer
        if layer != self.layer:
            raise ValueError("Bad layer, %s, for %s." % (layer, self))
        layer = self.base
    tearDown = classmethod(tearDown)

class Layerx:
    layerx = '1' # Internal to samples. Not part of layer API
    basex = '0'

    def setUp(self):
        global layerx
        if layerx != self.basex:
            raise ValueError("Bad layerx, %s, for %s." % (layerx, self))
        layerx = self.layerx
    setUp = classmethod(setUp)

    def tearDown(self):
        global layerx
        if layerx != self.layerx:
            raise ValueError("Bad layerx, %s, for %s." % (layerx, self))
        layerx = self.basex
    tearDown = classmethod(tearDown)

class Layer11(Layer1):
    layer = '11' # Internal to samples. Not part of layer API
    base  = '1'  # Internal to samples. Not part of layer API

class Layer12(Layer1):
    layer = '12' # Internal to samples. Not part of layer API
    base  = '1'  # Internal to samples. Not part of layer API

class Layer111(Layerx, Layer11):
    layer = '111' # Internal to samples. Not part of layer API
    base  = '11'  # Internal to samples. Not part of layer API
    layerx = '2' # Internal to samples. Not part of layer API
    basex = '1'

    def setUp(self):
        global layer
        if layer != self.base:
            raise ValueError("Bad layer, %s, for %s." % (layer, self))
        layer = self.layer
        global layerx
        if layerx != self.basex:
            raise ValueError("Bad layerx, %s, for %s." % (layerx, self))
        layerx = self.layerx
    setUp = classmethod(setUp)

    def tearDown(self):
        global layer
        if layer != self.layer:
            raise ValueError("Bad layer, %s, for %s." % (layer, self))
        layer = self.base
        global layerx
        if layerx != self.layerx:
            raise ValueError("Bad layerx, %s, for %s." % (layerx, self))
        layerx = self.basex
    tearDown = classmethod(tearDown)

class Layer121(Layer12):
    layer = '121' # Internal to samples. Not part of layer API
    base  = '12'  # Internal to samples. Not part of layer API

class Layer112(Layerx, Layer11):
    layer = '112' # Internal to samples. Not part of layer API
    base  = '11'  # Internal to samples. Not part of layer API
    layerx = '2' # Internal to samples. Not part of layer API
    basex = '1'

    def setUp(self):
        global layer
        if layer != self.base:
            raise ValueError("Bad layer, %s, for %s." % (layer, self))
        layer = self.layer
        global layerx
        if layerx != self.basex:
            raise ValueError("Bad layerx, %s, for %s." % (layerx, self))
        layerx = self.layerx
    setUp = classmethod(setUp)

    def tearDown(self):
        global layer
        if layer != self.layer:
            raise ValueError("Bad layer, %s, for %s." % (layer, self))
        layer = self.base
        global layerx
        if layerx != self.layerx:
            raise ValueError("Bad layerx, %s, for %s." % (layerx, self))
        layerx = self.basex
    tearDown = classmethod(tearDown)

class Layer122(Layer12):
    layer = '122' # Internal to samples. Not part of layer API
    base  = '12'  # Internal to samples. Not part of layer API
