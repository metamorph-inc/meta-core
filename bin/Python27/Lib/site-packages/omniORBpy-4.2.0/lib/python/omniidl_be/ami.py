# -*- python -*-
#                           Package   : omniidl
# ami.py                    Created on: 2011/01/18
#			    Author    : Duncan Grisby
#
#    Copyright (C) 2011-2012 Apasphere Ltd
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
#   Functions to build AMI implied-IDL structures. When used as an
#   omniidl back-end, outputs the generated IDL.

"""AMI support"""

from omniidl import idlast, idltype, idlutil, idlvisitor, output
from omniidl_be.dump import DumpVisitor
import sys

usage_string = """\
  -Wbinline       Output declarations from #included files inline
  -Wbdump         Dump original IDL as well as AMI IDL"""


#
# Static declarations of types defined in messaging.idl

reply_handler_decl = idlast.Interface("messaging.idl", 0, 0, [], [],
                                      "ReplyHandler",
                                      ["Messaging", "ReplyHandler"],
                                      "IDL:omg.org/Messaging/ReplyHandler:1.0",
                                      0, 0, [])
reply_handler_decl._setContents([])

poller_decl = idlast.ValueAbs("messaging.idl", 0, 0, [], [],
                              "Poller",
                              ["Messaging", "Poller"],
                              "IDL:omg.org/Messaging/Poller/1.0",
                              [], [])
poller_decl._setContents([])

exception_holder_decl = idlast.Value("messaging.idl", 0, 0, [], [],
                                     "ExceptionHolder",
                                     ["Messaging", "ExceptionHolder"],
                                     "IDL:omg.org/Messaging/ExceptionHolder:1.0",
                                     0, [], 0, [])
exception_holder_decl._setContents([])

exception_holder_type = idltype.Declared(exception_holder_decl,
                                         exception_holder_decl.scopedName(),
                                         idltype.tk_value, 0)

messaging_module_decl = idlast.Module("messaging.idl", 0, 0, [], [],
                                      "Messaging",
                                      ["Messaging"],
                                      "IDL:omg.org/Messaging:1.0",
                                      [reply_handler_decl,
                                       poller_decl,
                                       exception_holder_decl])

#
# Main visitor class

