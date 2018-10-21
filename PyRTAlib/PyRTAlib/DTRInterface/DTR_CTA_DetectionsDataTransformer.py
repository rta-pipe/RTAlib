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
from .DTR_Utils import *

class DTR_CTA_DetectionsDataTransformer(DTR_Transformer):
    def __init__(self, configFilePath=''):
        super().__init__(configFilePath)

    def getName(self):
        return 'DTR_CTA_DetectionsDataTransformer'

    def getDataType(self):
        return 'detection'

    def getIndexForData(self):
        return 'tstart'

    #def requiredKeys(self):
        #return ['instrument_id', 'observation_id', 'analysis_session_type_id', 'sqrtts', 'x', 'y']

    def getOutputChannel(self, eventData):
        return self.config.get('Dtr','outputchannel')+'.'+eventData['instrument_id']+'.'+eventData['observation_id']+'.'+eventData['analysis_session_type_id']

    def getStoreLocationKey(self, eventData):
        return 'gui_data.'+self.config.get('Dtr','guiname')+'.'+eventData['instrument_id']+'.'+eventData['observation_id']+'.'+eventData['analysis_session_type_id']+'.detection'



    def transformData(self, eventData):

        # Converting TTs to MJDs
        eventData["tstart_mjd"] = str((float(eventData["tstart"]) / 86400.0) + 53005.0)
        eventData["tstop_mjd"]  = str((float(eventData["tstop"]) / 86400.0) + 53005.0)
        
        # Compute x in TT
        x_tt = float(eventData["tstart"]) + (float(eventData["tstop"])-float(eventData["tstart"]))/2

        # Converting x in MJD
        x = (x_tt / 86400.0) + 53005.0
        eventData["x"] = str(x)


        eventData["error_x"] = '0'

        eventData["text"] = eventData["dataType"]+'_'+eventData["detectionid"]+'_'+eventData["l"]+'_'+eventData["b"]

        if float(eventData["sqrtts"]) <= 4:
            eventData["isUpperLimit"] = 'True'
            eventData["class"] = '0'
            eventData["error_y"] = '0'

        else:
            eventData["isUpperLimit"] = 'False'
            eventData["class"] = '0'
            eventData["error_y"] = eventData["fluxerr"]


        if "l" in eventData and "b" in eventData:
            radec=convert_gal_to_fk5(eventData["l"], eventData["b"])
            eventData["ra"] = radec[0]
            eventData["dec"]= radec[1]


        if "ra" in eventData and "dec" in eventData:
            lb=convert_fk5_to_gal(eventData["ra"], eventData["dec"])
            eventData["l"] = lb[0]
            eventData["b"] = lb[1]





        return eventData



    """ Data from mock data
        {'import_time': '1539884499.1339893',
        'rootname': '/data01/ANALYSIS3.local/CTA-SOUTH/DREVT3_CTA_O1/ctools-lc_LC1000-shift10/T442803540_442804540/T442803540_442804540_E0.1_100_P184.557449_-5.78436',
        'fluxul': 'NULL',
        'detectionid': '40636',
        'label': 'Crab-1',
        'l': '184.557',
        'b': '-5.7843',
        'ella': '-1',
        'ellb': '-1',
        'fluxerr': '0.317889041855604',
        'flux': '13.6674',
        'sqrtts': '110.566',
        'spectralindex': '2.43214',
        'spectralindexerr': '0.0203501',
        'tstart': '442803540',
        'tstop': '442804540',
        'emin': '0.1',
        'emax': '100',
        'run_l': '184.557449',
        'run_b': '-5.78436',
        'dataType': 'detection',
        'instrument_id': '1',
        'observation_id': '2',
        'analysis_session_type_id': '3'}
    """
