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

from .DTR_Transformer import DTR_Transformer

class DTR_DetectionsDataTransformer(DTR_Transformer):
    def __init__(self, configFilePath=''):
        super().__init__(configFilePath)

    def getName(self):
        return 'DTR_DetectionsData'

    def getDataType(self):
        return 'detection'

    def getIndexForData(self):
        return 'timerealtt'

    #def requiredKeys(self):
        #return ['instrument_id', 'observation_id', 'analysis_session_type_id', 'sqrtts', 'x', 'y']

    def getOutputChannel(self, eventData):
        return self.config.get('Dtr','outputchannel')+'.'+eventData['instrument_id']+'.'+eventData['observation_id']+'.'+eventData['analysis_session_type_id']

    def getStoreLocationKey(self, eventData):
        return 'gui_data.'+self.config.get('Dtr','guiname')+'.'+eventData['instrument_id']+'.'+eventData['observation_id']+'.'+eventData['analysis_session_type_id']+'.detection'



    def transformData(self, eventData):

        eventData["timerealtt"] = (float(self.config.get('General','mjdref')) - 53005.0) * 86400.0 + float(eventData['t_start_mjd'])

        if float(eventData['sqrtts']) <= 2.9:
            eventData['isUpperLimit'] = True
            eventData['class'] = 0
            eventData['error_y'] = 0
        elif float(eventData['sqrtts']) > 2.9 and float(eventData['sqrtts']) <= 4:
            eventData['isUpperLimit'] = False
            eventData['class'] = 1
        elif float(eventData['sqrtts']) > 4 and float(data['sqrtts']) <= 5:
            eventData['isUpperLimit'] = False
            eventData['class'] = 2
        else:
            eventData['isUpperLimit'] = False
            eventData['class'] = 3

        return eventData



    """ Data from mock data
        instrument_id,
        observation_id,
        analysis_session_type_id
        "x":58130.000347222,
        "y":"0.0000001",
        "flux_err":"3137.94569515991",
        "time_err":0.00034722222335404,
        "text":"58130 , 58130.0006944 , 0 +/- 3137.9 *10^-16 , sqrt(TS)= 0",
        "sqrtts":"0",
        "ra":83.633,
        "dec":22.015,
        "t_start_mjd":58130,
        "t_stop_mjd":58130.0006944,
        "t_start_tt":"442800000",
        "t_stop_tt":"442800060",
        "label":"Crab-1",
        "detectionid":"40902"
         self.config.get('General', 'mjdref', 'float'),
    """
