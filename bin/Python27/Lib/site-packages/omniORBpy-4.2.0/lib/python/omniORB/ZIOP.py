# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# ZIOP.py                    Created on: 2013/05/28
#                            Author    : Duncan Grisby (dgrisby)
#
#    Copyright (C) 2013 Apasphere Ltd
#
#    This file is part of the omniORBpy library
#
#    The omniORBpy library is free software; you can redistribute it
#    and/or modify it under the terms of the GNU Lesser General
#    Public License as published by the Free Software Foundation;
#    either version 2.1 of the License, or (at your option) any later
#    version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free
#    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
#    MA 02111-1307, USA
#
#
# Description:
#    ZIOP module

import omniORB.ziop_idl
import _omniZIOP
from omniORB import CORBA

import omniORB
omniORB.updateModule("omniORB.ZIOP")


# Policies

class CompressionEnablingPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/ZIOP/CompressionEnablingPolicy:1.0"

    def __init__(self, value):
        self._value       = value
        self._policy_type = COMPRESSION_ENABLING_POLICY_ID

    def _get_compression_enabled(self):
        return self._value

    compression_enabled = property(_get_compression_enabled)

class CompressionIdLevelListPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/ZIOP/CompressionIdLevelListPolicy:1.0"

    def __init__(self, value):
        self._value       = value
        self._policy_type = COMPRESSOR_ID_LEVEL_LIST_POLICY_ID

    def _get_compressor_ids(self):
        return self._value

    compressor_ids = property(_get_compressor_ids)

class CompressionLowValuePolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/ZIOP/CompressionLowValuePolicy:1.0"

    def __init__(self, value):
        self._value       = value
        self._policy_type = COMPRESSION_LOW_VALUE_POLICY_ID

    def _get_low_value(self):
        return self._value

    low_value = property(_get_low_value)

class CompressionMinRatioPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/ZIOP/CompressionMinRatioPolicy:1.0"

    def __init__(self, value):
        self._value       = value
        self._policy_type = COMPRESSION_MIN_RATIO_POLICY_ID

    def _get_ratio(self):
        return self._value

    ratio = property(_get_ratio)


def _create_policy(ptype, val):
    if ptype == COMPRESSION_ENABLING_POLICY_ID:
        return CompressionEnablingPolicy(val)

    if ptype == COMPRESSOR_ID_LEVEL_LIST_POLICY_ID:
        return CompressionIdLevelListPolicy(val)

    if ptype == COMPRESSION_LOW_VALUE_POLICY_ID:
        return CompressionLowValuePolicy(val)

    if ptype == COMPRESSION_MIN_RATIO_POLICY_ID:
        return CompressionMinRatioPolicy(val)

    return None

omniORB.policyMakers.append(_create_policy)
