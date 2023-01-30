# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# BiDirPolicy.py             Created on: 2003/04/25
#                            Author    : Duncan Grisby (dgrisby)
#
#    Copyright (C) 2003-2013 Apasphere Ltd.
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
#    License along with this library. If not, see http://www.gnu.org/licenses/
#
#
# Description:
#    Definitions for BiDirPolicy module

import omniORB
from omniORB import CORBA

try:
    property
except NameError:
    def property(*args):
        return None


NORMAL = 0
BOTH   = 1

BIDIRECTIONAL_POLICY_TYPE = 37


class BidirectionalPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/BiDirPolicy/BidirectionalPolicy:1.0"

    def __init__(self, value):
        if value not in (NORMAL, BOTH):
            raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)
        self._value       = value
        self._policy_type = BIDIRECTIONAL_POLICY_TYPE

    def _get_value(self):
        return self._value

    value = property(_get_value)


def _create_policy(ptype, val):
    if ptype == BIDIRECTIONAL_POLICY_TYPE:
        return BidirectionalPolicy(val)
    return None

omniORB.policyMakers.append(_create_policy)


# typedef unsigned short BidirectionalPolicyValue

class BidrectionalPolicyValue:
    _NP_RepositoryId = "IDL:omg.org/BiDirPolicy/BidrectionalPolicyValue:1.0"
    def __init__(self, *args, **kw):
        raise RuntimeError("Cannot construct objects of this type.")
_d_BidrectionalPolicyValue  = omniORB.tcInternal.tv_ushort
_ad_BidrectionalPolicyValue = (omniORB.tcInternal.tv_alias, BidrectionalPolicyValue._NP_RepositoryId, "BidrectionalPolicyValue", omniORB.tcInternal.tv_ushort)
_tc_BidrectionalPolicyValue = omniORB.tcInternal.createTypeCode(_ad_BidrectionalPolicyValue)
omniORB.registerType(BidrectionalPolicyValue._NP_RepositoryId, _ad_BidrectionalPolicyValue, _tc_BidrectionalPolicyValue)