class AMIVisitor(idlvisitor.AstVisitor, idlvisitor.TypeVisitor):

    def __init__(self, inject=1):
        self.current = []
        self.added   = []
        self.inject  = inject

        idlast.registerDecl(messaging_module_decl.scopedName(),
                            messaging_module_decl)

        idlast.registerDecl(reply_handler_decl.scopedName(),
                            reply_handler_decl)

        idlast.registerDecl(poller_decl.scopedName(),
                            poller_decl)

        idlast.registerDecl(exception_holder_decl.scopedName(),
                            exception_holder_decl)


    def result(self):
        return idlast.AST("<generated>", self.current, [], [])
        

    def visitAST(self, node):
        new_contents = [messaging_module_decl]

        for n in node.declarations():
            self.added = [n]
            n.accept(self)
            new_contents.extend(self.added)

        if self.inject:
            node.declarations()[:] = new_contents


    def visitModule(self, node):
        saved_current = self.current
        saved_added   = self.added
        self.current = []

        new_contents = []

        for n in node.definitions():
            self.added = [n]
            n.accept(self)
            new_contents.extend(self.added)

        if self.inject:
            node.definitions()[:] = new_contents

        new_mod = idlast.Module(node.file(), node.line(), node.mainFile(),
                                [], [], node.identifier(),
                                node.scopedName(), node.repoId(),
                                self.current)

        self.current = saved_current
        self.current.append(new_mod)
        self.added = saved_added



    def _callables(self, node):
        """
        _callables(node) -> operations, clashes

        Return the operations for an interface, converting attributes
        into get/set operations.

        Also return a set of defined names used to avoid name clashes.
        """
        operations = []
        clashes    = set()

        for cb in node.callables():

            if isinstance(cb, idlast.Operation):

                clashes.add(cb.identifier())

                if cb.oneway():
                    continue

                operations.append(cb)


            elif isinstance(cb, idlast.Attribute):

                for dec in cb.declarators():

                    clashes.add(dec.identifier())

                    op_ident  = "get_" + dec.identifier()
                    op_scope  = node.scopedName() + [op_ident]
                    op_repoId = "IDL:%s:1.0" % idlutil.slashName(op_scope)

                    op = idlast.Operation(dec.file(), dec.line(),
                                          dec.mainFile(), [], [], 0,
                                          cb.attrType(),
                                          op_ident, op_scope, op_repoId,
                                          [], [], [])

                    op._ami_declarator = dec
                    operations.append(op)

                    if cb.readonly():
                        continue

                    op_ident  = "set_" + dec.identifier()
                    op_scope  = node.scopedName() + [op_ident]
                    op_repoId = "IDL:%s:1.0" % idlutil.slashName(op_scope)

                    params = [idlast.Parameter(dec.file(), dec.line(),
                                               dec.mainFile(), [], [],
                                               0, cb.attrType(),
                                               "attr_" + dec.identifier())]

                    op = idlast.Operation(dec.file(), dec.line(),
                                          dec.mainFile(), [], [], 0,
                                          idltype.Base(idltype.tk_void),
                                          op_ident, op_scope, op_repoId,
                                          params, [], [])

                    op._ami_declarator = dec
                    operations.append(op)
                    
        return operations, clashes


    def _amiName(self, scopedName, kind):
        ident  = scopedName[-1]
        scope  = scopedName[:-1]
        prefix = ""

        while 1:
            candidate = "AMI_%s%s%s" % (prefix, ident, kind)
            try:
                idlast.findDecl(scope + [candidate])

            except idlast.DeclNotFound:
                return candidate

            prefix = prefix + "AMI_"


    def _prefixName(self, prefix, oident, clashes):
        ami = ""
        while 1:
            ident = "%s%s%s" % (prefix, ami, oident)
            if ident not in clashes:
                return ident
            ami = ami + "ami_"


    def _suffixName(self, suffix, oident, clashes):
        ami = ""
        while 1:
            ident = "%s%s%s" % (oident, ami, suffix)
            if ident not in clashes:
                clashes.add(ident)
                return ident
            ami = ami + "_ami"


    def _handler(self, node):
        """
        _handler(node) -> (handler_fwd, handler)
        
        Build ReplyHandler interface.
        """

        def amiHandler(n):
            while isinstance(n, idlast.Declarator):
                n = n.fullDecl()
            return n._ami_handler

        if node.inherits():
            base_handlers = [ amiHandler(n) for n in node.inherits() ]
        else:
            base_handlers = [ reply_handler_decl ]

        handler_ident  = self._amiName(node.scopedName(), "Handler")
        handler_scope  = node.scopedName()[:-1] + [handler_ident]
        handler_repoId = "IDL:%s:1.0" % idlutil.slashName(handler_scope)

        comment = idlast.Comment("// ReplyHandler for interface %s" %
                                 node.identifier(),
                                 node.file(), node.line())

        handler_fwd = idlast.Forward(node.file(), node.line(),
                                     node.mainFile(), [], [],
                                     handler_ident, handler_scope,
                                     handler_repoId, 0, 0)

        handler = idlast.Interface(node.file(), node.line(),
                                   node.mainFile(), [], [comment],
                                   handler_ident, handler_scope,
                                   handler_repoId, 0, 0, base_handlers)
        handler._ami_gen = 1

        operations = []

        # Success methods

        callables, clashes = self._callables(node)

        for cb in callables:

            op_scope  = handler_scope + [cb.identifier()]
            op_repoId = "IDL:%s:1.0" % idlutil.slashName(op_scope)

            params = []

            if cb.returnType().kind() != idltype.tk_void:
                params.append(idlast.Parameter(cb.file(), cb.line(),
                                               cb.mainFile(), [], [],
                                               0, cb.returnType(),
                                               "ami_return_val"))
            for param in cb.parameters():
                if param.is_out():
                    params.append(idlast.Parameter(param.file(),
                                                   param.line(),
                                                   param.mainFile(),
                                                   [], [],
                                                   0, param.paramType(),
                                                   param.identifier()))

            op = idlast.Operation(cb.file(), cb.line(),
                                  cb.mainFile(), [], [],
                                  0, idltype.Base(idltype.tk_void),
                                  cb.identifier(), op_scope, op_repoId,
                                  params, [], [])

            if hasattr(cb, "_ami_declarator"):
                if cb.identifier().startswith("set"):
                    cb._ami_declarator._ami_set_handler = op
                else:
                    cb._ami_declarator._ami_get_handler = op
            else:
                cb._ami_handler = op

            operations.append(op)


        # _excep methods
        exceps = []

        for cb in callables:

            ident = cb.identifier()

            excep_ident = self._suffixName("_excep", ident, clashes)

            params = [idlast.Parameter("<builtin>", 0, 0, [], [],
                                       0, exception_holder_type,
                                       "excep_holder")]

            op_scope  = handler_scope + [excep_ident]
            op_repoId = "IDL:%s:1.0" % idlutil.slashName(op_scope)

            op = idlast.Operation(cb.file(), cb.line(),
                                  cb.mainFile(), [], [],
                                  0, idltype.Base(idltype.tk_void),
                                  excep_ident, op_scope, op_repoId,
                                  params, [], [])

            

            if hasattr(cb, "_ami_declarator"):
                if cb.identifier().startswith("set"):
                    cb._ami_declarator._ami_set_handler_excep = op
                else:
                    cb._ami_declarator._ami_get_handler_excep = op
            else:
                cb._ami_handler_excep = op

            exceps.append(op)
            

        handler._setContents(operations + exceps)
        idlast.registerDecl(handler.scopedName(), handler)
        
        node._ami_handler = handler

        return handler_fwd, handler


    def _poller(self, node):
        """
        _poller(node) -> (poller_fwd, poller)
        
        Build Poller valuetype.
        """
        def amiPoller(n):
            while isinstance(n, idlast.Declarator):
                n = n.fullDecl()
            return n._ami_poller

        if node.inherits():
            base_pollers = [ amiPoller(n) for n in node.inherits() ]
        else:
            base_pollers = [ poller_decl ]

        poller_ident  = self._amiName(node.scopedName(), "Poller")
        poller_scope  = node.scopedName()[:-1] + [poller_ident]
        poller_repoId = "IDL:%s:1.0" % idlutil.slashName(poller_scope)

        comment = idlast.Comment("// Poller valuetype for interface %s" %
                                 node.identifier(),
                                 node.file(), node.line())

        poller_fwd = idlast.ValueForward(node.file(), node.line(),
                                         node.mainFile(), [], [],
                                         poller_ident, poller_scope,
                                         poller_repoId, 1)

        poller = idlast.ValueAbs(node.file(), node.line(),
                                 node.mainFile(), [], [comment],
                                 poller_ident, poller_scope, poller_repoId,
                                 base_pollers, [])
        poller._ami_gen = 1

        operations = []

        callables, clashes = self._callables(node)

        for cb in callables:

            if cb.oneway():
                continue

            op_scope  = poller_scope + [cb.identifier()]
            op_repoId = "IDL:%s:1.0" % idlutil.slashName(op_scope)

            params = [idlast.Parameter(cb.file(), cb.line(),
                                       cb.mainFile(), [], [],
                                       0,
                                       idltype.Base(idltype.tk_ulong),
                                       "ami_timeout")]

            if cb.returnType().kind() != idltype.tk_void:
                params.append(idlast.Parameter(cb.file(), cb.line(),
                                               cb.mainFile(), [], [],
                                               1, cb.returnType(),
                                               "ami_return_val"))
            for param in cb.parameters():
                if param.is_out():
                    params.append(idlast.Parameter(param.file(),
                                                   param.line(),
                                                   param.mainFile(),
                                                   [], [],
                                                   1, param.paramType(),
                                                   param.identifier()))

            op = idlast.Operation(cb.file(), cb.line(),
                                  cb.mainFile(), [], [],
                                  0, idltype.Base(idltype.tk_void),
                                  cb.identifier(), op_scope, op_repoId,
                                  params, cb.raises(), [])

            if hasattr(cb, "_ami_declarator"):
                if cb.identifier().startswith("set"):
                    cb._ami_declarator._ami_set_poller = op
                else:
                    cb._ami_declarator._ami_get_poller = op

                op._ami_from = cb._ami_declarator
            else:
                cb._ami_poller = op
                op._ami_from   = cb

            operations.append(op)


        poller._setContents(operations)
        idlast.registerDecl(poller.scopedName(), poller)

        node._ami_poller = poller
        poller._ami_from = node

        return poller_fwd, poller


    def _implied(self, node, handler, poller):
        """
        _implied(node, handler, poller) -> interface

        Generate implied client-side sendc / sendp operations. Returns
        a new idlast.Interface containing just those methods, and sets
        _ami_ops attribute on the original Interface node.
        """
        operations = []

        handler_type = idltype.Declared(handler, handler.scopedName(),
                                        idltype.tk_objref, 0)

        poller_type  = idltype.Declared(poller, poller.scopedName(),
                                        idltype.tk_value, 0)

        intf_scope = node.scopedName()

        callables, clashes = self._callables(node)

        for cb in callables:

            if cb.oneway():
                continue

            params = []
            for param in cb.parameters():
                if param.is_in():
                    params.append(idlast.Parameter(param.file(),
                                                   param.line(),
                                                   param.mainFile(),
                                                   [], [],
                                                   0, param.paramType(),
                                                   param.identifier()))

            # sendc...

            op_ident  = self._prefixName("sendc_", cb.identifier(), clashes)
            op_scope  = intf_scope + [op_ident]
            op_repoId = "IDL:%s:1.0" % idlutil.slashName(op_scope)

            handler_param = idlast.Parameter(cb.file(), cb.line(),
                                             cb.mainFile(), [], [],
                                             0,
                                             handler_type,
                                             "ami_handler")

            op = idlast.Operation(cb.file(), cb.line(),
                                  cb.mainFile(), [], [],
                                  0, idltype.Base(idltype.tk_void),
                                  op_ident, op_scope, op_repoId,
                                  [handler_param] + params,
                                  [], cb.contexts())
            operations.append(op)

            if hasattr(cb, "_ami_declarator"):
                op._ami_from = cb._ami_declarator

                if cb.identifier().startswith("set"):
                    cb._ami_declarator._ami_set_sendc = op
                    op._ami_setter = 1
                else:
                    cb._ami_declarator._ami_get_sendc = op
                    op._ami_setter = 0
            else:
                cb._ami_sendc = op
                op._ami_from  = cb


            # sendp...

            op_ident  = self._prefixName("sendp_", cb.identifier(), clashes)
            op_scope  = intf_scope + [op_ident]
            op_repoId = "IDL:%s:1.0" % idlutil.slashName(op_scope)

            poller_param  = idlast.Parameter(cb.file(), cb.line(),
                                             cb.mainFile(), [], [],
                                             0,
                                             poller_type,
                                             "ami_poller")

            op = idlast.Operation(cb.file(), cb.line(),
                                  cb.mainFile(), [], [],
                                  0, poller_type,
                                  op_ident, op_scope, op_repoId,
                                  params[:], [], cb.contexts())

            operations.append(op)

            if hasattr(cb, "_ami_declarator"):
                op._ami_from = cb._ami_declarator

                if cb.identifier().startswith("set"):
                    cb._ami_declarator._ami_set_sendp = op
                    op._ami_setter = 1
                else:
                    cb._ami_declarator._ami_get_sendp = op
                    op._ami_setter = 0
            else:
                cb._ami_sendp = op
                op._ami_from  = cb

                
        node._ami_ops = operations

        comment = idlast.Comment("// AMI implied operations for interface %s" %
                                 node.identifier(),
                                 node.file(), node.line())

        implied_intf  = idlast.Interface(node.file(), node.line(),
                                         node.mainFile(), node.pragmas(),
                                         [comment], node.identifier(),
                                         node.scopedName(), node.repoId(),
                                         node.abstract(), node.local(),
                                         node.inherits())
        implied_intf._setContents(operations)

        return implied_intf


    def visitInterface(self, node):
        if node.local():
            return

        handler_fwd, handler = self._handler(node)
        poller_fwd,  poller  = self._poller(node)
        implied_ops_intf     = self._implied(node, handler_fwd, poller_fwd)

        self.current.append(handler)
        self.current.append(poller)
        self.current.append(implied_ops_intf)
        self.added = [handler_fwd, poller_fwd, node, handler, poller]



def run(tree, args):
    inline = 0
    dump   = 0

    for arg in args:
        if arg == "inline":
            inline = 1

        elif arg == "dump":
            dump = 1

        else:
            sys.stderr.write(main.cmdname + ": Warning: ami "
                             "back-end does not understand argument: %s\n" %
                             arg)

    av = AMIVisitor(dump)
    tree.accept(av)

    st = output.Stream(sys.stdout, 2)
    dv = DumpVisitor(st, inline)

    if dump:
        tree.accept(dv)

    else:
        new_tree = av.result()
        new_tree.accept(dv)
