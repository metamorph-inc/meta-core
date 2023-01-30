# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# PortableServer.py          Created on: 1999/09/22
#                            Author    : Duncan Grisby (dpg1)
#
#    Copyright (C) 2005-2013 Apasphere Ltd
#    Copyright (C) 1999 AT&T Laboratories Cambridge
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
#    PortableServer module

import _omnipy
import omniORB
from omniORB import CORBA

try:
    property
except NameError:
    def property(*args):
        return None


# native Servant
class Servant(object):
    _NP_RepositoryId = ""

    def _this(self):
        return _omnipy.servantThis(self)

    def _default_POA(self):
        if omniORB.rootPOA: return omniORB.rootPOA
        assert(omniORB.orb)
        omniORB.rootPOA = omniORB.orb.resolve_initial_references("RootPOA")
        return omniORB.rootPOA

    def _get_interface(self):
        omniORB.importIRStubs() # Make sure IR stubs are loaded
        ir = omniORB.orb.resolve_initial_references("InterfaceRepository")
        ir = ir._narrow(CORBA.Repository)
        if ir is None:
            raise CORBA.INTF_REPOS(omniORB.INTF_REPOS_NotAvailable,
                                   CORBA.COMPLETED_NO)
        interf = ir.lookup_id(self._NP_RepositoryId)
        return interf._narrow(CORBA.InterfaceDef)

    def _non_existent(self):
        return 0

_d_Servant = omniORB.tcInternal.tv_native


