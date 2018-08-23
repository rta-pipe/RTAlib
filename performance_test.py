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
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB
from PyRTAlib.Utils         import read_data_from_fits
from PyRTAlib.Utils         import Config


def test(batchsize, numberOfThreads):

    config = Config('./')
    config.reload('./')
    config.set('General', 'debug', 'no')
    config.set('General', 'numberofthreads', numberOfThreads)
    config.set('MySql', 'batchsize', batchsize)

    eventSecList = []
    executionTimeList = []

    RTA_DL3ASTRI = RTA_DL3ASTRI_DB(database)

    obsId = getUniqueObservationId()
    #for jj in range(2):

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

    RTA_DL3ASTRI.close()

        #main_thread = threading.main_thread()
        #for t in RTA_DL3ASTRI.getThreads():
        #    if t is not main_thread:
        #        t.join()
    end_perf = time.perf_counter()

    executionTime = end_perf - start_perf
    eventSec = int(numberOfEvents)/executionTime
    # eventSecList.append(eventSec)
    # executionTimeList.append(executionTime)


    # Perf = collections.namedtuple('res', ['avg', 'stddev'])

    #avgES = statistics.mean(eventSecList)
    #stddevES = statistics.stdev(eventSecList)
    #ES = Perf(avgES, stddevES)

    #avgET = statistics.mean(executionTimeList)
    #stddevET = statistics.stdev(executionTimeList)
    #ET = Perf(avgET, stddevET)

    print("Events/Sec: {}  Execution Time: {}".format(batchsize, round(eventSec,2), round(executionTime,2)))
    return (eventSec, executionTime)
    #return ES




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
    if database == 'mysql':
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        if not mysqlConn.executeQuery('delete from evt3'):
            exit()
        mysqlConn.close()
    elif database == 'redis' or database == 'redis-basic':
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()
        redisConn.conn.delete('evt3')
    else:
        print("Error!! Unknown database {}".format(database))
        exit()




    """
        Test configuration
    """
    threads = [1]#, 2, 4, 8]
    batchsizes = [1]#, 2, 50, 100, 200, 400, 800, 1600, 3200]


    """
        Plot
    """
    w, h = len(batchsizes), len(threads);
    x  = []
    y = [[0 for x in range(w)] for y in range(h)]
    #erry = [[0 for x in range(w)] for y in range(h)]


    """
        Starting testing
    """
    print("\n**************************\n******  START TEST  ******\n**************************\n")
    print("Number of events: {}".format(numberOfEvents))


    for idx_t, t in enumerate(threads):
        for idx_b, b in enumerate(batchsizes):
            print("\n--> Number of threads: {}, Batch size: {}".format(t, b))
            e_s, e_t = test(b,t)
            x.append(b)
            y[idx_t].append(e_s)
            #erry[idx_t].append(p[1])

    print(x)
    print(y)
    #print(erry)

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
