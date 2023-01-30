# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# URI.py                     Created on: 2000/06/26
#                            Author    : Duncan Grisby (dpg1)
#
#    Copyright (C) 2002-2014 Apasphere Ltd
#    Copyright (C) 2000 AT&T Laboratories Cambridge
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
#    URI handling functions

import types, string, re
import CosNaming
from omniORB import CORBA


__regex = re.compile(r"([/\.\\])")


def stringToName(sname):
    """stringToName(string) -> CosNaming.Name

Convert a stringified name to a CosNaming.Name"""

    # Try to understand this at your peril... :-)
    #
    # It works by splitting the input string into a list. Each item in
    # the list is either a string fragment, or a single "special"
    # character -- ".", "/", or "\". It then walks over the list,
    # building a list of NameComponents, based on the meanings of the
    # special characters.

    if not isinstance(sname, str):
        raise CORBA.BAD_PARAM(omniORB.BAD_PARAM_WrongPythonType, COMPLETED_NO)

    if sname == "":
        raise CosNaming.NamingContext.InvalidName()

    parts   = __regex.split(sname)
    name    = [CosNaming.NameComponent("","")]
    dotseen = 0

    parts = [_f for _f in parts if _f]
    parts.reverse()
    while parts:
        part = parts.pop()

        if part == "\\":
            if not parts:
                raise CosNaming.NamingContext.InvalidName()
            part = parts.pop()
            if part != "\\" and part != "/" and part != ".":
                raise CosNaming.NamingContext.InvalidName()

        elif part == ".":
            if dotseen:
                raise CosNaming.NamingContext.InvalidName()
            dotseen = 1
            continue

        elif part == "/":
            if not parts:
                raise CosNaming.NamingContext.InvalidName()
            
            if dotseen:
                if name[-1].kind == "" and name[-1].id != "":
                    raise CosNaming.NamingContext.InvalidName()
            else:
                if name[-1].id == "":
                    raise CosNaming.NamingContext.InvalidName()

            dotseen = 0
            name.append(CosNaming.NameComponent("",""))
            continue

        if dotseen:
            name[-1].kind = name[-1].kind + part
        else:
            name[-1].id = name[-1].id + part

    return name



def nameToString(name):
    """nameToString(CosNaming.Name) -> string

Convert the CosNaming.Name into its stringified form."""

    parts = []

    if not isinstance(name, (list, tuple)):
        raise CORBA.BAD_PARAM(omniORB.BAD_PARAM_WrongPythonType, COMPLETED_NO)

    if len(name) == 0:
        raise CosNaming.NamingContext.InvalidName()

    try:
        for nc in name:
            if nc.id == "" and nc.kind == "":
                parts.append(".")
            elif nc.kind == "":
                parts.append(__regex.sub(r"\\\1", nc.id))
            else:
                parts.append(__regex.sub(r"\\\1", nc.id) + "." +
                             __regex.sub(r"\\\1", nc.kind))
    except AttributeError:
        raise CORBA.BAD_PARAM(omniORB.BAD_PARAM_WrongPythonType, COMPLETED_NO)

    return "/".join(parts)


def addrAndNameToURI(addr, sname):
    """addrAndNameToURI(addr, sname) -> URI

Create a valid corbaname URI from an address string and a stringified name"""

    # *** Note that this function does not properly check the address
    # string. It should raise InvalidAddress if the address looks
    # invalid.

    import urllib.request, urllib.parse, urllib.error

    if not (isinstance(addr, str) and isinstance(sname, str)):
        raise CORBA.BAD_PARAM(omniORB.BAD_PARAM_WrongPythonType, COMPLETED_NO)

    if addr == "":
        raise CosNaming.NamingContextExt.InvalidAddress()

    if sname == "":
        return "corbaname:" + addr
    else:
        stringToName(sname) # This might raise InvalidName
        return "corbaname:" + addr + "#" + urllib.parse.quote(sname)
