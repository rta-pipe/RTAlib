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
import sys
import os
from os.path import dirname, abspath, join
from time import sleep
from random import randint, uniform
import redis
from ast import literal_eval
# for Python 3.x
from io import StringIO
import csv

rootFolder =  os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
sys.path.append(rootFolder+'/PyRTAlib/')
from PyRTAlib.Utils         import Config


def sendMockData():
    config = Config(configFilePath)

    redisConn = redis.Redis(
                                host=config.get('Redis','host'),
                                port=6379,
                                db=config.get('Redis','dbname'),
                                password=config.get('Redis','password')
                            )
    # pubsub = redisConn.pubsub()

    dtrInputChannel = config.get('Dtr','inputchannel')

    mockDetectionData = getMockData()

    print("Example of data: ", mockDetectionData[0])

    # SORTING...


    for data in reversed(mockDetectionData):
        if not "dataType" in mockDetectionData:
            data["dataType"] = 'detection'

        if not "instrument_id" in mockDetectionData:
            data["instrument_id"] = instrument_id

        if not "observation_id" in mockDetectionData:
            data["observation_id"] = observation_id

        if not "analysis_session_type_id" in mockDetectionData:
            data["analysis_session_type_id"] = analysis_session_type_id

        sleep(randint(int(sleepMin),int(sleepMax)))

        print("Publish ",data," on DTR channel ",dtrInputChannel)

        redisConn.publish(dtrInputChannel, data)

    redisConn.publish(dtrInputChannel, 'STOP')



def getMockData():
    print("Parsing data...")
    if fileFormat == 'json':
        mockdatafile = open(mockDataPath,'r')
        mockData = mockdatafile.read()
        return literal_eval(mockData);

    elif fileFormat == 'csv':
        data = []
        with open(mockDataPath, newline='') as csvfile:
            reader = list(csv.reader(csvfile, delimiter=','))
            keys = reader[0]
            print(keys)
            print("Data length: {}".format(len(reader)))
            for row in reader[2:]:
                dict = {}
                l = len(row)
                for i in range(l):
                    dict[keys[i]] = row[i]
                data.append(dict)

        print("Data length: {}".format(len(data)))
        return data

    else:
        print("Format not found.")

if __name__ == '__main__':

    if len(sys.argv) < 4:
        print("Please enter: \n - the path to the configuration file \n - the instrument_id \n - the observation_id \n - the analysis_session_type_id \n - sleep time (lower bound) \n - sleep time (upper bound) \n - mock file path\n - mock file format")
        exit()


    configFilePath = sys.argv[1]
    instrument_id = sys.argv[2]
    observation_id = sys.argv[3]
    analysis_session_type_id = sys.argv[4]
    sleepMin = sys.argv[5]
    sleepMax = sys.argv[6]
    mockDataPath = sys.argv[7]
    fileFormat = sys.argv[8]

    sendMockData()


    """
        Examples:
            CSV ASTRI:
                {'import_time': '1539852367.9418802',
                'rootname': '/data01/ANALYSIS3.local/ASTRI/DREVT3_ASTRI_O1/ctools-lc_LC-from-start/T442800000_442800030/T442800000_442800030_E0.631_350_P184.557449_-5.78436',
                'fluxul': 'NULL',
                'detectionid': '42631',
                'label': 'Crab-1',
                'l': '184.557',
                'b': '-5.7843',
                'ella': '-1',
                'ellb': '-1',
                'fluxerr': '0.0275682318967894',
                'flux': '0.0000001',
                'sqrtts': '0',
                'spectralindex': '0',
                'spectralindexerr': '39935.6',
                'tstart': '442800000', ----> IS ALWAYS THE SAME
                'tstop': '442800030',
                'emin': '0.631',
                'emax': '350',
                'run_l': '184.557449',
                'run_b': '-5.78436',
                'dataType': 'detection',
                'instrument_id': '1',
                'observation_id': '2',
                'analysis_session_type_id': '3'}

            CSV CTA:
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
