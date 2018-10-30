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
from sys import path
from os.path import dirname, abspath, realpath
from time import sleep

rootFolder = dirname(dirname(dirname(dirname(realpath(__file__)))))
path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.RTAInterface  import RTA_DLTEST_DB
from UtilsUT import getConfig
from UtilsUT import UtilsMySql
from UtilsUT import UtilsRedis


DEBUG = False
config_file_path = '../../../Configs'
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
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(1, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(1, utilsRedis.countSortedSetMembers(tableName))

        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())


    # test single threading batch insert
    def test_batch_insert(self):
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'batchsize', 2)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(0, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(0, utilsRedis.countSortedSetMembers(tableName))

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(2, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(2, utilsRedis.countSortedSetMembers(tableName))

        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())



    # test single threading batch insert closing before finish
    def test_batch_insert_closed_connection_before_finish(self):
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'batchsize', 2)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

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



    # test multi threading streaming (one asynchronous thread)
    def test_aynchronous_single_thread_streaming_insert(self):
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 1)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path, pure_multithreading=True)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path, pure_multithreading=True)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.3)

        self.assertEqual(1, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(1, utilsRedis.countSortedSetMembers(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(1, s1[0])
        self.assertEqual(1, s2[0])

        # check statistics

    # test multi threading batch (one asynchronous thread)
    def test_aynchronous_single_thread_batch_insert(self):
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 1).set('General', 'batchsize', 2)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path, pure_multithreading=True)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path, pure_multithreading=True)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.3)

        self.assertEqual(0, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(0, utilsRedis.countSortedSetMembers(tableName))

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.3)

        self.assertEqual(2, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(2, utilsRedis.countSortedSetMembers(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(2, s1[0])
        self.assertEqual(2, s2[0])


    # test multi threading batch (one asynchronous thread)
    def test_aynchronous_single_thread_batch_insert_closing_connection_before_finish(self):
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 1).set('General', 'batchsize', 2)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path, pure_multithreading=True)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path, pure_multithreading=True)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(1, s1[0])
        self.assertEqual(1, s2[0])

        sleep(0.2)

        self.assertEqual(1, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(1, utilsRedis.countSortedSetMembers(tableName))

    # test multi threading streaming (multiple threads)
    def test_aynchronous_multi_thread_streaming_insert(self):
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 2)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        for i in range(50):
            rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
            rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.3)

        self.assertEqual(50, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(50, utilsRedis.countSortedSetMembers(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(50, s1[0])
        self.assertEqual(50, s2[0])

    # test multi threading batch (multiple threads)
    """ It's difficult to understand when a thread commits the transaction

    def test_aynchronous_multi_thread_batch_insert(self):
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 2).set('General', 'batchsize', 3)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        for i in range(2):
            rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
            rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.5)

        self.assertEqual(0, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(0, utilsRedis.countSortedSetMembers(tableName))

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.5)

        self.assertEqual(3, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(3, utilsRedis.countSortedSetMembers(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(3, s1[0])
        self.assertEqual(3, s2[0])
    """

    # test multi threading batch closing connection before finish (multiple threads)
    def test_aynchronous_multi_thread_batch_insert_closing_connection_before_finish(self):
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 2).set('General', 'batchsize', 3)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        for i in range(2):
            rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
            rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.5)

        self.assertEqual(0, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(0, utilsRedis.countSortedSetMembers(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(2, s1[0])
        self.assertEqual(2, s2[0])

        self.assertEqual(2, utilsMySql.countRowsInTable(tableName))
        self.assertEqual(2, utilsRedis.countSortedSetMembers(tableName))



    # Test Pythonic usage
    def test_pythonic_usage(self):
        self.assertEqual(True, utilsMySql.truncateTable(tableName))
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True)
        with RTA_DLTEST_DB('mysql', config_file_path) as rta_dltest_db_mysql:
            rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(1, utilsMySql.countRowsInTable(tableName))


        with RTA_DLTEST_DB('redis-basic', config_file_path) as rta_dltest_db_redis:
            rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(1, utilsRedis.countSortedSetMembers(tableName))




    # test Redis Publisher
    def test_publish_for_dtr(self):
        self.assertEqual(True, utilsRedis.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True).set('Dtr', 'active', 'yes').set('Dtr', 'inputchannel', 'test_channel')

        channel = utilsRedis.getRedisChannelListener('test_channel')
        self.assertEqual(True, bool(channel))

        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        self.assertEqual(True, rta_dltest_db_redis.dbConnector.testConnection())

        sleep(0.3)

        countMessages = 0
        for message in channel.listen():
            countMessages += 1
            break

        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        for message in channel.listen():
            countMessages += 1
            break

        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())
        for message in channel.listen():
            countMessages += 1
            break

        self.assertEqual(3, countMessages)





if __name__ == '__main__':

    unittest.main()
