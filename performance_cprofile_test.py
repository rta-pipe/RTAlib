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

from PyRTAlib.DBConnectors  import MySqlDBConnector
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB
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

        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')

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



def getUniqueObservationId():
    global OBSID
    OBSID += 1
    return OBSID

if __name__ == '__main__':

    OBSID = 0

    os.environ['RTACONFIGFILE'] = './'

    fitspath = sys.argv[1]

    """
        Reading FITS data
    """
    print("Reading data..")
    evt3datafits = read_data_from_fits(fitspath)
    evt3data = []
    for fitsevent in evt3datafits:
        evt3data.append(fitsevent)

    print(evt3data[0])

    """
        Truncate Table
    """
    mysqlConn = MySqlDBConnector('./')
    mysqlConn.connect()
    if not mysqlConn.executeQuery('delete from evt3'):
        exit()
    mysqlConn.close()

    cProfile.run('test(1000, 1, 1)', sort='tottime')

    cProfile.run('test(1000, 200, 1)', sort='tottime')
