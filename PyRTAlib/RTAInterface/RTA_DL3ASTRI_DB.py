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
from ..DataModels import EVT3_ASTRI


class RTA_DL3ASTRI_DB(RTA_DL_DB):

    def __init__(self, database, configFilePath = ''):
        super().__init__(database, configFilePath = '')


    def insertEvent(self, eventidfits, time, ra_deg, dec_deg, energy, detx, dety, mcid, observationid=0, datarepositoryid=0, status = 1):
        """
        Add mjdref

        """
        evt3 = EVT3_ASTRI(eventidfits, time, ra_deg, dec_deg, energy, detx, dety, mcid, self.configs['mjdref'], observationid, datarepositoryid, status)
        return self.dbConnector.insertData(evt3.getInsertQuery(self.configs['evt3modelname']))

    def fakeInsert(self, eventidfits, time, ra_deg, dec_deg, energy, detx, dety, mcid, observationid=0, datarepositoryid=0, status = 1):
         evt3 = EVT3_ASTRI(eventidfits, time, ra_deg, dec_deg, energy, detx, dety, mcid, self.configs['mjdref'], observationid, datarepositoryid, status)
         return True


    """
    def insertEvent(self, *args):
        return self.dbConnector.insertData(self.configs['evt3modelname'], *args)

    def insertEvent_3(self, **kwargs):
        return self.dbConnector.insertData_3(self.configs['evt3modelname'], **kwargs)
    """
