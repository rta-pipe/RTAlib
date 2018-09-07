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

    def __init__(self, database, configFilePath = '', pure_multithreading = False):
        super().__init__(database, configFilePath, pure_multithreading)

        # Pipeline Database Updater
        if not self.pure_multithreading and self.config.get('MySqlPipelineDatabase', 'active', 'bool'):
            self.mysqlDbConnector = self.getMySqlConnector(configFilePath, 'MySqlPipelineDatabase')
            self.mysqlDbConnector.connect()
            print('[RTA_DL3ASTRI_DB] Pipeline updater activated.')


    def insertEvent(self, eventidfits, time, ra_deg, dec_deg, energy, detx, dety, alt, az, gammaness, observationid = 1, datarepositoryid = 1, status = 1):
        evt3 = EVT3_ASTRI(eventidfits, time, ra_deg, dec_deg, energy, detx, dety, alt, az, gammaness, self.config.get('General', 'mjdref', 'float'), observationid, datarepositoryid, status)
        committed = super()._insertEvent(evt3)
        if committed:
            self.updatePipeline(evt3.timerealtt, evt3.observationid, evt3.datarepositoryid)

    def getRandomEvent(self):
        return EVT3_ASTRI.getRandomEvent()

    def updatePipeline(self, timerealtt, observationid, datarepositoryid):
        query = 'update observation_to_datarepository set tenddata='+str(timerealtt)+' where observationid='+str(observationid)+' and datarepositoryid='+str(datarepositoryid)
        if self.config.get('MySqlPipelineDatabase', 'debug', 'bool'):
            print('[RTA_DL3ASTRI_DB] Updating pipeline..query={}'.format(query))
        self.mysqlDbConnector.executeQuery(query)



    """
    def fakeInsert(self, eventidfits, time, ra_deg, dec_deg, energy, detx, dety, mcid, observationid=0, datarepositoryid=0, status = 1):
        evt3 = EVT3_ASTRI(eventidfits, time, ra_deg, dec_deg, energy, detx, dety, mcid, self.configs['mjdref'], observationid, datarepositoryid, status)
        return self.dbConnector.fakeInsertData(self.configs['evt3modelname'], evt3.getData())
    """
