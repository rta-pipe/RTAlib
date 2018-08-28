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

import unittest
import os
import time
from random import randint, uniform

from PyRTAlib.Utils         import Config
from PyRTAlib.DBConnectors  import MySqlDBConnector, RedisDBConnectorBASIC
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB_old
from PyRTAlib.DTRInterface  import DTR


def getConfiguration(debug='yes', numberOfThreads=1, batchsize=1, dtr='no', debugDtr='no'):
    config = Config('./')
    config.set('General', 'debug', debug)
    config.set('General', 'numberofthreads', numberOfThreads)
    config.set('General', 'batchsize', batchsize)
    config.set('Dtr', 'active', dtr)
    return config



class Integration_rtalib_dtr_gui(unittest.TestCase):

    os.environ['RTACONFIGFILE'] = './'



    def test_dtr(self):
        print("/\____/\____/\____/\____/\__test__/\__dtr__/\_____/\_____/\____/\____/\_")

        # Configuration
        config = getConfiguration(debug='yes', numberOfThreads=2, batchsize=1, dtr='yes', debugDtr='yes')
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()

        # TODO Delete old data
        #redisConn.delete()


        # Connect and insert random event
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('mysql')

        for i in range(10000):
            time.sleep(3)
            RTA_DL3ASTRI.insertEvent( *RTA_DL3ASTRI.getRandomEvent() )


        RTA_DL3ASTRI.waitAndClose()

        # TODO check number of inserted elements
        self.assertEqual(True, True)






if __name__ == '__main__':
    unittest.main()
