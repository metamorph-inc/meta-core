# -*- python -*-
#                           Package   : omniidl
# dump.py                   Created on: 1999/10/29
#			    Author    : Duncan Grisby (dpg1)
#
#    Copyright (C) 2005-2011 Apasphere Ltd
#    Copyright (C) 1999 AT&T Laboratories Cambridge
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
#   Back-end which just dumps the IDL tree

"""Dumps the IDL tree"""

from omniidl import idlast, idltype, idlutil, idlvisitor, output, main
import sys

usage_string = """\
  -Wbinline       Output declarations from #included files inline"""


ccolonName = idlutil.ccolonName

def relativeScope(fromScope, destScope):
    """relativeScope variant that handles invented fromScopes"""
    rs = idlutil.relativeScope(fromScope, destScope)

    if rs[0] is None:
        try:
            rd = idlast.findDecl(destScope)

        except idlast.DeclNotFound:
            return rs

        new_rs = rs
        while new_rs[0] is None and len(fromScope) > 1:
            fromScope = fromScope[:-1]
            new_rs = idlutil.relativeScope(fromScope, destScope)

        if new_rs[0] is not None:
            return new_rs

    return rs


class DummyStream:
    def __init__(self, st):
        self.st = st

    def out(self, *args, **kw):
        pass

    def inc_indent(self):
        self.st.inc_indent()

    def dec_indent(self):
        self.st.dec_indent()


