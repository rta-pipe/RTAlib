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

rootFolder = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
sys.path.append(rootFolder+'/PyRTAlib/')
from PyRTAlib.Utils         import Config


def sendMockData(instrument_id, observation_id, analysis_session_type_id):
    config = Config('./')

    redisConn = redis.Redis(
                                host=config.get('Redis','host'),
                                port=6379,
                                db=config.get('Redis','dbname'),
                                password=config.get('Redis','password')
                            )
    # pubsub = redisConn.pubsub()

    dtrInputChannel = config.get('Dtr','inputchannel')

    mockDetectionData = getMockData()

    for data in mockDetectionData:
        data["dataType"] = 'detection'
        data["instrument_id"] = instrument_id
        data["observation_id"] = observation_id
        data["analysis_session_type_id"] = analysis_session_type_id
        sleep(randint(3,7))
        print("Publish ",data," on DTR channel ",dtrInputChannel)
        redisConn.publish(dtrInputChannel, data)

    redisConn.publish(dtrInputChannel, 'STOP')



def getMockData(mockDataPath,fileFormat):
    days_file = open(mockDataPath,'r')
    mockData = days_file.read()

    if fileFormat == 'json':
        return literal_eval(mockData);
    elif fileFormat == 'csv':
        f = StringIO(mockData)
        reader = csv.reader(f, delimiter=',')
        for row in reader:
            print('\t'.join(row))
        return '[]'
    else:
        print("Format not found.")

if __name__ == '__main__':

    if len(sys.argv) < 4:
        print("Please enter: \n - the path to the configuration file \n - the instrument_id \n - the observation_id \n - the analysis_session_type_id \n - sleep time (lower bound) \n - sleep time (upper bound)")
        exit()


    configFilePath = sys.argv[1]
    instrument_id = sys.argv[2]
    observation_id = sys.argv[3]
    analysis_session_type_id = sys.argv[4]
    sleepMin = sys.argv[5]
    sleepMax = sys.argv[6]
    mockDataPath = sys.argv[7]
    fileFormat = sys.argv[8]



    sendMockData(instrument_id, observation_id, analysis_session_type_id)
