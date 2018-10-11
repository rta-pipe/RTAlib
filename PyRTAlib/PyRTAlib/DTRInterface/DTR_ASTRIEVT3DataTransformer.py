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

################################################################################
#################### TODO !!! IMPLEMENT THE MODEL !!! TODO #####################
################################################################################

from .DTR_Transformer import DTR_Transformer

class DTR_ASTRIEVT3DataTransformer(DTR_Transformer):
    def __init__(self, configFilePath=''):
        super().__init__(configFilePath)


    def getName(self):
        return ''

    def getDataType(self):
        return ''

    def getIndexForData(self):
        return ''

    def requiredKeys(self):
        return ''

    def getOutputChannel(self, eventData):
        return ''

    def transformData(self, eventData):
        return {'energy': eventData['energy'], 'time': eventData['timerealtt'], 'isUpperLimit': True if eventData['energy']<= 2.9 else False }

    def getStoreLocationKey(self, eventData):
        return ''


    """ Data from RTA_DL3ASTRI_DB
        eventidfits,
        time,
        ra_deg,
        dec_deg,
        energy,
        detx,
        dety,
        alt,
        az,
        gammaness,
        self.config.get('General', 'mjdref', 'float'),
        observationid,
        datarepositoryid,
        status
    """