class DumpVisitor (idlvisitor.AstVisitor, idlvisitor.TypeVisitor):

    def __init__(self, st, inline):
        self.dummy_st = DummyStream(st)
        self.real_st  = st
        self.st       = st
        self.scope    = []
        self.inline   = inline

    def visitAST(self, node):
        for n in node.declarations():
            if not self.inline:
                if n.mainFile():
                    self.st = self.real_st
                else:
                    self.st = self.dummy_st

            n.accept(self)

    def visitModule(self, node):
        self.st.out("")

        for comment in node.comments():
            self.st.out(comment.text().strip())

        self.st.out("""\
module @id@ {""", id = node.identifier())

        self.st.inc_indent()

        self.scope.append(node.identifier())

        for n in node.definitions():
            n.accept(self)

        self.scope.pop()

        self.st.dec_indent()

        self.st.out("""\
};""")

    def visitInterface(self, node):

        self.st.out("")

        for comment in node.comments():
            self.st.out(comment.text().strip())

        if len(node.inherits()) != 0:
            inheritl = []
            for i in node.inherits():
                inheritl.append(ccolonName(relativeScope(self.scope,
                                                         i.scopedName())))

            inherits = ": " + ", ".join(inheritl) + " "
        else:
            inherits = ""

        if   node.abstract(): qual = "abstract "
        elif node.local():    qual = "local "
        else:                 qual = ""
        
        self.st.out("""\
@qual@interface @id@ @inherits@{""",
                    id = node.identifier(), inherits=inherits, qual=qual)

        self.st.inc_indent()

        self.scope.append(node.identifier())

        for n in node.contents():
            n.accept(self)

        if hasattr(node, "_ami_ops"):
            self.st.out("""
/* AMI pseudo-operations...
""")
            for n in node._ami_ops:
                n.accept(self)
            self.st.out("""
** ...end of AMI pseudo operations.
*/
""")

        self.scope.pop()

        self.st.dec_indent()
        self.st.out("""\
};""")


    def visitForward(self, node):
        for comment in node.comments():
            self.st.out(comment.text().strip())

        if   node.abstract(): qual = "abstract "
        elif node.local():    qual = "local "
        else:                 qual = ""
        
        self.st.out("""
@qual@interface @id@;""", id = node.identifier(), qual=qual)


    def visitConst(self, node):
        node.constType().accept(self)
        type = self.__result_type

        if node.constKind() == idltype.tk_enum:
            value = ccolonName(relativeScope(self.scope,
                                             node.value().scopedName()))

        elif node.constKind() == idltype.tk_string:
            value = '"' + idlutil.escapifyString(node.value()) + '"'

        elif node.constKind() == idltype.tk_wstring:
            value = 'L"' + idlutil.escapifyWString(node.value()) + '"'

        elif node.constKind() == idltype.tk_wchar:
            value = "L'" + idlutil.escapifyWString([node.value()]) + "'"

        elif node.constKind() in [idltype.tk_float, idltype.tk_double,
                                  idltype.tk_longdouble]:
            value = idlutil.reprFloat(node.value())

        elif node.constKind() == idltype.tk_fixed:
            value = node.value() + "d"
        else:
            value = str(node.value())
        
        self.st.out("""\
const @type@ @id@ = @value@;""",
                    type=type, id=node.identifier(), value=value)


    def visitTypedef(self, node):
        if node.constrType():
            node.aliasType().decl().accept(self)

        node.aliasType().accept(self)
        type  = self.__result_type
        decll = []
        for d in node.declarators():
            d.accept(self)
            decll.append(self.__result_declarator)

        decls = ", ".join(decll)

        self.st.out("""\
typedef @type@ @decls@;""",
               type=type, decls=decls)


    def visitStruct(self, node):
        self.st.out("""
struct @id@ {""",
               id = node.identifier())

        self.scope.append(node.identifier())

        for m in node.members():
            if m.constrType():
                self.st.inc_indent()
                m.memberType().decl().accept(self)
                self.st.dec_indent()

            m.memberType().accept(self)
            type = self.__result_type
            decll = []
            for d in m.declarators():
                d.accept(self)
                decll.append(self.__result_declarator)
            decls = ", ".join(decll)

            self.st.out("""\
  @type@ @decls@;""",

                   type=type, decls=decls)

        self.scope.pop()

        self.st.out("""\
};""")

    def visitStructForward(self, node):
        self.st.out("""
struct @id@;""", id = node.identifier())


    def visitException(self, node):
        self.st.out("""
exception @id@ {""",
               id = node.identifier())

        self.scope.append(node.identifier())

        for m in node.members():
            if m.constrType():
                self.st.inc_indent()
                m.memberType().decl().accept(self)
                self.st.dec_indent()

            m.memberType().accept(self)
            type = self.__result_type
            decll = []
            for d in m.declarators():
                d.accept(self)
                decll.append(self.__result_declarator)
            decls = ", ".join(decll)

            self.st.out("""\
  @type@ @decls@;""",

                        type=type, decls=decls)

        self.scope.pop()

        self.st.out("""\
};""")


    def visitUnion(self, node):
        if node.constrType():

            self.st.out("""
union @id@ switch (""",
                        id = node.identifier())
            self.st.inc_indent()
            node.switchType().decl().accept(self)
            self.st.out(""") {""")
            self.st.dec_indent

        else:
            node.switchType().accept(self)
            stype = self.__result_type

            self.st.out("""\
union @id@ switch (@stype@) {""",

                        id=node.identifier(), stype=stype)

        self.scope.append(node.identifier())

        for c in node.cases():
            if c.constrType():
                self.st.inc_indent()
                c.caseType().decl().accept(self)
                self.st.dec_indent()

            for l in c.labels():
                if l.default():
                    self.st.out("""\
  default:""")
                else:
                    if l.labelKind() == idltype.tk_enum:
                        lv = ccolonName(relativeScope(self.scope,
                                                      l.value().scopedName()))
                    elif l.labelKind() == idltype.tk_char:
                        lv = "'" + repr(l.value())[1:-1] + "'"
                    else:
                        lv = str(l.value())
                        
                    self.st.out("""\
  case @lv@:""",
                           lv=lv)
                    
            c.caseType().accept(self)
            type = self.__result_type
            c.declarator().accept(self)
            decl = self.__result_declarator

            self.st.out("""\
    @type@ @decl@;""",
                   
                   type=type, decl=decl)

        self.scope.pop()

        self.st.out("};")


    def visitUnionForward(self, node):
        self.st.out("""
union @id@;""", id = node.identifier())

    def visitEnum(self, node):
        enuml = []
        for e in node.enumerators(): enuml.append(e.identifier())

        enums = ", ".join(enuml)

        self.st.out("""\
enum @id@ {@enums@};""",

               id=node.identifier(), enums=enums)

    def visitAttribute(self, node):
        if node.readonly():
            readonly = "readonly "
        else:
            readonly = ""

        node.attrType().accept(self)
        type = self.__result_type

        ids  = ", ".join(node.identifiers())

        self.st.out("""\
@readonly@attribute @type@ @ids@;""",

               readonly=readonly, type=type, ids=ids)


    def visitOperation(self, node):
        if node.oneway():
            oneway = "oneway "
        else:
            oneway = ""
        
        node.returnType().accept(self)
        rtype = self.__result_type

        self.scope.append(node.identifier())

        paraml = []
        for p in node.parameters():
            if   p.is_in() and p.is_out(): inout = "inout"
            elif p.is_in():                inout = "in"
            else:                          inout = "out"
            p.paramType().accept(self)
            type = self.__result_type
            paraml.append(inout + " " + type + " " + p.identifier())

        params = ", ".join(paraml)

        self.scope.pop()

        if len(node.raises()) > 0:
            raisel = []
            for r in node.raises():
                ename  = ccolonName(relativeScope(self.scope, r.scopedName()))

                raisel.append(ename)

            raises = " raises (" + ", ".join(raisel) + ")"
        else:
            raises = ""

        self.st.out("""\
@oneway@@rtype@ @id@(@params@)@raises@;""",
               
               oneway=oneway, rtype=rtype, id=node.identifier(),
               params=params, raises=raises)

    def visitNative(self, node):
        self.st.out("""
native @id@;""",
                    id=node.identifier())

    def visitValue(self, node):
        self.st.out("")

        for comment in node.comments():
            self.st.out(comment.text().strip())

        if node.inherits():
            inheritl = []
            for i in node.inherits():
                inheritl.append(ccolonName(relativeScope(self.scope,
                                                         i.scopedName())))

            if node.truncatable():
                truncatable = "truncatable "
            else:
                truncatable = ""

            inherits = ": " + truncatable + ", ".join(inheritl) + " "
        else:
            inherits = ""

        if node.supports():
            inheritl = []
            for i in node.supports():
                inheritl.append(ccolonName(relativeScope(self.scope,
                                                         i.scopedName())))

            inherits = inherits + "supports " + ", ".join(inheritl) + " "

        if node.custom():
            custom = "custom "
        else:
            custom = ""

        self.st.out("""\
@custom@valuetype @id@ @inherits@{""",
                    id = node.identifier(), inherits=inherits, custom=custom)

        self.scope.append(node.identifier())

        self.st.inc_indent()
        for n in node.contents():
            n.accept(self)

        self.scope.pop()

        self.st.dec_indent()
        self.st.out("""\
};""")

    def visitStateMember(self, node):
        access = {0: "public", 1: "private"}[node.memberAccess()]

        if node.constrType():
            node.memberType().decl().accept(self)

        node.memberType().accept(self)
        type = self.__result_type
        decll = []
        for d in node.declarators():
            d.accept(self)
            decll.append(self.__result_declarator)
        decls = ", ".join(decll)

        self.st.out("""\
@access@ @type@ @decls@;""",
                    access=access, type=type, decls=decls)

    def visitValueAbs(self, node):
        self.st.out("")

        for comment in node.comments():
            self.st.out(comment.text().strip())

        if node.inherits():
            inheritl = []
            for i in node.inherits():
                inheritl.append(ccolonName(relativeScope(self.scope,
                                                         i.scopedName())))

            inherits = ": " + ", ".join(inheritl) + " "
        else:
            inherits = ""

        if node.supports():
            inheritl = []
            for i in node.supports():
                inheritl.append(ccolonName(relativeScope(self.scope,
                                                         i.scopedName())))

            inherits = inherits + "supports " + ", ".join(inheritl) + " "

        self.st.out("""\
abstract valuetype @id@ @inherits@{""",
                    id = node.identifier(), inherits=inherits)

        self.st.inc_indent()
        for n in node.contents():
            n.accept(self)

        self.st.dec_indent()
        self.st.out("""\
};""")

    def visitValueForward(self, node):
        self.st.out("")

        for comment in node.comments():
            self.st.out(comment.text().strip())

        if node.abstract():
            abstract = "abstract "
        else:
            abstract = ""

        self.st.out("""\
@abstract@valuetype @id@;""",
                    id = node.identifier(), abstract = abstract)

        
    def visitValueBox(self, node):      return

    def visitFactory(self, node):       return


    def visitDeclarator(self, node):
        l = [node.identifier()]
        for s in node.sizes():
            l.append("[" + str(s) + "]")

        self.__result_declarator = "".join(l)


    ttsMap = {
        idltype.tk_void:       "void",
        idltype.tk_short:      "short",
        idltype.tk_long:       "long",
        idltype.tk_ushort:     "unsigned short",
        idltype.tk_ulong:      "unsigned long",
        idltype.tk_float:      "float",
        idltype.tk_double:     "double",
        idltype.tk_boolean:    "boolean",
        idltype.tk_char:       "char",
        idltype.tk_octet:      "octet",
        idltype.tk_any:        "any",
        idltype.tk_TypeCode:   "CORBA::TypeCode",
        idltype.tk_Principal:  "CORBA::Principal",
        idltype.tk_longlong:   "long long",
        idltype.tk_ulonglong:  "unsigned long long",
        idltype.tk_longdouble: "long double",
        idltype.tk_wchar:      "wchar"
        }

    def visitBaseType(self, type):
        self.__result_type = self.ttsMap[type.kind()]

    def visitStringType(self, type):
        if type.bound() == 0:
            self.__result_type = "string"
        else:
            self.__result_type = "string<" + str(type.bound()) + ">"

    def visitWStringType(self, type):
        if type.bound() == 0:
            self.__result_type = "wstring"
        else:
            self.__result_type = "wstring<" + str(type.bound()) + ">"

    def visitSequenceType(self, type):
        type.seqType().accept(self)
        if type.bound() == 0:
            self.__result_type = "sequence<" + self.__result_type + ">"
        else:
            self.__result_type = "sequence<" + self.__result_type + ", " +\
                                 str(type.bound()) + ">"

    def visitFixedType(self, type):
        if type.digits() > 0:
            self.__result_type = "fixed<" + str(type.digits()) + "," +\
                                 str(type.scale()) + ">"
        else:
            self.__result_type = "fixed"

    def visitDeclaredType(self, type):
        if type.decl().scopedName() == ["CORBA", "Object"]:
            self.__result_type = "Object"
            return

        rs = relativeScope(self.scope, type.decl().scopedName())
        self.__result_type = ccolonName(rs)


def run(tree, args):
    inline = 0

    for arg in args:
        if arg == "inline":
            inline = 1

        else:
            sys.stderr.write(main.cmdname + ": Warning: dump "
                             "back-end does not understand argument: %s\n" %
                             arg)

    st = output.Stream(sys.stdout, 2)
    dv = DumpVisitor(st, inline)
    tree.accept(dv)
