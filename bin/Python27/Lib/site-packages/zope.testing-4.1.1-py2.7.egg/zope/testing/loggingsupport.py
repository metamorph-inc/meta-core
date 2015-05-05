##############################################################################
#
# Copyright (c) 2004 Zope Foundation and Contributors.
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
import logging

class Handler(logging.Handler):

    def __init__(self, *names, **kw):
        logging.Handler.__init__(self)
        self.names = names
        self.records = []
        self.setLoggerLevel(**kw)

    def setLoggerLevel(self, level=1):
        self.level = level
        self.oldlevels = {}

    def emit(self, record):
        self.records.append(record)

    def clear(self):
        del self.records[:]

    def install(self):
        for name in self.names:
            logger = logging.getLogger(name)
            self.oldlevels[name] = logger.level
            logger.setLevel(self.level)
            logger.addHandler(self)

    def uninstall(self):
        for name in self.names:
            logger = logging.getLogger(name)
            logger.setLevel(self.oldlevels[name])
            logger.removeHandler(self)

    def __str__(self):
        return '\n'.join(
            [("%s %s\n  %s" %
              (record.name, record.levelname,
               '\n'.join([line
                          for line in record.getMessage().split('\n')
                          if line.strip()])
               )
              )
              for record in self.records]
              )


class InstalledHandler(Handler):

    def __init__(self, *names, **kw):
        Handler.__init__(self, *names, **kw)
        self.install()
