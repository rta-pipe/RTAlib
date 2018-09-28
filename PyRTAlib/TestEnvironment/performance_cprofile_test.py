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
import cProfile
from random import randint, uniform
from time import time

from PyRTAlib.DBConnectors  import MySqlDBConnector
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB_old
from PyRTAlib.Utils         import read_data_from_fits
from PyRTAlib.Utils         import Config


def test(numberOfEvents, batchsize, numberofthreads):

        numberOfEvents = numberOfEvents

        config = Config('./')
        config.reload('./')
        config.set('General', 'debug', 'no')
        config.set('General', 'numberofthreads', numberofthreads)
        config.set('MySql', 'batchsize', batchsize)

        obsId = getUniqueObservationId()

        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('mysql')

        for i in range(int(numberOfEvents)):
            RTA_DL3ASTRI.insertEvent(  i,
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



def getUniqueObservationId():
    global OBSID
    OBSID += 1
    return OBSID


def simulate_evt3_data(numberOfEvents):
    evt3data = []
    for i in range(int(numberOfEvents)):
        rndEvent = []
        rndEvent.append(randint(0, 9999999))
        rndEvent.append(time())
        rndEvent.append(randint(0, 9999999))
        rndEvent.append(uniform(-180,180))
        rndEvent.append(uniform(-90, 90))
        rndEvent.append(uniform(0, 0.5))
        rndEvent.append(uniform(0, 0.1))
        rndEvent.append(uniform(0, 0.1))
        rndEvent.append(randint(0, 9999999))
        rndEvent.append(randint(0, 9999999))
        evt3data.append(rndEvent)
    return evt3data


if __name__ == '__main__':

    OBSID = 0

    os.environ['RTACONFIGFILE'] = './'


    """
        Simulating FITS data
    """
    print("Generating data..")
    evt3data = simulate_evt3_data(10000)


    """
        Truncate Table
    """
    print("Truncating table..")
    mysqlConn = MySqlDBConnector('./')
    mysqlConn.connect()
    if not mysqlConn.executeQuery('delete from evt3'):
        exit()
    mysqlConn.close()

    print("Running test.. number of events = 10000, batchsize = 1")
    cProfile.run('test(10000, 1, 1)', sort='tottime')

    print("Running test.. number of events = 10000, batchsize = 200")
    cProfile.run('test(10000, 200, 1)', sort='tottime')
