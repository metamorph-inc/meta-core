# -*- python -*-
#                           Package   : omniidl
# idlutil.py                Created on: 1999/10/27
#			    Author    : Duncan Grisby (dpg1)
#
#    Copyright (C) 2003-2011 Apasphere Ltd
#    Copyright (C) 1999      AT&T Laboratories Cambridge
#
#  This file is part of omniidl.
#
#  omniidl is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
#  02111-1307, USA.
#
# Description:
#   
#   Utility functions

"""Utility functions for IDL compilers

escapifyString()  -- return a string with non-printing characters escaped.
escapifyWString() -- return a wstring with non-printing characters escaped.
reprFloat()       -- return a string represenation of an IDL float type.
slashName()       -- format a scoped name with '/' separating components.
dotName()         -- format a scoped name with '.' separating components.
ccolonName()      -- format a scoped name with '::' separating components.
pruneScope()      -- remove common prefix from a scoped name.
relativeScope()   -- give a minimal name for one scope relative to another."""

def slashName(scopedName, our_scope=[]):
    """slashName(list, [list]) -> string

Return a scoped name given as a list of strings as a single string
with the components separated by '/' characters. If a second list is
given, remove a common prefix using pruneScope()."""
    
    pscope = pruneScope(scopedName, our_scope)
    return "/".join(pscope)

def dotName(scopedName, our_scope=[]):
    """dotName(list, [list]) -> string

Return a scoped name given as a list of strings as a single string
with the components separated by '.' characters. If a second list is
given, remove a common prefix using pruneScope()."""
    
    pscope = pruneScope(scopedName, our_scope)
    return ".".join(pscope)

def ccolonName(scopedName, our_scope=[]):
    """ccolonName(list, [list]) -> string

Return a scoped name given as a list of strings as a single string
with the components separated by '::' strings. If a second list is
given, remove a common prefix using pruneScope()."""
    
    pscope = pruneScope(scopedName, our_scope)

    if pscope[0] is None:
        return "::" + "::".join(pscope[1:])
    else:
        return "::".join(pscope)

def pruneScope(target_scope, our_scope):
    """pruneScope(list A, list B) -> list

Given two lists of strings (scoped names), return a copy of list A
with any prefix it shares with B removed.

  e.g. pruneScope(['A', 'B', 'C', 'D'], ['A', 'B', 'D']) -> ['C', 'D']"""

    if not our_scope:
        return target_scope

    tscope = target_scope[:]
    i = 0
    while len(tscope) > 0 and \
          i < len(our_scope) and \
          tscope[0] == our_scope[i]:
        del tscope[0]
        i = i + 1
    return tscope

_valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" \
               " 0123456789_!$%^&*()-=+[]{};#:@~,./<>?|`"

_valid_unichars = map(ord, list(_valid_chars))

def escapifyString(s):
    """escapifyString(string) -> string

Return the given string with any non-printing characters escaped."""
    
    global _valid_chars
    l = list(s)
    for i in range(len(l)):
        if l[i] not in _valid_chars:
            l[i] = "\\%03o" % ord(l[i])
    return "".join(l)


def escapifyWString(l, escchar="u"):
    """escapifyWString(int list) -> string

Take a list of integers representing Unicode characters and return an
ASCII string with all characters outside that range replaced with \\u
escapes."""

    global _valid_unichars
    m = [None] * len(l)
    for i in range(len(l)):
        assert(l[i] <= 0xffff)
        if l[i] in _valid_unichars:
            m[i] = chr(l[i])
        else:
            m[i] = "\\%s%04x" % (escchar, l[i])
    return "".join(m)


def reprFloat(f):
    """reprFloat(float) -> string

Return the string representation of an IDL float type (float, double,
long double), with enough precision to completely reconstruct the bit
pattern."""
    # *** Deal with long double

    s = "%.17g" % f
    if s.find(".") == -1 and s.find("e") == -1:
        s = s + ".0"
    return s


def relativeScope(fromScope, destScope):
    """relativeScope(fromScope, destScope) -> list

Given two globally-scoped names, return a minimal scoped name list
which identifies the destination scope, without clashing with another
identifier. For example, given IDL:

  module M {
    typedef short A;
    typedef long  B;

    module N {
      typedef string B;
      interface I {
        void op(in ::M::A x, in ::M::B y);
      };
    };
  };

relativeScope(["M", "N", "I"], ["M", "A"]) -> ["A"]
relativeScope(["M", "N", "I"], ["M", "B"]) -> ["M", "B"]

If the only valid result is a globally-scoped name, the result list is
prefixed with None:

  module O {
    typedef short C;
  };
  module P {
    module O {
      interface J {
        void op(in ::O::C z);
      };
    };
  };

relativeScope(["P", "O", "J"], ["O", "C"]) -> [None, "O", "C"]
"""
    if not fromScope:
        # At global scope: just use the destination scope
        return destScope

    import _omniidl
    rs = _omniidl.relativeScopedName(fromScope, destScope)

    if rs is None:
        # Compiler does not know about the scopes. Globally scope.
        return [None] + destScope
    else:
        return rs
