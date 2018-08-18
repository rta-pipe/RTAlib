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

from PyRTAlib.DBConnectors  import MySqlDBConnector
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB
from PyRTAlib.Utils         import read_data_from_fits



class ThreadSafeIndex:

    def __init__(self, start=0):
        self.lock = threading.Lock()
        self.value = start

    def getSafeIndex(self):
        self.lock.acquire()
        index = self.value
        try:
            self.value = self.value + 1
        finally:
            self.lock.release()
            return index

def threadInsertData(threadid, batchsize, dataSafeIndex, obsIdSafeIndex):

    rta_dl3astri = RTA_DL3ASTRI_DB('mysql')
    rta_dl3astri.dbConnector.batchsize = batchsize

    obsId = obsIdSafeIndex.getSafeIndex()

    numberOfEventsToInsert = int(int(numberOfEvents)/int(numberOfThreads))

    #print("Thread {}. I am going to insert {} events".format(threadid, numberOfEventsToInsert))

    start_perf = time.perf_counter()
    for i in range(numberOfEventsToInsert):
        index = dataSafeIndex.getSafeIndex()
        rta_dl3astri.insertEvent(  evt3data[index][0],
                                   evt3data[index][1],
                                   evt3data[index][2],
                                   evt3data[index][3],
                                   evt3data[index][4],
                                   evt3data[index][5],
                                   evt3data[index][6],
                                   evt3data[index][7],
                                   obsId
                                 )
    rta_dl3astri.close()
    end_perf = time.perf_counter()


    executionTime = end_perf - start_perf
    eventSec = numberOfEventsToInsert/executionTime

    print("Thread {}.\n  Execution time: {}.\n  Event/Sec: {}.".format(threadid, executionTime, eventSec))



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

    print("Test terminated. Total time: {}.".format(executionTime))






if __name__ == '__main__':

    os.environ['RTACONFIGFILE'] = './'

    fitspath = sys.argv[1]
    numberOfEvents = sys.argv[2]
    numberOfThreads = sys.argv[3]

    """
    Reading FITS data
    """
    print("Reading data..")
    evt3data = read_data_from_fits(fitspath)
    print(evt3data[0])


    """
    Truncate Table
    """
    mysqlConn = MySqlDBConnector('./')
    mysqlConn.connect()
    if not mysqlConn.executeQuery('delete from evt3'):
        exit()
    mysqlConn.close()

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

    # TEST - BATCHSIZE = 50
    print("\n\nTEST - Batch size: 1 (Streaming)")
    p = test_multithread(1, dataSafeIndex, obsIdSafeIndex)

    # TEST - BATCHSIZE = 50
    print("\n\nTEST - Batch size: 50")
    p = test_multithread(50, dataSafeIndex, obsIdSafeIndex)


    # TEST - BATCHSIZE = 200
    print("\n\nTEST - Batch size: 200")
    p = test_multithread(200, dataSafeIndex, obsIdSafeIndex)

    # TEST - BATCHSIZE = 500
    print("\n\nTEST - Batch size: 500")
    p = test_multithread(500, dataSafeIndex, obsIdSafeIndex)
