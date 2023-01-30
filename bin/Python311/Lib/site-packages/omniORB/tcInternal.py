# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# tcInternal.py              Created on: 1999/06/24
#                            Author    : Duncan Grisby (dpg1)
#
#    Copyright (C) 2002-2014 Apasphere Ltd
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
#    TypeCode internal implementation

import omniORB

# To avoid a circular dependency, we manually extract some definitions
# from the partial CORBA module.

c = omniORB._CORBA_dict
g = globals()

for k, v in c.items():
    if k.startswith("tk_") or k in ("BAD_PARAM",
                                    "COMPLETED_NO",
                                    "VM_TRUNCATABLE",
                                    "INTERNAL",
                                    "BAD_TYPECODE",
                                    "TCKind",
                                    "TypeCode"):
        g[k] = v

del c, g, k, v

# The TypeCode implementation is based upon descriptors consisting of
# Python tuples, as used by the marshalling code. Although the public
# interface to TypeCodes presents a graph of connected TypeCode
# objects, the graph is actually only fully-stored by the descriptor
# tuple. A root TypeCode object has a reference to the top of the
# descriptor tuple. Non-root TypeCode objects are only created when
# requested.

# Kinds as numbers:
tv_null               = 0
tv_void               = 1
tv_short              = 2
tv_long               = 3
tv_ushort             = 4
tv_ulong              = 5
tv_float              = 6
tv_double             = 7
tv_boolean            = 8
tv_char               = 9
tv_octet              = 10
tv_any                = 11
tv_TypeCode           = 12
tv_Principal          = 13
tv_objref             = 14
tv_struct             = 15
tv_union              = 16
tv_enum               = 17
tv_string             = 18
tv_sequence           = 19
tv_array              = 20
tv_alias              = 21
tv_except             = 22
tv_longlong           = 23
tv_ulonglong          = 24
tv_longdouble         = 25
tv_wchar              = 26
tv_wstring            = 27
tv_fixed              = 28
tv_value              = 29
tv_value_box          = 30
tv_native             = 31
tv_abstract_interface = 32
tv_local_interface    = 33
tv__indirect          = -1


# Create a TypeCode given a class or a repoId

def typeCodeFromClassOrRepoId(t):
    try:
        t = t._NP_RepositoryId
    except AttributeError:
        pass

    if not isinstance(t, str):
        raise TypeError("Argument must be CORBA class or repository id.")

    d = omniORB.findType(t)
    if d is None:
        raise TypeError("Unknown CORBA type.")

    return createTypeCode(d)


# Implementations of public ORB TypeCode creation functions

def createStructTC(id, name, members):
    dlist  = [tv_struct, None, id, name]
    mnames = []
    for m in members:
        mnames.append(m.name)
        dlist.append(m.name)
        dlist.append(m.type._d)
    str = omniORB.createUnknownStruct(id, mnames)
    dlist[1] = str
    d = tuple(dlist)
    return createTypeCode(d)

def createUnionTC(id, name, discriminator_type, members):
    mlist   = []
    count   = 0
    defused = -1
    mmap    = {}
    for m in members:
        val = m.label.value()
        if m.label.typecode().kind() == tk_octet and val == 0:
            val     = -1
            defused = count

        tup = (val, m.name, m.type._d)
        if defused != count: mmap[val] = tup
        mlist.append(tup)
        count = count + 1

    union = omniORB.createUnknownUnion(id, defused, mlist)

    if defused >= 0:
        default = mlist[defused]
    else:
        default = None

    d = (tv_union, union, id, name, discriminator_type._k._v,
         defused, tuple(mlist), default, mmap)
    return createTypeCode(d)

def createEnumTC(id, name, members):
    mlist = []
    count = 0

    for m in members:
        mlist.append(omniORB.EnumItem(m, count))
        count = count + 1

    d = (tv_enum, id, name, tuple(mlist))
    return createTypeCode(d)

def createAliasTC(id, name, original_type):
    d = (tv_alias, id, name, original_type._d)
    return createTypeCode(d)

def createExceptionTC(id, name, members):
    dlist  = [tv_except, None, id, name]
    mnames = []
    for m in members:
        mnames.append(m.name)
        dlist.append(m.name)
        dlist.append(m.type._d)
    exc = omniORB.createUnknownException(id, mnames)
    dlist[1] = exc
    d = tuple(dlist)
    return createTypeCode(d)