# interface POAManager
class POAManager (CORBA.Object) :
    _NP_RepositoryId = "IDL:omg.org/PortableServer/POAManager:1.0"

    def __init__(self, pm):
        CORBA.Object.__init__(self, pm)

    def activate(self):
        self._obj.activate()

    def hold_requests(self, wait_for_completion):
        self._obj.hold_requests(wait_for_completion)
    
    def discard_requests(self, wait_for_completion):
        self._obj.discard_requests(wait_for_completion)
    
    def deactivate(self, etherialize_objects, wait_for_completion):
        self._obj.deactivate(etherialize_objects, wait_for_completion)

    def get_state(self):
        return self.State._item(self._obj.get_state())


    # Generated declarations

    class AdapterInactive (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POAManager/AdapterInactive:1.0"

    _d_AdapterInactive  = (omniORB.tcInternal.tv_except, AdapterInactive,
                           AdapterInactive._NP_RepositoryId, "AdapterInactive")
    _tc_AdapterInactive = omniORB.tcInternal.createTypeCode(_d_AdapterInactive)
    omniORB.registerType(AdapterInactive._NP_RepositoryId,
                         _d_AdapterInactive, _tc_AdapterInactive)
    
    HOLDING    = omniORB.EnumItem("HOLDING", 0)
    ACTIVE     = omniORB.EnumItem("ACTIVE", 1)
    DISCARDING = omniORB.EnumItem("DISCARDING", 2)
    INACTIVE   = omniORB.EnumItem("INACTIVE", 3)
    State = omniORB.Enum("IDL:omg.org/PortableServer/POAManager/State:1.0",
                         (HOLDING, ACTIVE, DISCARDING, INACTIVE))
    
    _d_State  = (omniORB.tcInternal.tv_enum, State._NP_RepositoryId,
                 "State", State._items)
    _tc_State = omniORB.tcInternal.createTypeCode(_d_State)
    omniORB.registerType(State._NP_RepositoryId, _d_State, _tc_State)



# interface POA
_d_POA = (omniORB.tcInternal.tv_objref,
          "IDL:omg.org/PortableServer/POA:1.0", "POA")

class POA (CORBA.Object) :
    """POA implementation."""
    
    _NP_RepositoryId = _d_POA[1]

    def __init__(self, poa):
        CORBA.Object.__init__(self, poa);

    def create_POA(self, adapter_name, a_POAManager, policies):
        if a_POAManager is None:
            pm = None
        else:
            pm = a_POAManager._obj
            
        return self._obj.create_POA(adapter_name, pm, policies)

    def find_POA(self, adapter_name, activate_it):
        return self._obj.find_POA(adapter_name, activate_it)

    def destroy(self, etherialize_objects, wait_for_completion):
        self._obj.destroy(etherialize_objects, wait_for_completion)

    def create_thread_policy(self, value):
        return ThreadPolicy(value)

    def create_lifespan_policy(self, value):
        return LifespanPolicy(value)

    def create_id_uniqueness_policy(self, value):
        return IdUniquenessPolicy(value)

    def create_id_assignment_policy(self, value):
        return IdAssignmentPolicy(value)

    def create_implicit_activation_policy(self, value):
        return ImplicitActivationPolicy(value)

    def create_servant_retention_policy(self, value):
        return ServantRetentionPolicy(value)

    def create_request_processing_policy(self, value):
        return RequestProcessingPolicy(value)

    def _get_the_name(self):
        return self._obj._get_the_name()

    the_name = property(_get_the_name)

    def _get_the_parent(self):
        return self._obj._get_the_parent()

    the_parent = property(_get_the_parent)

    def _get_the_children(self):
        return self._obj._get_the_children()

    the_children = property(_get_the_children)

    def _get_the_POAManager(self):
        try:
            return self.__manager
        except AttributeError:
            self.__manager = self._obj._get_the_POAManager()
            return self.__manager

    the_POAManager = property(_get_the_POAManager)

    def _get_the_activator(self):
        return self._obj._get_the_activator()

    def _set_the_activator(self, value):
        return self._obj._set_the_activator(value)

    the_activator = property(_get_the_activator, _set_the_activator)

    def get_servant_manager(self):
        return self._obj.get_servant_manager()

    def set_servant_manager(self, imgr):
        return self._obj.set_servant_manager(imgr)

    def get_servant(self):
        return self._obj.get_servant()

    def set_servant(self, p_servant):
        return self._obj.set_servant(p_servant)

    def activate_object(self, p_servant):
        return self._obj.activate_object(p_servant)

    def activate_object_with_id(self, id, p_servant):
        return self._obj.activate_object_with_id(id, p_servant)

    def deactivate_object(self, oid):
        return self._obj.deactivate_object(oid)

    def create_reference(self, intf):
        return self._obj.create_reference(intf)

    def create_reference_with_id(self, oid, intf):
        return self._obj.create_reference_with_id(oid, intf)

    def servant_to_id(self, p_servant):
        return self._obj.servant_to_id(p_servant)

    def servant_to_reference(self, p_servant):
        return self._obj.servant_to_reference(p_servant)

    def reference_to_servant(self, reference):
        return self._obj.reference_to_servant(reference)

    def reference_to_id(self, reference):
        return self._obj.reference_to_id(reference)

    def id_to_servant(self, oid):
        return self._obj.id_to_servant(oid)

    def id_to_reference(self, oid):
        return self._obj.id_to_reference(oid)


    # Generated exception declarations
    # exception AdapterAlreadyExists
    class AdapterAlreadyExists (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/AdapterAlreadyExists:1.0"

    _d_AdapterAlreadyExists  = (omniORB.tcInternal.tv_except,
                                AdapterAlreadyExists,
                                AdapterAlreadyExists._NP_RepositoryId,
                                "AdapterAlreadyExists")
    _tc_AdapterAlreadyExists = omniORB.tcInternal.createTypeCode(_d_AdapterAlreadyExists)
    omniORB.registerType(AdapterAlreadyExists._NP_RepositoryId,
                         _d_AdapterAlreadyExists, _tc_AdapterAlreadyExists)
    
    # exception AdapterInactive
    class AdapterInactive (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/AdapterInactive:1.0"
    
    _d_AdapterInactive  = (omniORB.tcInternal.tv_except, AdapterInactive,
                           AdapterInactive._NP_RepositoryId, "AdapterInactive")
    _tc_AdapterInactive = omniORB.tcInternal.createTypeCode(_d_AdapterInactive)
    omniORB.registerType(AdapterInactive._NP_RepositoryId,
                         _d_AdapterInactive, _tc_AdapterInactive)
    
    # exception AdapterNonExistent
    class AdapterNonExistent (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/AdapterNonExistent:1.0"
    
    _d_AdapterNonExistent  = (omniORB.tcInternal.tv_except,
                              AdapterNonExistent,
                              AdapterNonExistent._NP_RepositoryId,
                              "AdapterNonExistent")
    _tc_AdapterNonExistent = omniORB.tcInternal.createTypeCode(_d_AdapterNonExistent)
    omniORB.registerType(AdapterNonExistent._NP_RepositoryId,
                         _d_AdapterNonExistent, _tc_AdapterNonExistent)
    
    # exception InvalidPolicy
    class InvalidPolicy (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/InvalidPolicy:1.0"
    
        def __init__(self, index):
            self.index = index
    
    _d_InvalidPolicy  = (omniORB.tcInternal.tv_except, InvalidPolicy,
                         InvalidPolicy._NP_RepositoryId, "InvalidPolicy",
                         "index", omniORB.tcInternal.tv_ushort)
    _tc_InvalidPolicy = omniORB.tcInternal.createTypeCode(_d_InvalidPolicy)
    omniORB.registerType(InvalidPolicy._NP_RepositoryId,
                         _d_InvalidPolicy, _tc_InvalidPolicy)
    
    # exception NoServant
    class NoServant (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/NoServant:1.0"
    
    _d_NoServant  = (omniORB.tcInternal.tv_except, NoServant,
                     NoServant._NP_RepositoryId, "NoServant")
    _tc_NoServant = omniORB.tcInternal.createTypeCode(_d_NoServant)
    omniORB.registerType(NoServant._NP_RepositoryId,
                         _d_NoServant, _tc_NoServant)
    
    # exception ObjectAlreadyActive
    class ObjectAlreadyActive (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/ObjectAlreadyActive:1.0"
    
    _d_ObjectAlreadyActive  = (omniORB.tcInternal.tv_except,
                               ObjectAlreadyActive,
                               ObjectAlreadyActive._NP_RepositoryId,
                               "ObjectAlreadyActive")
    _tc_ObjectAlreadyActive = omniORB.tcInternal.createTypeCode(_d_ObjectAlreadyActive)
    omniORB.registerType(ObjectAlreadyActive._NP_RepositoryId,
                         _d_ObjectAlreadyActive, _tc_ObjectAlreadyActive)
    
    # exception ObjectNotActive
    class ObjectNotActive (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/ObjectNotActive:1.0"
    
    _d_ObjectNotActive  = (omniORB.tcInternal.tv_except, ObjectNotActive,
                           ObjectNotActive._NP_RepositoryId, "ObjectNotActive")
    _tc_ObjectNotActive = omniORB.tcInternal.createTypeCode(_d_ObjectNotActive)
    omniORB.registerType(ObjectNotActive._NP_RepositoryId,
                         _d_ObjectNotActive, _tc_ObjectNotActive)
    
    # exception ServantAlreadyActive
    class ServantAlreadyActive (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/ServantAlreadyActive:1.0"
    
    _d_ServantAlreadyActive  = (omniORB.tcInternal.tv_except,
                                ServantAlreadyActive,
                                ServantAlreadyActive._NP_RepositoryId,
                                "ServantAlreadyActive")
    _tc_ServantAlreadyActive = omniORB.tcInternal.createTypeCode(_d_ServantAlreadyActive)
    omniORB.registerType(ServantAlreadyActive._NP_RepositoryId,
                         _d_ServantAlreadyActive, _tc_ServantAlreadyActive)
    
    # exception ServantNotActive
    class ServantNotActive (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/ServantNotActive:1.0"
    
    _d_ServantNotActive  = (omniORB.tcInternal.tv_except, ServantNotActive,
                            ServantNotActive._NP_RepositoryId,
                            "ServantNotActive")
    _tc_ServantNotActive = omniORB.tcInternal.createTypeCode(_d_ServantNotActive)
    omniORB.registerType(ServantNotActive._NP_RepositoryId,
                         _d_ServantNotActive, _tc_ServantNotActive)
    
    # exception WrongAdapter
    class WrongAdapter (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/WrongAdapter:1.0"
    
    _d_WrongAdapter  = (omniORB.tcInternal.tv_except, WrongAdapter,
                        WrongAdapter._NP_RepositoryId, "WrongAdapter")
    _tc_WrongAdapter = omniORB.tcInternal.createTypeCode(_d_WrongAdapter)
    omniORB.registerType(WrongAdapter._NP_RepositoryId,
                         _d_WrongAdapter, _tc_WrongAdapter)
    
    # exception WrongPolicy
    class WrongPolicy (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/POA/WrongPolicy:1.0"
    
    _d_WrongPolicy  = (omniORB.tcInternal.tv_except, WrongPolicy,
                       WrongPolicy._NP_RepositoryId, "WrongPolicy")
    _tc_WrongPolicy = omniORB.tcInternal.createTypeCode(_d_WrongPolicy)
    omniORB.registerType(WrongPolicy._NP_RepositoryId,
                         _d_WrongPolicy, _tc_WrongPolicy)


# interface Current
class Current (CORBA.Object) :
    _NP_RepositoryId = "IDL:omg.org/PortableServer/Current:1.0"

    def __init__(self, pc):
        CORBA.Object.__init__(self, pc)

    def get_POA(self):
        return self._obj.get_POA()

    def get_object_id(self):
        return self._obj.get_object_id()

    def get_reference(self):
        return self._obj.get_reference()

    def get_servant(self):
        return self._obj.get_servant()


    # Generated declarations

    class NoContext (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/PortableServer/Current/NoContext:1.0"

    _d_NoContext  = (omniORB.tcInternal.tv_except, NoContext,
                     NoContext._NP_RepositoryId, "NoContext")
    _tc_NoContext = omniORB.tcInternal.createTypeCode(_d_NoContext)
    omniORB.registerType(NoContext._NP_RepositoryId,
                         _d_NoContext, _tc_NoContext)


# Generated declarations

# ObjectId
class ObjectId(object):
    _NP_RepositoryId = "IDL:omg.org/PortableServer/ObjectId:1.0"
    def __init__(self):
        raise RuntimeError("Cannot construct objects of this type.")

_d_ObjectId  = (omniORB.tcInternal.tv_sequence, omniORB.tcInternal.tv_octet, 0)
_ad_ObjectId = (omniORB.tcInternal.tv_alias,
                "IDL:omg.org/PortableServer/ObjectId:1.0", "ObjectId",
                (omniORB.tcInternal.tv_sequence,
                 omniORB.tcInternal.tv_octet, 0))
_tc_ObjectId = omniORB.tcInternal.createTypeCode(_ad_ObjectId)


# exception ForwardRequest
class ForwardRequest (CORBA.UserException):
    _NP_RepositoryId = "IDL:omg.org/PortableServer/ForwardRequest:1.0"

    def __init__(self, forward_reference):
        self.forward_reference = forward_reference

_d_ForwardRequest  = (omniORB.tcInternal.tv_except, ForwardRequest,
                      ForwardRequest._NP_RepositoryId, "ForwardRequest",
                      "forward_reference", CORBA._d_Object)
_tc_ForwardRequest = omniORB.tcInternal.createTypeCode(_d_ForwardRequest)
omniORB.registerType(ForwardRequest._NP_RepositoryId,
                     _d_ForwardRequest, _tc_ForwardRequest)


# Policies

def _create_policy(ptype, val):
    if ptype == 16:
        return ThreadPolicy(val)
    elif ptype == 17:
        return LifespanPolicy(val)
    elif ptype == 18:
        return IdUniquenessPolicy(val)
    elif ptype == 19:
        return IdAssignmentPolicy(val)
    elif ptype == 20:
        return ImplicitActivationPolicy(val)
    elif ptype == 21:
        return ServantRetentionPolicy(val)
    elif ptype == 22:
        return RequestProcessingPolicy(val)
    return None

omniORB.policyMakers.append(_create_policy)


class ThreadPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/PortableServer/ThreadPolicy:1.0"

    def __init__(self, value):
        if value not in ThreadPolicyValue._items:
            raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)
        self._value       = value
        self._policy_type = 16

    def _get_value(self):
        return self._value

    value = property(_get_value)


class LifespanPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/PortableServer/LifespanPolicy:1.0"

    def __init__(self, value):
        if value not in LifespanPolicyValue._items:
            raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)
        self._value       = value
        self._policy_type = 17

    def _get_value(self):
        return self._value

    value = property(_get_value)


class IdUniquenessPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/PortableServer/IdUniquenessPolicy:1.0"

    def __init__(self, value):
        if value not in IdUniquenessPolicyValue._items:
            raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)
        self._value       = value
        self._policy_type = 18

    def _get_value(self):
        return self._value

    value = property(_get_value)


class IdAssignmentPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/PortableServer/IdAssignmentPolicy:1.0"

    def __init__(self, value):
        if value not in IdAssignmentPolicyValue._items:
            raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)
        self._value       = value
        self._policy_type = 19

    def _get_value(self):
        return self._value

    value = property(_get_value)


class ImplicitActivationPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/PortableServer/ImplicitActivationPolicy:1.0"

    def __init__(self, value):
        if value not in ImplicitActivationPolicyValue._items:
            raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)
        self._value       = value
        self._policy_type = 20

    def _get_value(self):
        return self._value

    value = property(_get_value)


class ServantRetentionPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/PortableServer/ServantRetentionPolicy:1.0"

    def __init__(self, value):
        if value not in ServantRetentionPolicyValue._items:
            raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)
        self._value       = value
        self._policy_type = 21

    def _get_value(self):
        return self._value

    value = property(_get_value)


class RequestProcessingPolicy (CORBA.Policy):
    _NP_RepositoryId = "IDL:omg.org/PortableServer/RequestProcessingPolicy:1.0"

    def __init__(self, value):
        if value not in RequestProcessingPolicyValue._items:
            raise CORBA.PolicyError(CORBA.BAD_POLICY_VALUE)
        self._value       = value
        self._policy_type = 22

    def _get_value(self):
        return self._value

    value = property(_get_value)


# enum ThreadPolicyValue
ORB_CTRL_MODEL      = omniORB.EnumItem("ORB_CTRL_MODEL", 0)
SINGLE_THREAD_MODEL = omniORB.EnumItem("SINGLE_THREAD_MODEL", 1)
MAIN_THREAD_MODEL   = omniORB.EnumItem("MAIN_THREAD_MODEL", 2)
ThreadPolicyValue   = omniORB.Enum(\
    "IDL:omg.org/PortableServer/ThreadPolicyValue:1.0",
    (ORB_CTRL_MODEL, SINGLE_THREAD_MODEL, MAIN_THREAD_MODEL))

_d_ThreadPolicyValue  = (omniORB.tcInternal.tv_enum,
                         ThreadPolicyValue._NP_RepositoryId,
                         "ThreadPolicyValue", ThreadPolicyValue._items)
_tc_ThreadPolicyValue = omniORB.tcInternal.createTypeCode(_d_ThreadPolicyValue)
omniORB.registerType(ThreadPolicyValue._NP_RepositoryId,
                     _d_ThreadPolicyValue, _tc_ThreadPolicyValue)

# enum LifespanPolicyValue
TRANSIENT  = omniORB.EnumItem("TRANSIENT", 0)
PERSISTENT = omniORB.EnumItem("PERSISTENT", 1)
LifespanPolicyValue = omniORB.Enum(\
    "IDL:omg.org/PortableServer/LifespanPolicyValue:1.0",
    (TRANSIENT, PERSISTENT))

_d_LifespanPolicyValue  = (omniORB.tcInternal.tv_enum,
                           LifespanPolicyValue._NP_RepositoryId,
                           "LifespanPolicyValue", LifespanPolicyValue._items)
_tc_LifespanPolicyValue = omniORB.tcInternal.createTypeCode(_d_LifespanPolicyValue)
omniORB.registerType(LifespanPolicyValue._NP_RepositoryId,
                     _d_LifespanPolicyValue, _tc_LifespanPolicyValue)

# enum IdUniquenessPolicyValue
UNIQUE_ID   = omniORB.EnumItem("UNIQUE_ID", 0)
MULTIPLE_ID = omniORB.EnumItem("MULTIPLE_ID", 1)
IdUniquenessPolicyValue = omniORB.Enum(\
    "IDL:omg.org/PortableServer/IdUniquenessPolicyValue:1.0",
    (UNIQUE_ID, MULTIPLE_ID))

_d_IdUniquenessPolicyValue  = (omniORB.tcInternal.tv_enum,
                               IdUniquenessPolicyValue._NP_RepositoryId,
                               "IdUniquenessPolicyValue",
                               IdUniquenessPolicyValue._items)
_tc_IdUniquenessPolicyValue = omniORB.tcInternal.createTypeCode(_d_IdUniquenessPolicyValue)
omniORB.registerType(IdUniquenessPolicyValue._NP_RepositoryId,
                     _d_IdUniquenessPolicyValue, _tc_IdUniquenessPolicyValue)

# enum IdAssignmentPolicyValue
USER_ID   = omniORB.EnumItem("USER_ID", 0)
SYSTEM_ID = omniORB.EnumItem("SYSTEM_ID", 1)
IdAssignmentPolicyValue = omniORB.Enum(\
    "IDL:omg.org/PortableServer/IdAssignmentPolicyValue:1.0",
    (USER_ID, SYSTEM_ID))

_d_IdAssignmentPolicyValue  = (omniORB.tcInternal.tv_enum,
                               IdAssignmentPolicyValue._NP_RepositoryId,
                               "IdAssignmentPolicyValue",
                               IdAssignmentPolicyValue._items)
_tc_IdAssignmentPolicyValue = omniORB.tcInternal.createTypeCode(_d_IdAssignmentPolicyValue)
omniORB.registerType(IdAssignmentPolicyValue._NP_RepositoryId,
                     _d_IdAssignmentPolicyValue, _tc_IdAssignmentPolicyValue)


# enum ImplicitActivationPolicyValue
IMPLICIT_ACTIVATION    = omniORB.EnumItem("IMPLICIT_ACTIVATION", 0)
NO_IMPLICIT_ACTIVATION = omniORB.EnumItem("NO_IMPLICIT_ACTIVATION", 1)
ImplicitActivationPolicyValue = omniORB.Enum(\
    "IDL:omg.org/PortableServer/ImplicitActivationPolicyValue:1.0",
    (IMPLICIT_ACTIVATION, NO_IMPLICIT_ACTIVATION))

_d_ImplicitActivationPolicyValue  = (omniORB.tcInternal.tv_enum,
                                     ImplicitActivationPolicyValue._NP_RepositoryId,
                                     "ImplicitActivationPolicyValue",
                                     ImplicitActivationPolicyValue._items)
_tc_ImplicitActivationPolicyValue = omniORB.tcInternal.createTypeCode(_d_ImplicitActivationPolicyValue)
omniORB.registerType(ImplicitActivationPolicyValue._NP_RepositoryId,
                     _d_ImplicitActivationPolicyValue,
                     _tc_ImplicitActivationPolicyValue)

# enum ServantRetentionPolicyValue
RETAIN     = omniORB.EnumItem("RETAIN", 0)
NON_RETAIN = omniORB.EnumItem("NON_RETAIN", 1)
ServantRetentionPolicyValue = omniORB.Enum(\
    "IDL:omg.org/PortableServer/ServantRetentionPolicyValue:1.0",
    (RETAIN, NON_RETAIN))

_d_ServantRetentionPolicyValue  = (omniORB.tcInternal.tv_enum,
                                   ServantRetentionPolicyValue._NP_RepositoryId,
                                   "ServantRetentionPolicyValue",
                                   ServantRetentionPolicyValue._items)
_tc_ServantRetentionPolicyValue = omniORB.tcInternal.createTypeCode(_d_ServantRetentionPolicyValue)
omniORB.registerType(ServantRetentionPolicyValue._NP_RepositoryId,
                     _d_ServantRetentionPolicyValue,
                     _tc_ServantRetentionPolicyValue)

# enum RequestProcessingPolicyValue
USE_ACTIVE_OBJECT_MAP_ONLY = omniORB.EnumItem("USE_ACTIVE_OBJECT_MAP_ONLY", 0)
USE_DEFAULT_SERVANT        = omniORB.EnumItem("USE_DEFAULT_SERVANT", 1)
USE_SERVANT_MANAGER        = omniORB.EnumItem("USE_SERVANT_MANAGER", 2)
RequestProcessingPolicyValue = omniORB.Enum(\
    "IDL:omg.org/PortableServer/RequestProcessingPolicyValue:1.0",
    (USE_ACTIVE_OBJECT_MAP_ONLY, USE_DEFAULT_SERVANT, USE_SERVANT_MANAGER))

_d_RequestProcessingPolicyValue  = (omniORB.tcInternal.tv_enum,
                                    RequestProcessingPolicyValue._NP_RepositoryId,
                                    "RequestProcessingPolicyValue",
                                    RequestProcessingPolicyValue._items)
_tc_RequestProcessingPolicyValue = omniORB.tcInternal.createTypeCode(_d_RequestProcessingPolicyValue)
omniORB.registerType(RequestProcessingPolicyValue._NP_RepositoryId,
                     _d_RequestProcessingPolicyValue,
                     _tc_RequestProcessingPolicyValue)


# ServantManagers

# interface ServantManager
_d_ServantManager = (omniORB.tcInternal.tv_local_interface,
                     "IDL:omg.org/PortableServer/ServantManager:1.0",
                     "ServantManager")

class ServantManager (CORBA.LocalObject):
    _NP_RepositoryId = _d_ServantManager[1]

    _nil = CORBA.Object._nil

_tc_ServantManager = omniORB.tcInternal.createTypeCode(_d_ServantManager)
omniORB.registerType(ServantManager._NP_RepositoryId,
                     _d_ServantManager, _tc_ServantManager)

# ServantManager object reference
class _objref_ServantManager (CORBA.Object):
    _NP_RepositoryId = ServantManager._NP_RepositoryId

    def __init__(self, obj):
        CORBA.Object.__init__(self, obj)

omniORB.registerObjref(ServantManager._NP_RepositoryId, _objref_ServantManager)


# interface ServantActivator
_d_ServantActivator = (omniORB.tcInternal.tv_local_interface,
                       "IDL:omg.org/PortableServer/ServantActivator:1.0",
                       "ServantActivator")

class ServantActivator (ServantManager):
    _NP_RepositoryId = _d_ServantActivator[1]

    _nil = CORBA.Object._nil

    def incarnate(self, oid, adapter):
        raise CORBA.NO_IMPLEMENT(omniORB.NO_IMPLEMENT_NoPythonMethod,
                                 CORBA.COMPLETED_NO)

    def etherialize(self, oid, adapter, serv,
                    cleanup_in_progress, remaining_activations):
        raise CORBA.NO_IMPLEMENT(omniORB.NO_IMPLEMENT_NoPythonMethod,
                                 CORBA.COMPLETED_NO)


_tc_ServantActivator = omniORB.tcInternal.createTypeCode(_d_ServantActivator)
omniORB.registerType(ServantActivator._NP_RepositoryId,
                     _d_ServantActivator, _tc_ServantActivator)

ServantActivator._d_incarnate = ((_d_ObjectId, _d_POA), (_d_Servant, ),
                                 {ForwardRequest._NP_RepositoryId:
                                  _d_ForwardRequest})

ServantActivator._d_etherealize = ((_d_ObjectId, _d_POA, _d_Servant,
                                    omniORB.tcInternal.tv_boolean,
                                    omniORB.tcInternal.tv_boolean), (), None)


# ServantActivator object reference
class _objref_ServantActivator (_objref_ServantManager):
    _NP_RepositoryId = ServantActivator._NP_RepositoryId

    def __init__(self, obj):
        _objref_ServantManager.__init__(self, obj)

    def incarnate(self, oid, adapter):
        return self._obj.invoke("incarnate",
                                ServantActivator._d_incarnate,
                                (oid, adapter))

    def etherealize(self, oid, adapter, serv, cleanup_in_progress,
                    remaining_activations):
        return self._obj.invoke("etherialize",
                                ServantActivator._d_etherialize,
                                (oid, adapter, serv, cleanup_in_progress,
                                 remaining_activations))

omniORB.registerObjref(ServantActivator._NP_RepositoryId,
                       _objref_ServantActivator)


# interface ServantLocator
_d_ServantLocator = (omniORB.tcInternal.tv_local_interface,
                     "IDL:omg.org/PortableServer/ServantLocator:1.0",
                     "ServantLocator")

class ServantLocator (ServantManager):
    _NP_RepositoryId = _d_ServantLocator[1]

    _nil = CORBA.Object._nil

    _d_Cookie = omniORB.tcInternal.tv_native

    def preinvoke(self, oid, adapter, operation):
        raise CORBA.NO_IMPLEMENT(omniORB.NO_IMPLEMENT_NoPythonMethod,
                                 CORBA.COMPLETED_NO)

    def postinvoke(self, oid, adapter, operations, the_cookie, the_servant):
        raise CORBA.NO_IMPLEMENT(omniORB.NO_IMPLEMENT_NoPythonMethod,
                                 CORBA.COMPLETED_NO)

_tc_ServantLocator = omniORB.tcInternal.createTypeCode(_d_ServantLocator)
omniORB.registerType(ServantLocator._NP_RepositoryId,
                     _d_ServantLocator, _tc_ServantLocator)

# ServantLocator operations and attributes
ServantLocator._d_preinvoke = ((_d_ObjectId, _d_POA, CORBA._d_Identifier),
                               (_d_Servant, ServantLocator._d_Cookie),
                               {ForwardRequest._NP_RepositoryId:
                                _d_ForwardRequest})

ServantLocator._d_postinvoke = ((_d_ObjectId, _d_POA, CORBA._d_Identifier,
                                 ServantLocator._d_Cookie, _d_Servant), (),
                                None)

# ServantLocator object reference
class _objref_ServantLocator (_objref_ServantManager):
    _NP_RepositoryId = ServantLocator._NP_RepositoryId

    def __init__(self, obj):
        _objref_ServantManager.__init__(self, obj)

    def preinvoke(self, oid, adapter, operation):
        return self._obj.invoke("preinvoke",
                                ServantLocator._d_preinvoke,
                                (oid, adapter, operation))

    def postinvoke(self, oid, adapter, operation, the_cookie, the_servant):
        return self._obj.invoke("postinvoke",
                                ServantLocator._d_postinvoke,
                                (oid, adapter, operation,
                                 the_cookie, the_servant))

omniORB.registerObjref(ServantLocator._NP_RepositoryId, _objref_ServantLocator)



# AdapterActivator

# interface AdapterActivator
_d_AdapterActivator = (omniORB.tcInternal.tv_local_interface,
                       "IDL:omg.org/PortableServer/AdapterActivator:1.0",
                       "AdapterActivator")

class AdapterActivator (CORBA.LocalObject):
    _NP_RepositoryId = _d_AdapterActivator[1]

    _nil = CORBA.Object._nil

    def unknown_adapter(self, parent, name):
        raise CORBA.NO_IMPLEMENT(omniORB.NO_IMPLEMENT_NoPythonMethod,
                                 CORBA.COMPLETED_NO)


_tc_AdapterActivator = omniORB.tcInternal.createTypeCode(_d_AdapterActivator)
omniORB.registerType(AdapterActivator._NP_RepositoryId,
                     _d_AdapterActivator, _tc_AdapterActivator)

# AdapterActivator operations and attributes
AdapterActivator._d_unknown_adapter = ((_d_POA,
                                        (omniORB.tcInternal.tv_string,0)),
                                       (omniORB.tcInternal.tv_boolean, ), None)

# AdapterActivator object reference
class _objref_AdapterActivator (CORBA.Object):
    _NP_RepositoryId = AdapterActivator._NP_RepositoryId

    def __init__(self, obj):
        CORBA.Object.__init__(self, obj)

    def unknown_adapter(self, parent, name):
        return self._obj.invoke("unknown_adapter",
                                AdapterActivator._d_unknown_adapter,
                                (parent, name))

omniORB.registerObjref(AdapterActivator._NP_RepositoryId,
                       _objref_AdapterActivator)
