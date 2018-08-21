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

import numpy as np

from PyRTAlib.DBConnectors  import MySqlDBConnector, RedisDBConnectorBASIC
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB
from PyRTAlib.Utils         import read_data_from_fits



class ThreadSafeIndex:

    def __init__(self, start=0):
        self.lock = threading.Lock()
        self.value = start

    def getDataSafeIndexAndIncr(self, incr):
        self.lock.acquire()
        index = self.value
        try:
            self.value = self.value + incr
        finally:
            self.lock.release()
            return index

def threadInsertData(threadid, batchsize, dataSafeIndex, obsIdSafeIndex):

    rta_dl3astri = RTA_DL3ASTRI_DB(database)
    rta_dl3astri.dbConnector.batchsize = batchsize
    rta_dl3astri.dbConnector.debug = False

    obsId = obsIdSafeIndex.getDataSafeIndexAndIncr(1)

    numberOfEventsToInsert = int(int(numberOfEvents)/int(numberOfThreads))

    index = dataSafeIndex.getDataSafeIndexAndIncr(numberOfEventsToInsert)

    #print("Thread {}. I am going to insert {} events".format(threadid, numberOfEventsToInsert))

    start_perf = time.perf_counter()

    for i in range(numberOfEventsToInsert):
        idx = index + i
        rta_dl3astri.insertEvent(  evt3data[idx][0],
                                   evt3data[idx][1],
                                   evt3data[idx][2],
                                   evt3data[idx][3],
                                   evt3data[idx][4],
                                   evt3data[idx][5],
                                   evt3data[idx][6],
                                   evt3data[idx][7],
                                   obsId
                                 )
    rta_dl3astri.close()
    end_perf = time.perf_counter()


    #executionTime = end_perf - start_perf
    #eventSec = numberOfEventsToInsert/executionTime
    #print("(Thread: {}) Event/Sec: {}  ExeTime: {}".format(threadid, round(eventSec,3), round(executionTime,3)))



def test_multithread(batchsize, dataSafeIndex, obsIdSafeIndex):

    start_perf = time.perf_counter()
    for jj in range(int(numberOfThreads)):

        t = threading.Thread(target=threadInsertData, args=(jj, batchsize, dataSafeIndex, obsIdSafeIndex,))
        t.start()


    main_thread = threading.main_thread()
    for t in threading.enumerate():
        if t is not main_thread:
            t.join()
    end_perf = time.perf_counter()
    executionTime = end_perf - start_perf

    print("[Batch size: {}]. Total Event/Sec: {},  Total time: {}".format(batchsize, round(int(numberOfEvents)/executionTime, 2) ,round(executionTime,3)))






if __name__ == '__main__':

    os.environ['RTACONFIGFILE'] = './'

    database = sys.argv[1]
    fitspath = sys.argv[2]
    numberOfEvents = sys.argv[3]
    numberOfThreads = sys.argv[4]

    """
    Reading FITS data
    """
    print("Reading data..")
    evt3data = read_data_from_fits(fitspath)
    print(evt3data[0])

    """
        Truncate Table
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
        keys = redisConn.decodeResponseList(redisConn.conn.keys('evt3:*'))
        if len(keys) > 0:
            redisConn.conn.delete(*keys)
        redisConn.conn.set('uniqueId:evt3', 0)
    else:
        print("Error!! Unknown database {}".format(database))
        exit()

    dataSafeIndex = ThreadSafeIndex()
    obsIdSafeIndex = ThreadSafeIndex()

    """
    Starting testing
    """
    print("\n**************************\n******  START TEST  ******\n**************************\n")

    print("Number of events to insert: {}".format(numberOfEvents))
    print("Number of threads: {}".format(numberOfThreads))
    print("Number of events per thread: {}".format(int(int(numberOfEvents)/int(numberOfThreads))))
    print("\n")

    p = test_multithread(1, dataSafeIndex, obsIdSafeIndex)

    # TEST - BATCHSIZE = 2
    p = test_multithread(2, dataSafeIndex, obsIdSafeIndex)


    # TEST - BATCHSIZE = 50
    p = test_multithread(50, dataSafeIndex, obsIdSafeIndex)


    # TEST - BATCHSIZE = 100
    p = test_multithread(100, dataSafeIndex, obsIdSafeIndex)


    # TEST - BATCHSIZE = 200
    p = test_multithread(200, dataSafeIndex, obsIdSafeIndex)

    # TEST - BATCHSIZE = 400
    p = test_multithread(400, dataSafeIndex, obsIdSafeIndex)

    # TEST - BATCHSIZE = 800
    p = test_multithread(800, dataSafeIndex, obsIdSafeIndex)

    # TEST - BATCHSIZE = 1000
    p = test_multithread(1000, dataSafeIndex, obsIdSafeIndex)

    # TEST - BATCHSIZE = 1000
    p = test_multithread(1500, dataSafeIndex, obsIdSafeIndex)

    # TEST - BATCHSIZE = 1000
    p = test_multithread(2000, dataSafeIndex, obsIdSafeIndex)
