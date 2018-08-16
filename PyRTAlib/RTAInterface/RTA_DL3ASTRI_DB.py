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

from .RTA_DL_DB import RTA_DL_DB


class RTA_DL3ASTRI_DB(RTA_DL_DB):

    def __init__(self, database, configFilePath = ''):
        super().__init__(database, configFilePath = '')

    def insertEvent(self, *args):
        return self.dbConnector.insertData(self.configs['evt3modelname'], *args)
