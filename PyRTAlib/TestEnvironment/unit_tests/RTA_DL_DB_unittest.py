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
import sys
import os
from os.path import dirname, abspath, join
import time
from random import randint, uniform

rootFolder = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))
sys.path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.RTAInterface  import RTA_DLTEST_DB
from PyRTAlib.Utils         import Config
#from PyRTAlib.DBConnectors  import MySqlDBConnector
from UtilsUT import getConfig
from UtilsUT import UtilsMySql
from UtilsUT import UtilsRedis


DEBUG = False
config_file_path = '../'
tableName = 'rtalib_dl_test_table'
utilsMySql = UtilsMySql(config_file_path)
utilsRedis = UtilsRedis(config_file_path)

"""
 (                           (         )                  (      (
 )\ )    *   )     (         )\ )   ( /(    *   )         )\ )   )\ )     (         (
(()/(  ` )  /(     )\       (()/(   )\()) ` )  /(   (    (()/(  (()/(     )\        )\    (
 /(_))  ( )(_)) ((((_)(      /(_)) ((_)\   ( )(_))  )\    /(_))  /(_)) ((((_)(    (((_)   )\
(_))   (_(_())   )\ _ )\    (_))    _((_) (_(_())  ((_)  (_))   (_))_|  )\ _ )\   )\___  ((_)
| _ \  |_   _|   (_)_\(_)   |_ _|  | \| | |_   _|  | __| | _ \  | |_    (_)_\(_) ((/ __| | __|
|   /    | |      / _ \      | |   | .` |   | |    | _|  |   /  | __|    / _ \    | (__  | _|
|_|_\    |_|     /_/ \_\    |___|  |_|\_|   |_|    |___| |_|_\  |_|     /_/ \_\    \___| |___|

"""

class RTA_DLTEST_DB_interface(unittest.TestCase):

    def test_base(self):
        self.assertEqual(True, 1==1)

    # test connection
    def test_connection(self):
        getConfig(config_file_path, DEBUG, reload=True)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)
        self.assertEqual(True, rta_dltest_db_mysql.dbConnector.testConnection())
        self.assertEqual(True, rta_dltest_db_redis.dbConnector.testConnection())

    # test closed connection
    def test_closed_connection(self):
        getConfig(config_file_path, DEBUG, reload=True)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)
        self.assertEqual(True, rta_dltest_db_mysql.dbConnector.testConnection())
        self.assertEqual(True, rta_dltest_db_redis.dbConnector.testConnection())
        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())
        self.assertEqual(False, rta_dltest_db_mysql.dbConnector.testConnection())
        self.assertEqual(False, rta_dltest_db_redis.dbConnector.testConnection())



    # test single threading straming insert
    def test_streaming_insert(self):
        utilsMySql.truncateTable(tableName)
        utilsRedis.deleteKey(tableName)

        getConfig(config_file_path, DEBUG, reload=True)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        rta_dltest_db_mysql.insertEvent( *rta_dltest_db_mysql.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *rta_dltest_db_redis.getRandomEvent() )

        self.assertEqual(1, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(1, utilsRedis.countSortedSetMembers(tableName))

        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())


    # test single threading batch insert
    def test_batch_insert(self):

        utilsMySql.truncateTable(tableName)
        utilsRedis.deleteKey(tableName)

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'batchsize', 2)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        rta_dltest_db_mysql.insertEvent( *rta_dltest_db_mysql.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *rta_dltest_db_redis.getRandomEvent() )

        self.assertEqual(0, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(0, utilsRedis.countSortedSetMembers(tableName))

        rta_dltest_db_mysql.insertEvent( *rta_dltest_db_mysql.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *rta_dltest_db_redis.getRandomEvent() )

        self.assertEqual(2, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(2, utilsRedis.countSortedSetMembers(tableName))

        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())



    # test single threading batch insert closing before finish
    def test_batch_insert_closed_connection_before_finish(self):

        utilsMySql.truncateTable(tableName)
        utilsRedis.deleteKey(tableName)

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'batchsize', 2)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        rta_dltest_db_mysql.insertEvent( *rta_dltest_db_mysql.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *rta_dltest_db_redis.getRandomEvent() )

        self.assertEqual(0, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(0, utilsRedis.countSortedSetMembers(tableName))

        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())

        self.assertEqual(1, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(1, utilsRedis.countSortedSetMembers(tableName))


    # pure multi threading mode active with flag = true or numberofthreads > 1
    def test_pure_multi_threading_mode_active(self):

        getConfig(config_file_path, DEBUG, reload=True)
        rta_dltest_db = RTA_DLTEST_DB('mysql', config_file_path, True)
        self.assertEqual(rta_dltest_db.pure_multithreading, True)
        rta_dltest_db.waitAndClose()

        getConfig(config_file_path, DEBUG, reload=True).set('General','numberofthreads', 2)
        rta_dltest_db = RTA_DLTEST_DB('mysql', config_file_path, False)
        self.assertEqual(rta_dltest_db.pure_multithreading, True)
        rta_dltest_db.waitAndClose()



    # test multi threading
    def test_aynchronous_single_thread_streaming_insert(self):

        utilsMySql.truncateTable(tableName)
        utilsRedis.deleteKey(tableName)

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 1)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path, True)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path, True)

        rta_dltest_db_mysql.insertEvent( *rta_dltest_db_mysql.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *rta_dltest_db_redis.getRandomEvent() )

        time.sleep(1)

        self.assertEqual(1, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(1, utilsRedis.countSortedSetMembers(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        print(s1)
        print(s2)



    # test Redis Publisher


if __name__ == '__main__':

    unittest.main()
