# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# __init__.py                Created on: 2013/07/23
#                            Author    : Duncan Grisby (dpg1)
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
#    License along with this library. If not, see http://www.gnu.org/licenses/
#
# Description:
#    omniORB-specifc policies

import omniORB
from omniORB import CORBA

ENDPOINT_PUBLISH_POLICY_TYPE = 0x41545402

class EndPointPublishPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omniorb.net/omniPolicy/EndPointPublishPolicy:1.0"

    def __init__(self, value):
        if not isinstance(value, list):
            raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)

        for item in value:
            if not isinstance(item, str):
                raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)

        self._value = value
        self._policy_type = ENDPOINT_PUBLISH_POLICY_TYPE

    def _get_value(self):
        return self._value

    value = property(_get_value)


def _create_policy(ptype, val):
    if ptype == 0x41545402:
        return EndPointPublishPolicy(val)

    return None

omniORB.policyMakers.append(_create_policy)
