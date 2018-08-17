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
import timeit
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

from RTAlib.PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB
from RTAlib.PyRTAlib.Utils         import read_data_from_fits

class PerformanceEVT3ASTRI_method_1():

    def __init__(self,fitspath, numberOfEvents):
        print("Reading data..")
        self.evt3data = read_data_from_fits(fitspath)
        print(self.evt3data[0])
        self.numberOfEvents = numberOfEvents


    def batch_1(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = 1
        for event in self.evt3data:
            RTA_DL3ASTRI.insertEvent_1( event[0], event[1], event[2], event[3], event[4], event[5], event[6], event[7] )
        RTA_DL3ASTRI.close()



    def batch_50(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = 50
        for event in self.evt3data:
            RTA_DL3ASTRI.insertEvent_1( event[0], event[1], event[2], event[3], event[4], event[5], event[6], event[7] )
        RTA_DL3ASTRI.close()

    def batch_100(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = 100
        for event in self.evt3data:
            RTA_DL3ASTRI.insertEvent_1( event[0], event[1], event[2], event[3], event[4], event[5], event[6], event[7] )
        RTA_DL3ASTRI.close()


"""
#evtid=randint(0, 9999999),
#eventidfits=randint(0, 9999999),
#observationid=randint(0, 9999999),
#datarepositoryid=randint(0, 9999999),
#ra_deg=uniform(-180,180),
#dec_deg=uniform(-90, 90),
#energy=uniform(0, 0.5),
#detx=uniform(0, 0.1),
#dety=uniform(0, 0.1),
#mcid=1,
#status=0,
#timerealtt=randint(0, 99999999),
#insert_time=randint(0, 99999999)
"""

if __name__ == '__main__':

    os.environ['RTACONFIGFILE'] = './'

    fitspath = sys.argv[1]
    numberOfEvents = sys.argv[2]

    timeplotdata1 =  np.array([])


    pm1 = PerformanceEVT3ASTRI_method_1(fitspath,numberOfEvents)

    #print(timeit.timeit(pm1.batch_1))

    np.append(timeplotdata1, 1)
    np.append(timeplotdata1, 2)

    fig, ax = plt.subplots()
    ax.plot(timeplotdata1)

    ax.set(xlabel='time (s)', ylabel='voltage (mV)',
           title='About as simple as it gets, folks')
    ax.grid()
    plt.show()
