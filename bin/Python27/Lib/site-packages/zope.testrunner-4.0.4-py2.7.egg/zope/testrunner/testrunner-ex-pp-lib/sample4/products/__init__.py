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
"""Sample package that knits in extra directories.
"""

import os

__path__.append(
    os.path.join(
        os.path.dirname( # testing
            os.path.dirname( # testrunner-ex-knit-lib
                os.path.dirname( # sample4
                    os.path.dirname(__file__) # products
                    )
                )
            )
        , "testrunner-ex-pp-products"
        )
    )

