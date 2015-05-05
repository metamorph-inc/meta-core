##############################################################################
#
# Copyright (c) 2005 Zope Foundation and Contributors.
# All Rights Reserved.
#
# This software is subject to the provisions of the Zope Public License,
# Version 2.1 (ZPL).  A copy of the ZPL should accompany this distribution.
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY AND ALL EXPRESS OR IMPLIED
# WARRANTIES ARE DISCLAIMED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF TITLE, MERCHANTABILITY, AGAINST INFRINGEMENT, AND FITNESS
# FOR A PARTICULAR PURPOSE.
#
##############################################################################
"""Stack-based test doctest setUp and tearDown

See setupstack.txt
"""

import os, stat, tempfile

key = '__' + __name__

def globs(test):
    try:
        return test.globs
    except AttributeError:
        return test.__dict__

def register(test, function, *args, **kw):
    tglobs = globs(test)
    stack = tglobs.get(key)
    if stack is None:
        stack = tglobs[key] = []
    stack.append((function, args, kw))

def tearDown(test):
    tglobs = globs(test)
    stack = tglobs.get(key)
    while stack:
        f, p, k = stack.pop()
        f(*p, **k)

def setUpDirectory(test):
    tmp = tempfile.mkdtemp()
    register(test, rmtree, tmp)
    here = os.getcwd()
    register(test, os.chdir, here)
    os.chdir(tmp)

def rmtree(path):
    for path, dirs, files in os.walk(path, False):
        for fname in files:
            fname = os.path.join(path, fname)
            if not os.path.islink(fname):
                os.chmod(fname, stat.S_IWUSR)
            os.remove(fname)
        for dname in dirs:
            dname = os.path.join(path, dname)
            os.rmdir(dname)
    os.rmdir(path)

def context_manager(test, manager):
    result = manager.__enter__()
    register(test, manager.__exit__, None, None, None)
    return result