def createInterfaceTC(id, name):
    d = (tv_objref, id, name)
    return createTypeCode(d)

def createStringTC(bound):
    d = (tv_string, bound)
    return createTypeCode(d)

def createWStringTC(bound):
    d = (tv_wstring, bound)
    return createTypeCode(d)

def createFixedTC(digits, scale):
    if digits < 1 or digits > 31 or scale > digits:
        raise BAD_PARAM(omniORB.BAD_PARAM_InvalidFixedPointLimits, COMPLETED_NO)

    d = (tv_fixed, digits, scale)
    return createTypeCode(d)

def createSequenceTC(bound, element_type):
    d = (tv_sequence, element_type._d, bound)
    return createTypeCode(d)

def createArrayTC(length, element_type):
    d = (tv_array, element_type._d, length)
    return createTypeCode(d)

def createRecursiveTC(id):
    class recursivePlaceHolder(object): pass
    recursivePlaceHolder._d = (tv__indirect, [id])
    return recursivePlaceHolder()

def createValueTC(id, name, modifier, base, members):
    base_desc = base._d

    if modifier == VM_TRUNCATABLE:
        if base_desc == tv_null:
            raise BAD_PARAM(omniORB.BAD_PARAM_InvalidTypeCode, COMPLETED_NO)
        base_ids = base_desc[5]
        if base_ids is None:
            base_ids = (id, base_desc[2])
        else:
            base_ids = (id,) + base_ids
    else:
        base_ids = None

    dlist = [tv_value, omniORB.createUnknownValue(id, base_desc),
             id, name, modifier, base_ids, base_desc]
    for m in members:
        dlist.append(m.name)
        dlist.append(m.type._d)
        dlist.append(m.access)

    return createTypeCode(tuple(dlist))

def createValueBoxTC(id, name, boxed_type):
    d = (tv_value_box, omniORB.createUnknownValue(id, tv_null),
         id, name, boxed_type._d)
    return createTypeCode(d)

def createAbstractInterfaceTC(id, name):
    d = (tv_abstract_interface, id, name)
    return createTypeCode(d)

def createLocalInterfaceTC(id, name):
    d = (tv_local_interface, id, name)
    return createTypeCode(d)





# Function to create a TypeCode object given a descriptor. Returns a
# static (stub generated) TypeCode object if possible.

typeCodeMapping = omniORB.typeCodeMapping

def createTypeCode(d):
    try:
        r = basicTypeCodes.get(d)
        if r is not None:
            return r
    except TypeError:
        # Happens if d contains a mutable object
        pass

    if not isinstance (d, tuple):
        raise INTERNAL()

    k = d[0]

    if   k == tv_string:  return TypeCode_string(d)
    elif k == tv_wstring: return TypeCode_wstring(d)
    elif k == tv_fixed:   return TypeCode_fixed(d)

    elif k in [ tv_objref, tv_abstract_interface, tv_local_interface ]:
        tc = typeCodeMapping.get(d[1])
        if tc is None:
            tc = TypeCode_objref(d)
        return tc

    elif k == tv_struct:
        tc = typeCodeMapping.get(d[2])
        if tc is None:
            tc = TypeCode_struct(d)
        return tc
    
    elif k == tv_union:
        tc = typeCodeMapping.get(d[2])
        if tc is None:
            tc = TypeCode_union(d)
        return tc
    
    elif k == tv_enum:
        tc = typeCodeMapping.get(d[1])
        if tc is None:
            tc = TypeCode_enum(d)
        return tc

    elif k == tv_sequence:  return TypeCode_sequence(d)
    elif k == tv_array:     return TypeCode_array(d)

    elif k == tv_alias:
        tc = typeCodeMapping.get(d[1])
        if tc is None:
            tc = TypeCode_alias(d)
        return tc
    
    elif k == tv_except:
        tc = typeCodeMapping.get(d[2])
        if tc is None:
            tc = TypeCode_except(d)
        return tc

    elif k == tv_value:
        tc = typeCodeMapping.get(d[2])
        if tc is None:
            tc = TypeCode_value(d)
        return tc

    elif k == tv_value_box:
        tc = typeCodeMapping.get(d[2])
        if tc is None:
            tc = TypeCode_value_box(d)
        return tc

    elif k == tv__indirect:
        if isinstance(d[1][0], str):
            nd = omniORB.findType(d[1][0])
            if nd is None:
                raise BAD_TYPECODE(omniORB.BAD_TYPECODE_InvalidIndirection,
                                   COMPLETED_NO)
            d[1][0] = nd
        return createTypeCode(d[1][0])

    raise INTERNAL()


