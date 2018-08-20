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

#import matplotlib
#import matplotlib.pyplot as plt
#import numpy as np

from PyRTAlib.DBConnectors  import MySqlDBConnector
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB
from PyRTAlib.Utils         import read_data_from_fits


def test(batchsize):

    eventSecList = []
    executionTimeList = []

    for jj in range(5):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = batchsize
        RTA_DL3ASTRI.dbConnector.debug = False

        start_perf = time.perf_counter()

        obsId = getUniqueObservationId()

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

    print("Number of events: {}".format(numberOfEvents))
    print("Batch size: {}".format(batchsize))
    print("Execution Time: {} +-  {}".format(ET.avg, ET.stddev))
    print("Events/Sec: {} +- {}".format(ES.avg, ES.stddev))
    print("\n\n")
    return ES




def getUniqueObservationId():
    global OBSID
    OBSID += 1
    return OBSID


if __name__ == '__main__':

    OBSID = 0

    os.environ['RTACONFIGFILE'] = './'

    fitspath = sys.argv[1]
    numberOfEvents = sys.argv[2]

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

    """
        Plot
    """
    x  = []
    y = []
    erry = []


    """
        Starting testing
    """
    print("\n**************************\n******  START TEST  ******\n**************************\n")


    # TEST - BATCHSIZE = 1
    p = test(1)
    x.append(1)
    y.append(p[0])
    erry.append(p[1])


    # TEST - BATCHSIZE = 2
    p = test(2)
    x.append(2)
    y.append(p[0])
    erry.append(p[1])

    # TEST - BATCHSIZE = 50
    p = test(50)
    x.append(50)
    y.append(p[0])
    erry.append(p[1])



    # TEST - BATCHSIZE = 100
    p = test(100)
    x.append(100)
    y.append(p[0])
    erry.append(p[1])


    # TEST - BATCHSIZE = 200
    p = test(200)
    x.append(200)
    y.append(p[0])
    erry.append(p[1])

    # TEST - BATCHSIZE = 400
    p = test(400)
    x.append(400)
    y.append(p[0])
    erry.append(p[1])

    # TEST - BATCHSIZE = 800
    p = test(800)
    x.append(800)
    y.append(p[0])
    erry.append(p[1])

    # TEST - BATCHSIZE = 1000
    p = test(1000)
    x.append(1000)
    y.append(p[0])
    erry.append(p[1])

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
