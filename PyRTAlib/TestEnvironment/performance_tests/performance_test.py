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

from sys import path, argv
from os.path import dirname, realpath

from time import perf_counter, strftime, sleep
from collections import namedtuple
from statistics import mean, stdev

import json

rootFolder = dirname(dirname(dirname(dirname(realpath(__file__)))))
path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.RTAInterface  import RTA_DLTEST_DB

from PyRTAlib.Utils.UtilsUT import getConfig
from PyRTAlib.Utils.UtilsUT import UtilsMySql
from PyRTAlib.Utils.UtilsUT import UtilsRedis

def getUtils(dbName, configurationFilePath):
    if dbName == 'redis-basic':
        return UtilsRedis(configurationFilePath)
    elif dbName == 'mysql':
        return UtilsMySql(configurationFilePath)
    else:
        print("The database: {} is not supported. Please retry.".format(dbName))
        exit()


def simulate_evt3_data(numberOfEvents):
    print("Simulating data...patience..")
    evt3data = []
    for i in range(int(numberOfEvents)):
        evt3data.append(RTA_DLTEST_DB.getRandomEvent())
    return evt3data

def resetUniqueValue():
    global UNIQUE_VALUE
    UNIQUE_VALUE = 0

def getUniqueValue():
    global UNIQUE_VALUE
    UNIQUE_VALUE += 1
    return UNIQUE_VALUE

def synchronous_performance_test(batchsize, numberofthreads, numberOfIterationPerTest, utilsObj):

    getConfig(configurationFilePath, False, reload=True)\
        .set('General', 'batchsize', batchsize)\
        .set('General', 'numberofthreads', numberofthreads)\
        .set('Dtr', 'active', 'no')\
        .set('MySqlPipelineDatabase', 'active', 'no')

    eventSecList = []
    executionTimeList = []

    for jj in range(numberOfIterationPerTest):

        resetUniqueValue()

        # Delete data
        utilsObj.deleteElements(tableName)
        elementsInserted = utils.countElements(tableName)
        if elementsInserted != 0:
            print("--> COUNT DATA CHECK FAILED!! Events in table: {}/0".format(elementsInserted))
            exit()

        sleep(1)

        RTA_DLTEST = RTA_DLTEST_DB(database)

        start_perf = perf_counter()
        for i in range(numberOfEvents):

            evt3data[i][0] = getUniqueValue()
            RTA_DLTEST.insertEvent( *evt3data[i] )

        RTA_DLTEST.waitAndClose()
        end_perf = perf_counter()

        executionTime = end_perf - start_perf
        eventSec = numberOfEvents/executionTime
        eventSecList.append(eventSec)
        executionTimeList.append(executionTime)
        print(".", end='', flush=True)

    Perf = namedtuple('res', ['avg', 'stddev'])

    avgES = mean(eventSecList)
    stddevES = stdev(eventSecList)
    ES = Perf(avgES, stddevES)

    avgET = mean(executionTimeList)
    stddevET = stdev(executionTimeList)
    ET = Perf(avgET, stddevET)

    print("\n{} +- {}\n{} +- {}".format(round(ES.avg,2), round(ES.stddev,2), round(ET.avg,2), round(ET.stddev,2)))
    return ES

def asynchronous_performance_test(batchsize, numberofthreads, numberOfIterationPerTest, utilsObj):

    getConfig(configurationFilePath, False, reload=True)\
        .set('General', 'batchsize', batchsize)\
        .set('General', 'numberofthreads', numberofthreads)\
        .set('Dtr', 'active', 'no')\
        .set('MySqlPipelineDatabase', 'active', 'no')

    eventSecList = []
    executionTimeList = []

    for jj in range(numberOfIterationPerTest):

        resetUniqueValue()

        # Delete data
        utilsObj.deleteElements(tableName)
        elementsInserted = utils.countElements(tableName)
        if elementsInserted != 0:
            print("--> COUNT DATA CHECK FAILED!! Events in table: {}/0".format(elementsInserted))
            exit()

        sleep(1)

        RTA_DLTEST = RTA_DLTEST_DB(database, '')

        for i in range(int(numberOfEvents)):

            evt3data[i][0] = getUniqueValue()
            RTA_DLTEST.insertEvent( *evt3data[i] )

        stats = RTA_DLTEST.waitAndClose()

        eventSecList.append(stats[2])
        executionTimeList.append(stats[1])
        print(".", end='', flush=True)

    Perf = namedtuple('res', ['avg', 'stddev'])

    avgES = mean(eventSecList)
    stddevES = stdev(eventSecList)
    ES = Perf(avgES, round(stddevES,2))

    avgET = mean(executionTimeList)
    stddevET = stdev(executionTimeList)
    ET = Perf(avgET, round(stddevET,2))

    print("\n{} +- {}\n{} +- {}".format(round(ES.avg,2), round(ES.stddev,2), round(ET.avg,2), round(ET.stddev,2)))
    return ES





if __name__ == '__main__':

    if len(argv) < 5:
        print("Please enter:\
                    \n - the database to be used (mysql or redis-basic)\
                    \n - the number of events to be inserted\
                    \n - the number of times a test must be repeated (>1)\
                    \n - the path to the configuration file")
        exit()

    database = argv[1]
    numberOfEvents = int(argv[2])
    numberOfIterationPerTest = int(argv[3])
    configurationFilePath = argv[4]
    tableName = 'evt3_performance'


    utils = getUtils(database, configurationFilePath)

    # Test configuration
    threads = [0, 10, 36, 50]
    batchsizes = [1, 100, 1000]#10, 50, 100, 200, 400, 800, 1600, 3200]

    # Simulating data
    # numberOfElementsToInsert = len(threads)*len(batchsizes)*int(numberOfEvents)
    evt3data = simulate_evt3_data(numberOfEvents)

    UNIQUE_VALUE = 0


    print("\n**************************\n******  START TEST  ******\n**************************\n")
    print("Number of events: {}".format(numberOfEvents))
    print("--> Number of threads: x, Batch size: y")
    print("Events/sec, Execution time")


    outputArray = []

    for idx_t, threadsNum in enumerate(threads):

        for idx_b, batchSize in enumerate(batchsizes):


            print("\n--> Number of threads: {}, Batch size: {}".format(threadsNum, batchSize))
            if threadsNum == 0:
                p = synchronous_performance_test(batchSize, threadsNum, numberOfIterationPerTest, utils)
            else:
                p = asynchronous_performance_test(batchSize, threadsNum, numberOfIterationPerTest, utils)

            resultObj = {}
            resultObj['threads'] = threadsNum
            resultObj['batchsize'] = batchSize
            resultObj['eventrate'] = p[0]
            resultObj['error'] = p[1]
            outputArray.append(resultObj)

            # Check tables
            elementsInserted = utils.countElements(tableName)
            if elementsInserted != numberOfEvents:
                print("--> COUNT DATA CHECK FAILED!! Inserted: {}/{}".format(elementsInserted, numberOfEvents))
                exit()


    print("\n\n")

    timestr = strftime("%Y%m%d-%H%M%S")
    with open('performance_pyrtalib_'+timestr+'.txt', 'w') as file:
         file.write(json.dumps(outputArray))

    print(json.dumps(outputArray))