# TypeCode base interface

class TypeCode_base (TypeCode):
    def __init__(self):
        self._d = 0
        self._k = tk_null

    def equal(self, tc):
        try:
            return self._d == tc._d
        
        except AttributeError:
            raise BAD_PARAM(omniORB.BAD_PARAM_WrongPythonType, COMPLETED_NO)

    def equivalent(self, tc):
        return self.equal(tc)

    def get_compact_typecode(self):
        return self

    def kind(self):
        return self._k

    # Operations which are only available for some kinds:
    def id(self):                       raise TypeCode.BadKind()
    def name(self):                     raise TypeCode.BadKind()
    def member_count(self):             raise TypeCode.BadKind()
    def member_name(self, index):       raise TypeCode.BadKind()
    def member_type(self, index):       raise TypeCode.BadKind()
    def member_label(self, index):      raise TypeCode.BadKind()

    def discriminator_type(self):       raise TypeCode.BadKind()
    def default_index(self):            raise TypeCode.BadKind()
    def length(self):                   raise TypeCode.BadKind()
    def content_type(self):             raise TypeCode.BadKind()

    def fixed_digits(self):             raise TypeCode.BadKind()
    def fixed_scale(self):              raise TypeCode.BadKind()

    def member_visibility(self, index): raise TypeCode.BadKind()
    def type_modifier(self):            raise TypeCode.BadKind()
    def concrete_base_type(self):       raise TypeCode.BadKind()


# Class for short, long, ushort, ulong, float, double, boolean, char,
# octet, any, TypeCode, Principal, longlong, ulonglong, longdouble:

class TypeCode_empty (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, int): raise INTERNAL()
        if desc not in ( tv_null, tv_void, tv_short, tv_long, tv_ushort,
                         tv_ulong, tv_float, tv_double, tv_boolean, tv_char,
                         tv_octet, tv_any, tv_TypeCode, tv_Principal,
                         tv_longlong, tv_ulonglong, tv_longdouble, tv_wchar ):
            raise INTERNAL()

        self._d = desc
        self._k = TCKind._item(desc)

    def __repr__(self):
        return "CORBA.TC" + str(self._k)[8:]

