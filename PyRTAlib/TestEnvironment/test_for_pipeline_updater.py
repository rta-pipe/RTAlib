# ==========================================================================
#
# Copyright (C) 2018 INAF - OAS Bologna
# Author: Leonardo Baroncelli <leonardo.baroncelli26@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ==========================================================================

import os

from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB_old

os.environ['RTACONFIGFILE'] = './'

# Connect and insert random event
RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('mysql')

RTA_DL3ASTRI.insertEvent( *RTA_DL3ASTRI.getRandomEvent() )

RTA_DL3ASTRI.waitAndClose()
