# Copyright 2009 Canonical Ltd.  All rights reserved.
#
# This file is part of lazr.testing
#
# lazr.testing is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, version 3 of the License.
#
# lazr.testing is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with lazr.testing.  If not, see <http://www.gnu.org/licenses/>.

"The lazr.testing package."

import pkg_resources
__version__ = pkg_resources.resource_string(
    "lazr.testing", "version.txt").strip()