# string:
class TypeCode_string (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_string:
            raise INTERNAL()
        self._d = desc
        self._k = tk_string

    def length(self):
        return self._d[1]

    def __repr__(self):
        if self._d[1] == 0:
            return "CORBA.TC_string"
        else:
            return "orb.create_string_tc(bound=%d)" % self._d[1]

# wstring:
class TypeCode_wstring (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_wstring:
            raise INTERNAL()
        self._d = desc
        self._k = tk_wstring

    def length(self):
        return self._d[1]

    def __repr__(self):
        if self._d[1] == 0:
            return "CORBA.TC_wstring"
        else:
            return "orb.create_wstring_tc(bound=%d)" % self._d[1]

# fixed:
class TypeCode_fixed (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_fixed:
            raise INTERNAL()
        self._d = desc
        self._k = tk_fixed

    def fixed_digits(self):
        return self._d[1]

    def fixed_scale(self):
        return self._d[2]

    def __repr__(self):
        return "orb.create_fixed_tc(digits=%d,scale=%d)" % (
            self.fixed_digits(), self.fixed_scale())

# objref:
class TypeCode_objref (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or \
               desc[0] not in [ tv_objref,
                                tv_abstract_interface,
                                tv_local_interface ]:
            raise INTERNAL()
        self._d = desc
        self._k = TCKind._items[desc[0]]

    def id(self):
        if self._d[1] is not None:
            return self._d[1]
        else:
            return ""
        
    def name(self): return self._d[2]

    def __repr__(self):
        return 'CORBA.TypeCode("%s")' % self.id()


# struct:
class TypeCode_struct (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_struct:
            raise INTERNAL()
        self._d = desc
        self._k = tk_struct

    def equivalent(self, tc):
        return equivalentDescriptors(self._d, tc._d)

    def get_compact_typecode(self):
        return TypeCode_struct(getCompactDescriptor(self._d), None)

    def id(self):                 return self._d[2]
    def name(self):               return self._d[3]
    def member_count(self):       return (len(self._d) - 4) / 2
    def member_name(self, index):
        off = index * 2 + 4
        if index < 0 or off >= len(self._d): raise TypeCode.Bounds()
        return self._d[off]

    def member_type(self, index):
        off = index * 2 + 5
        if index < 0 or off >= len(self._d): raise TypeCode.Bounds()
        return createTypeCode(self._d[off])

    def __repr__(self):
        return 'CORBA.TypeCode("%s")' % self.id()

    
# union:
class TypeCode_union (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_union:
            raise INTERNAL()
        self._d = desc
        self._k = tk_union

    def equivalent(self, tc):
        return equivalentDescriptors(self._d, tc._d)

    def get_compact_typecode(self):
        return TypeCode_union(getCompactDescriptor(self._d), None)

    def id(self):                  return self._d[2]
    def name(self):                return self._d[3]
    def member_count(self):        return len(self._d[6])

    def member_name(self, index):
        if index < 0 or index >= len(self._d[6]): raise TypeCode.Bounds()
        return self._d[6][index][1]
    
    def member_type(self, index):
        if index < 0 or index >= len(self._d[6]): raise TypeCode.Bounds()
        return createTypeCode(self._d[6][index][2])

    def member_label(self, index):
        if index < 0 or index >= len(self._d[6]): raise TypeCode.Bounds()
        if index == self._d[5]: return CORBA.Any(_tc_octet, 0)
        return CORBA.Any(createTypeCode(self._d[4]), self._d[6][index][0])

    def discriminator_type(self): return createTypeCode(self._d[4])

    def default_index(self):
        if self._d[5] >= 0: return self._d[5]
        return -1

    def __repr__(self):
        return 'CORBA.TypeCode("%s")' % self.id()


# enum:
class TypeCode_enum (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_enum:
            raise INTERNAL()
        self._d = desc
        self._k = tk_enum

    def equivalent(self, tc):
        return equivalentDescriptors(self._d, tc._d)

    def get_compact_typecode(self):
        return TypeCode_enum(getCompactDescriptor(self._d))

    def id(self):           return self._d[1]
    def name(self):         return self._d[2]
    def member_count(self): return len(self._d[3])

    def member_name(self, index):
        if index < 0 or index >= len(self._d[3]): raise TypeCode.Bounds()
        return self._d[3][index]._n

    def __repr__(self):
        return 'CORBA.TypeCode("%s")' % self.id()


# sequence:
class TypeCode_sequence (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_sequence:
            raise INTERNAL()
        self._d = desc
        self._k = tk_sequence

    def equivalent(self, tc):
        return equivalentDescriptors(self._d, tc._d)

    def get_compact_typecode(self):
        return TypeCode_sequence(getCompactDescriptor(self._d), None)

    def length(self):
        return self._d[2]

    def content_type(self):
        return createTypeCode(self._d[1])

    def __repr__(self):
        return "orb.create_sequence_tc(bound=%d, element_type=%s)" % (
            self.length(), repr(self.content_type()))


# array:
class TypeCode_array (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_array:
            raise INTERNAL()
        self._d = desc
        self._k = tk_array

    def equivalent(self, tc):
        return equivalentDescriptors(self._d, tc._d)

    def get_compact_typecode(self):
        return TypeCode_sequence(getCompactDescriptor(self._d), None)

    def length(self):
        return self._d[2]

    def content_type(self):
        return createTypeCode(self._d[1])

    def __repr__(self):
        return "orb.create_array_tc(length=%d, element_type=%s)" % (
            self.length(), repr(self.content_type()))

# alias:
class TypeCode_alias (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_alias:
            raise INTERNAL()
        self._d = desc
        self._k = tk_alias

    def equivalent(self, tc):
        return equivalentDescriptors(self._d, tc._d)

    def get_compact_typecode(self):
        return TypeCode_alias(getCompactDescriptor(self._d), None)

    def id(self):           return self._d[1]
    def name(self):         return self._d[2]

    def content_type(self):
        return createTypeCode(self._d[3])

    def __repr__(self):
        return 'CORBA.TypeCode("%s")' % self.id()

# except:
class TypeCode_except (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_except:
            raise INTERNAL()
        self._d = desc
        self._k = tk_except

    def equivalent(self, tc):
        return equivalentDescriptors(self._d, tc._d)

    def get_compact_typecode(self):
        return TypeCode_except(getCompactDescriptor(self._d), None)

    def id(self):                 return self._d[2]
    def name(self):               return self._d[3]
    def member_count(self):       return (len(self._d) - 4) / 2
    def member_name(self, index):
        off = index * 2 + 4
        if index < 0 or off >= len(self._d): raise TypeCode.Bounds()
        return self._d[off]

    def member_type(self, index):
        off = index * 2 + 5
        if index < 0 or off >= len(self._d): raise TypeCode.Bounds()
        return createTypeCode(self._d[off])

    def __repr__(self):
        return 'CORBA.TypeCode("%s")' % self.id()


# value:
class TypeCode_value (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_value:
            raise INTERNAL()
        self._d = desc
        self._k = tk_value

    def equivalent(self, tc):
        return equivalentDescriptors(self._d, tc._d)

    def get_compact_typecode(self):
        return TypeCode_value(getCompactDescriptor(self._d), None)

    def id(self):                 return self._d[2]
    def name(self):               return self._d[3]
    def member_count(self):       return (len(self._d) - 7) / 3
    def member_name(self, index):
        off = index * 3 + 7
        if index < 0 or off >= len(self._d): raise TypeCode.Bounds()
        return self._d[off]

    def member_type(self, index):
        off = index * 3 + 8
        if index < 0 or off >= len(self._d): raise TypeCode.Bounds()
        return createTypeCode(self._d[off])

    def member_visibility(self, index):
        off = index * 3 + 9
        if index < 0 or off >= len(self._d): raise TypeCode.Bounds()
        return self._d[off]

    def type_modifier(self):
        return self._d[4]

    def concrete_base_type(self):
        if self._d[6] == tv_null:
            return None
        else:
            return createTypeCode(self._d[6])

    def __repr__(self):
        return 'CORBA.TypeCode("%s")' % self.id()


# valuebox:
class TypeCode_value_box (TypeCode_base):
    def __init__(self, desc):
        if not isinstance(desc, tuple) or desc[0] != tv_value_box:
            raise INTERNAL()
        self._d = desc
        self._k = tk_value_box

    def equivalent(self, tc):
        return equivalentDescriptors(self._d, tc._d)

    def get_compact_typecode(self):
        return TypeCode_alias(getCompactDescriptor(self._d), None)

    def id(self):           return self._d[2]
    def name(self):         return self._d[3]

    def content_type(self):
        return createTypeCode(self._d[4])

    def __repr__(self):
        return 'CORBA.TypeCode("%s")' % self.id()


# Map of pre-created basic TypeCodes
basicTypeCodes = {
    tv_null:      TypeCode_empty(tv_null),
    tv_void:      TypeCode_empty(tv_void),
    tv_short:     TypeCode_empty(tv_short),
    tv_long:      TypeCode_empty(tv_long),
    tv_ushort:    TypeCode_empty(tv_ushort),
    tv_ulong:     TypeCode_empty(tv_ulong),
    tv_float:     TypeCode_empty(tv_float),
    tv_double:    TypeCode_empty(tv_double),
    tv_boolean:   TypeCode_empty(tv_boolean),
    tv_char:      TypeCode_empty(tv_char),
    tv_octet:     TypeCode_empty(tv_octet),
    tv_any:       TypeCode_empty(tv_any),
    tv_TypeCode:  TypeCode_empty(tv_TypeCode),
    tv_Principal: TypeCode_empty(tv_Principal),
    tv_longlong:  TypeCode_empty(tv_longlong),
    tv_ulonglong: TypeCode_empty(tv_ulonglong),
    tv_longdouble:TypeCode_empty(tv_longdouble),
    tv_wchar:     TypeCode_empty(tv_wchar),

    (tv_string, 0): TypeCode_string ((tv_string,  0)),
    (tv_wstring,0): TypeCode_wstring((tv_wstring, 0)),
}


# Functions to test descriptor equivalence
def equivalentDescriptors(a, b, seen=None, a_ids=None, b_ids=None):

    if seen is None:
        seen  = {}
        a_ids = {}
        b_ids = {}

    try:
        if a == b: return 1

        # If they don't trivially match, they must be tuples:
        if not (isinstance(a, tuple) and isinstance(b, tuple)):
            return 0

        # Follow aliases and indirections
        while (isinstance(a, tuple) and
               (a[0] == tv_alias or a[0] == tv__indirect)):

            if a[0] == tv_alias:
                if a[1] != "": a_ids[a[1]] = a
                a = a[3]
            else:
                if isinstance(a[1][0], str):
                    a = a_ids[a[1][0]]
                else:
                    a = a[1][0]

        while (isinstance(b, tuple) and
               (b[0] == tv_alias or b[0] == tv__indirect)):
            
            if b[0] == tv_alias:
                if b[1] != "": b_ids[b[1]] = b
                b = b[3]
            else:
                if isinstance(b[1][0], str):
                    b = b_ids[b[1][0]]
                else:
                    b = b[1][0]

        # Re-do the trivial checks on the underlying types.
        if a == b: return 1

        if not (isinstance(a, tuple) and isinstance(b, tuple)):
            return 0

        # Handle cycles
        idt = id(a), id(b)
        if idt in seen:
            return 1

        seen[idt] = None

        # Must be same kind
        if a[0] != b[0]:
            return 0

        if a[0] == tv_struct:
            # id
            if a[2] != "": a_ids[a[2]] = a
            if b[2] != "": b_ids[b[2]] = b

            if a[2] != "" and b[2] != "":
                if a[2] == b[2]:
                    return 1
                else:
                    return 0

            # members:
            if len(a) != len(b):
                return 0
            
            for i in range(4, len(a), 2):
                # Member type
                if not equivalentDescriptors(a[i+1], b[i+1],
                                             seen, a_ids, b_ids):
                    return 0
            return 1

        elif a[0] == tv_union:
            # id
            if a[2] != "": a_ids[a[2]] = a
            if b[2] != "": b_ids[b[2]] = b

            if a[2] != "" and b[2] != "":
                if a[2] == b[2]:
                    return 1
                else:
                    return 0

            # discriminant type
            if not equivalentDescriptors(a[4], b[4], seen, a_ids, b_ids):
                return 0

            # default index
            if a[5] != b[5]:
                return 0

            # Members
            if len(a[6]) != len(b[6]):
                return 0

            for i in range(len(a[6])):
                # Member label
                if a[6][i][0] != b[6][i][0]:
                    return 0

                # Member descriptor
                if not equivalentDescriptors(a[6][i][2], b[6][i][2],
                                             seen, a_ids, b_ids):
                    return 0

            return 1

        elif a[0] == tv_enum:
            # id
            if a[1] != "": a_ids[a[1]] = a
            if b[1] != "": b_ids[b[1]] = b

            if a[1] != "" and b[1] != "":
                if a[1] == b[1]:
                    return 1
                else:
                    return 0

            # Members
            if len(a[3]) != len(b[3]):
                return 0

            return 1

        elif a[0] == tv_sequence:
            # Bound
            if a[2] != b[2]:
                return 0

            # Type
            return equivalentDescriptors(a[1], b[1], seen, a_ids, b_ids)

        elif a[0] == tv_array:
            # Length
            if a[2] != b[2]:
                return 0

            # Type
            return equivalentDescriptors(a[1], b[1], seen, a_ids, b_ids)

        elif a[0] == tv_except:
            # id
            if a[2] != "": a_ids[a[2]] = a
            if b[2] != "": b_ids[b[2]] = b

            if a[2] != "" and b[2] != "":
                if a[2] == b[2]:
                    return 1
                else:
                    return 0

                # members:
                if len(a) != len(b):
                    return 0

                for i in range(4, len(self._d), 2):
                    # Member type
                    if not equivalentDescriptors(a[i+1], b[i+1],
                                                 seen, a_ids, b_ids):
                        return 0
            return 1

        elif a[0] == tv_value:
            # id
            if a[2] != "": a_ids[a[2]] = a
            if b[2] != "": b_ids[b[2]] = b

            if a[2] != "" and b[2] != "":
                if a[2] == b[2]:
                    return 1
                else:
                    return 0

            # members
            if len(a) != len(b):
                return 0

            for i in range(7, len(a), 3):
                # Access spec
                if a[i+2] != b[i+2]:
                    return 0
                
                if not equivalentDescriptors(a[i+1], b[i+1],
                                             seen, a_ids, b_ids):
                    return 0

            return 1

        elif a[0] == tv_value_box:
            # id
            if a[2] != "": a_ids[a[2]] = a
            if b[2] != "": b_ids[b[2]] = b

            if a[2] != "" and b[2] != "":
                if a[2] == b[2]:
                    return 1
                else:
                    return 0

            # Boxed type
            if equivalentDescriptors(a[4], b[4], seen, a_ids, b_ids):
                return 1
            else:
                return 0

        return 0

    except AttributeError:
        raise BAD_PARAM(BAD_PARAM_WrongPythonType, COMPLETED_NO)


# Functions to compact descriptors:
def getCompactDescriptor(d):
    seen = {}
    ind  = []
    r    = r_getCompactDescriptor(d, seen, ind)

    # Fix up indirections:
    for i in ind:
        try:
            if isinstance(i[0], str):
                i[0] = seen[i[0]]
            else:
                i[0] = seen[id(i[0])]
        except KeyError:
            raise BAD_TYPECODE(BAD_TYPECODE_InvalidIndirection, COMPLETED_NO)

    return r

def r_getCompactDescriptor(d, seen, ind):
    if isinstance(d, tuple):
        k = d[0]
    else:
        k = d

    if   k == tv_short:     r = d
    elif k == tv_long:      r = d
    elif k == tv_ushort:    r = d
    elif k == tv_ulong:     r = d
    elif k == tv_float:     r = d
    elif k == tv_double:    r = d
    elif k == tv_boolean:   r = d
    elif k == tv_char:      r = d
    elif k == tv_octet:     r = d
    elif k == tv_any:       r = d
    elif k == tv_TypeCode:  r = d
    elif k == tv_Principal: r = d
    elif k == tv_string:    r = d
    elif k == tv_objref:    r = d
    elif k == tv_longlong:  r = d
    elif k == tv_ulonglong: r = d
    elif k == tv_longdouble:r = d
    
    elif k == tv_struct:
        c = list(d)
        c[3] = ""
        for i in range(4, len(c), 2):
            c[i]   = ""
            c[i+1] = r_getCompactDescriptor(d[i+1], seen, ind)

        r = tuple(c)
        seen[d[2]] = r
        seen[id(d)] = r
    
    elif k == tv_union:
        c = list(d)
        c[3] = ""
        c[4] = r_getCompactDescriptor(d[4], seen, ind)

        m = []
        for u in d[6]:
            m.append((u[0], "", r_getCompactDescriptor(u[2], seen, ind)))

        c[6] = tuple(m)

        if d[7] is not None:
            c[7] = (d[7][0], "", r_getCompactDescriptor(d[7][2], seen, ind))

        r = tuple(c)
        seen[d[2]] = r
        seen[id(d)] = r
        
    elif k == tv_enum:
        m = []
        for e in d[3]:
            m.append(omniORB.AnonymousEnumItem(e._v))
        r = (k, d[1], "", tuple(m))

    elif k == tv_sequence:
        r = (k, r_getCompactDescriptor(d[1], seen, ind), d[2])
        
    elif k == tv_array:
        r = (k, r_getCompactDescriptor(d[1], seen, ind), d[2])

    elif k == tv_alias:
        r = (k, d[1], "", r_getCompactDescriptor(d[3], seen, ind))

    elif k == tv_except:
        c = list(d)
        c[3] = ""
        for i in range(4, len(c), 2):
            c[i]   = ""
            c[i+1] = r_getCompactDescriptor(d[i+1], seen, ind)

        r = tuple(c)

    elif k == tv_value:
        c = list(d)
        c[3] = ""
        for i in range(7, len(c), 3):
            c[i]   = ""
            c[i+1] = r_getCompactDescriptor(d[i+1], seen, ind)

        r = tuple(c)
        seen[d[2]] = r
        seen[id(d)] = r
    
    elif k == tv_value_box:
        c = list(d)
        c[3] = ""
        c[4] = r_getCompactDescriptor(d[4], seen, ind)
        r = tuple(c)
        seen[d[2]] = r
        seen[id(d)] = r

    elif k == tv_abstract_interface:
        r = d

    elif k == tv_local_interface:
        r = d

    elif k == tv__indirect:
        l = [d[1][0]]
        ind.append(l)
        r = (k, l)

    else: raise INTERNAL()

    return r
