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
import sys
import time
import collections
import statistics
import threading

#import matplotlib
#import matplotlib.pyplot as plt
#import numpy as np

from PyRTAlib.DBConnectors  import MySqlDBConnector, RedisDBConnectorBASIC
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB_old
from PyRTAlib.Utils         import read_data_from_fits
from PyRTAlib.Utils         import Config


def deleteData(database):

    config = Config('./')

    """
        Deleting existing data
    """
    if database == 'mysql':
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        if not mysqlConn.executeQuery('delete from '+config.get('General', 'evt3modelname')):
            exit()
        if not mysqlConn.executeQuery('delete from '+config.get('General', 'evt3modelname')+'_memory'):
            exit()
        mysqlConn.close()
    elif database == 'redis' or database == 'redis-basic':
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()
        redisConn.conn.delete(config.get('General', 'evt3modelname'))
    else:
        print("Error!! Unknown database {}".format(database))
        exit()


def test(batchsize, numberofthreads):

    config = Config('./')
    config.reload('./')
    config.set('General', 'debug', 'no')
    config.set('General', 'numberofthreads', numberofthreads)
    config.set('General', 'batchsize', batchsize)
    config.set('Dtr', 'active', 'no')


    eventSecList = []
    executionTimeList = []

    for jj in range(5):

        obsId = getUniqueObservationId()
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old(database)

        start_perf = time.perf_counter()
        for i in range(int(numberOfEvents)):
            RTA_DL3ASTRI.insertEvent(  evt3data[i][0],
                                       evt3data[i][1],
                                       evt3data[i][2],
                                       evt3data[i][3],
                                       evt3data[i][4],
                                       evt3data[i][5],
                                       evt3data[i][6],
                                       evt3data[i][7],
                                       obsId
                                     )

        RTA_DL3ASTRI.waitAndClose()
        end_perf = time.perf_counter()

        executionTime = end_perf - start_perf
        eventSec = int(numberOfEvents)/executionTime
        eventSecList.append(eventSec)
        executionTimeList.append(executionTime)


    Perf = collections.namedtuple('res', ['avg', 'stddev'])

    avgES = statistics.mean(eventSecList)
    stddevES = statistics.stdev(eventSecList)
    ES = Perf(avgES, stddevES)

    avgET = statistics.mean(executionTimeList)
    stddevET = statistics.stdev(executionTimeList)
    ET = Perf(avgET, stddevET)

    print("{} +- {}\n{} +- {}".format(round(ES.avg,2), round(ES.stddev,2), round(ET.avg,2), round(ET.stddev,2)))
    return ES




def getUniqueObservationId():
    global OBSID
    OBSID += 1
    return OBSID


if __name__ == '__main__':

    OBSID = 0

    os.environ['RTACONFIGFILE'] = './'

    database = sys.argv[1]
    fitspath = sys.argv[2]
    numberOfEvents = sys.argv[3]


    """
        Reading FITS data
    """
    print("Reading data..")
    evt3data = read_data_from_fits(fitspath)
    print(evt3data[0])



    """
        Deleting existing data
    """
    deleteData(database)




    """
        Test configuration
    """
    threads = [1]
    batchsizes = [1, 10, 50, 100, 200, 400, 800, 1600, 3200]

    insertionsNumber = len(threads)*len(batchsizes)*int(numberOfEvents)
    availableData = len(evt3data)

    print("Number of insertions: {}".format(insertionsNumber))
    print("Available data: {}".format(len(evt3data)))

    if insertionsNumber > availableData:
        print("NOT ENOUGH DATA!!")

    """
        Plot
    """
    #w, h = len(batchsizes), len(threads);
    x  = []
    y = []
    erry = []


    """
        Starting testing
    """
    print("\n**************************\n******  START TEST  ******\n**************************\n")
    print("Number of events: {}".format(numberOfEvents))
    print("--> Number of threads: x, Batch size: y")
    print("Events/sec, Execution time")

    for idx_t, t in enumerate(threads):
        for idx_b, b in enumerate(batchsizes):
            print("\n--> Number of threads: {}, Batch size: {}".format(t, b))
            p = test(b,t)
            x.append(b)
            y.append(p[0])
            erry.append(p[1])

            deleteData(database)


    print(x)
    print(y)
    print(erry)

    """
    # Two subplots, the axes array is 1-d
    f, ax = plt.subplots(1)

    ax.plot(np.array(x), np.array(y))
    ax.grid()
    ax.set_title('Event/Sec')
    ax.errorbar(x, y, yerr=erry)

    plt.xlabel('Batch size')
    plt.xlabel('Event/Sec')

    plt.show()
    """
