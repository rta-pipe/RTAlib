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

#!/usr/bin/env python

import unittest
from sys import path
from os.path import dirname, abspath, realpath
from time import sleep
from os import environ

rootFolder = dirname(dirname(dirname(dirname(realpath(__file__)))))
path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.RTAInterface  import RTA_DLTEST_DB
from PyRTAlib.Utils.UtilsUT import getConfig
from PyRTAlib.Utils.UtilsUT import UtilsMySql
from PyRTAlib.Utils.UtilsUT import UtilsRedis


if 'RTALIBCONFIG' not in environ:
    print('\n[RTA_DL_DB__unittest.py] No RTALIBCONFIG env var found. Please export it and let it points to the rtalibconfig_testing configuration file.\n')
    exit(1)
config_file_path = environ['RTALIBCONFIG']

DEBUG = False
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
        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 0)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)
        self.assertEqual(True, rta_dltest_db_mysql.dbConnector.testConnection())
        self.assertEqual(True, rta_dltest_db_redis.dbConnector.testConnection())

    # test closed connection
    def test_closed_connection(self):
        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 0)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)
        self.assertEqual(True, rta_dltest_db_mysql.dbConnector.testConnection())
        self.assertEqual(True, rta_dltest_db_redis.dbConnector.testConnection())
        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())
        self.assertEqual(False, rta_dltest_db_mysql.dbConnector.testConnection())
        self.assertEqual(False, rta_dltest_db_redis.dbConnector.testConnection())


    # test synchronous mode active
    def test_set_synchronous_mode(self):
        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 0)
        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)
        self.assertEqual(rta_dltest_db_mysql.multithreading, False)
        self.assertEqual(rta_dltest_db_redis.multithreading, False)



    # test synchronous mode streaming insert
    def test_streaming_insert(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))

        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 0)\
            .set('General', 'batchsize', 1)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(1, utilsMySql.countElements(tableName))
        self.assertEqual(1, utilsRedis.countElements(tableName))

        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())


    # test synchronous mode batch insert
    def test_batch_insert(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))

        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 0)\
            .set('General', 'batchsize', 2)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(0, utilsMySql.countElements(tableName))
        self.assertEqual(0, utilsRedis.countElements(tableName))

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(2, utilsMySql.countElements(tableName))
        self.assertEqual(2, utilsRedis.countElements(tableName))

        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())



    # test synchronous mode batch insert closing before finish
    def test_batch_insert_closed_connection_before_finish(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))

        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 0)\
            .set('General', 'batchsize', 2)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(0, utilsMySql.countElements(tableName))
        self.assertEqual(0, utilsRedis.countElements(tableName))

        self.assertEqual(True, rta_dltest_db_mysql.waitAndClose())
        self.assertEqual(True, rta_dltest_db_redis.waitAndClose())

        self.assertEqual(1, utilsMySql.countElements(tableName))
        self.assertEqual(1, utilsRedis.countElements(tableName))


    # multi threading mode active with numberofthreads >= 1
    def test_multi_threading_mode_active(self):

        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General','numberofthreads', 1)

        rta_dltest_db = RTA_DLTEST_DB('mysql', config_file_path)

        self.assertEqual(rta_dltest_db.multithreading, True)

        rta_dltest_db.waitAndClose()



    # test multi threading streaming (one asynchronous thread)
    def test_asynchronous_single_thread_streaming_insert(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))


        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 1)\
            .set('General', 'batchsize', 1)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        self.assertEqual(rta_dltest_db_mysql.multithreading, True)
        self.assertEqual(rta_dltest_db_redis.multithreading, True)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.3)

        self.assertEqual(1, utilsMySql.countElements(tableName))
        self.assertEqual(1, utilsRedis.countElements(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(1, s1[0])
        self.assertEqual(1, s2[0])

        # check statistics

    # test multi threading batch (one asynchronous thread)
    def test_asynchronous_single_thread_batch_insert(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))


        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 1)\
            .set('General', 'batchsize', 2)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        self.assertEqual(rta_dltest_db_mysql.multithreading, True)
        self.assertEqual(rta_dltest_db_redis.multithreading, True)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.3)

        self.assertEqual(0, utilsMySql.countElements(tableName))
        self.assertEqual(0, utilsRedis.countElements(tableName))

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.3)

        self.assertEqual(2, utilsMySql.countElements(tableName))
        self.assertEqual(2, utilsRedis.countElements(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(2, s1[0])
        self.assertEqual(2, s2[0])


    # test multi threading batch (one asynchronous thread)
    def test_asynchronous_single_thread_batch_insert_closing_connection_before_finish(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))


        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 1)\
            .set('General', 'batchsize', 2)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        self.assertEqual(rta_dltest_db_mysql.multithreading, True)
        self.assertEqual(rta_dltest_db_redis.multithreading, True)

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(1, s1[0])
        self.assertEqual(1, s2[0])

        sleep(0.2)

        self.assertEqual(1, utilsMySql.countElements(tableName))
        self.assertEqual(1, utilsRedis.countElements(tableName))

    # test multi threading streaming (multiple threads)
    def test_asynchronous_multi_thread_streaming_insert(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))

        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 2)\
            .set('General', 'batchsize', 1)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        self.assertEqual(rta_dltest_db_mysql.multithreading, True)
        self.assertEqual(rta_dltest_db_redis.multithreading, True)

        for i in range(50):
            rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
            rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.3)

        self.assertEqual(50, utilsMySql.countElements(tableName))
        self.assertEqual(50, utilsRedis.countElements(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(50, s1[0])
        self.assertEqual(50, s2[0])

    # test multi threading batch (multiple threads)
    """ It's difficult to understand when a thread commits the transaction

    def test_asynchronous_multi_thread_batch_insert(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))


        getConfig(config_file_path, DEBUG, reload=True).set('General', 'numberofthreads', 2).set('General', 'batchsize', 3)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        for i in range(2):
            rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
            rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.5)

        self.assertEqual(0, utilsMySql.countElements(tableName))
        self.assertEqual(0, utilsRedis.countElements(tableName))

        rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.5)

        self.assertEqual(3, utilsMySql.countElements(tableName))
        self.assertEqual(3, utilsRedis.countElements(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(3, s1[0])
        self.assertEqual(3, s2[0])
    """

    # test multi threading batch closing connection before finish (multiple threads)
    def test_asynchronous_multi_thread_batch_insert_closing_connection_before_finish(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))


        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 2)\
            .set('General', 'batchsize', 3)

        rta_dltest_db_mysql = RTA_DLTEST_DB('mysql', config_file_path)
        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        self.assertEqual(rta_dltest_db_mysql.multithreading, True)
        self.assertEqual(rta_dltest_db_redis.multithreading, True)

        for i in range(2):
            rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
            rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        sleep(0.5)

        self.assertEqual(0, utilsMySql.countElements(tableName))
        self.assertEqual(0, utilsRedis.countElements(tableName))

        s1 = rta_dltest_db_mysql.waitAndClose()
        s2 = rta_dltest_db_redis.waitAndClose()

        self.assertEqual(2, s1[0])
        self.assertEqual(2, s2[0])

        self.assertEqual(2, utilsMySql.countElements(tableName))
        self.assertEqual(2, utilsRedis.countElements(tableName))



    # Test Pythonic usage
    def test_pythonic_usage(self):
        self.assertEqual(True, utilsMySql.deleteElements(tableName))
        self.assertEqual(True, utilsRedis.deleteElements(tableName))

        getConfig(config_file_path, DEBUG, reload=True)


        with RTA_DLTEST_DB('mysql', config_file_path) as rta_dltest_db_mysql:
            rta_dltest_db_mysql.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(1, utilsMySql.countElements(tableName))


        with RTA_DLTEST_DB('redis-basic', config_file_path) as rta_dltest_db_redis:
            rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )

        self.assertEqual(1, utilsRedis.countElements(tableName))




    # test Redis Publisher synchronous mode
    def test_publish_for_dtr(self):
        self.assertEqual(True, utilsRedis.deleteElements(tableName))

        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 0)\
            .set('Dtr', 'active', 'yes')\
            .set('Dtr', 'inputchannel', 'test_channel')

        channel = utilsRedis.getRedisChannelListener('test_channel')
        self.assertEqual(True, bool(channel))

        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

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


    # test Redis Publisher multithreading mode
    def test_publish_for_dtr_multithread(self):
        self.assertEqual(True, utilsRedis.deleteElements(tableName))

        getConfig(config_file_path, DEBUG, reload=True)\
            .set('General', 'numberofthreads', 2)\
            .set('Dtr', 'active', 'yes')\
            .set('Dtr', 'inputchannel', 'test_channel')

        channel = utilsRedis.getRedisChannelListener('test_channel')
        self.assertEqual(True, bool(channel))

        rta_dltest_db_redis = RTA_DLTEST_DB('redis-basic', config_file_path)

        sleep(0.3)

        countMessages = 0
        for message in channel.listen():
            countMessages += 1
            break

        rta_dltest_db_redis.insertEvent( *RTA_DLTEST_DB.getRandomEvent() )
        for message in channel.listen():
            countMessages += 1
            break

        s1 = rta_dltest_db_redis.waitAndClose()
        self.assertEqual(1, s1[0])

        for message in channel.listen():
            countMessages += 1
            break

        self.assertEqual(3, countMessages)



if __name__ == '__main__':

    import xmlrunner
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))
