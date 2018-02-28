# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# interceptors.py            Created on: 2003/05/27
#                            Author    : Duncan Grisby (dgrisby)
#
#    Copyright (C) 2003-2013 Apasphere Ltd
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
#    Python request interceptors

"""omniORB.interceptors

Interceptor registration functions:

  addClientSendRequest
  addClientReceiveReply
  addServerReceiveRequest
  addServerSendReply
  addServerSendException

To register an interceptor function, call the relevant registration
function with a callable argument. The callable will be called with
two or three arguments. The first argument is the name of the
operation being invoked; the second is the set of service contexts to
be retrieved or filled in. ServerSendException has a third argument,
the repository id of the exception being thrown.

When receiving service contexts (ClientReceiveReply,
ServerReceiveRequest), the second argument is a tuple of 2-tuples. In
each 2-tuple, the first item is the service context id and the second
item is the CDR encapsulation of the service context. The
encapsulation can be decoded with omniORB.cdrUnmarshal() (but only if
you know the type to decode it to).

When sending service contexts (ClientSendRequest, ServerSendReply,
ServerSendException), the second argument is an empty list. The
interceptor function can choose to add one or more service context
tuples, with the same form described above, by appending to the list.
Encapsulations are created with omniORB.cdrMarshal().

Interceptor registration functions may only be called before the ORB
is initialised. Attempting to call them later results in a
BAD_INV_ORDER exception.
"""

from _omnipy.interceptor_func import *
