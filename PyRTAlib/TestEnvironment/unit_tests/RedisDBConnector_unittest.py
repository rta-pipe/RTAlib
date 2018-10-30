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

rootFolder = dirname(dirname(dirname(dirname(realpath(__file__)))))
path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.Utils         import Config
from PyRTAlib.DBConnectors  import RedisDBConnectorBASIC
from UtilsUT import UtilsRedis
from UtilsUT import getConfig


DEBUG = False
config_file_path = '../../../Configs/rtalibconfig_testing'
utils = UtilsRedis(config_file_path)
tableName = 'rtalib_test_table'


"""
 (            (       (      (                   )       )      )                             )    (
 )\ )         )\ )    )\ )   )\ )       (     ( /(    ( /(   ( /(           (      *   )   ( /(    )\ )
(()/(   (    (()/(   (()/(  (()/(       )\    )\())   )\())  )\())  (       )\   ` )  /(   )\())  (()/(
 /(_))  )\    /(_))   /(_))  /(_))    (((_)  ((_)\   ((_)\  ((_)\   )\    (((_)   ( )(_)) ((_)\    /(_))
(_))   ((_)  (_))_   (_))   (_))      )\___    ((_)   _((_)  _((_) ((_)   )\___  (_(_())    ((_)  (_))
| _ \  | __|  |   \  |_ _|  / __|    ((/ __|  / _ \  | \| | | \| | | __| ((/ __| |_   _|   / _ \  | _ \
|   /  | _|   | |) |  | |   \__ \     | (__  | (_) | | .` | | .` | | _|   | (__    | |    | (_) | |   /
|_|_\  |___|  |___/  |___|  |___/      \___|  \___/  |_|\_| |_|\_| |___|   \___|   |_|     \___/  |_|_\

"""


class RedisConnectorBASIC(unittest.TestCase):


    def test_redis_password_set(self):
        getConfig(config_file_path, DEBUG, reload=True)

        redisConn = RedisDBConnectorBASIC(config_file_path)
        self.assertEqual(True, redisConn.connect())
        self.assertEqual(True, redisConn.testConnection())
        self.assertEqual(True, redisConn.close())


    def test_connection_with_wrong_password(self):
        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('Redis', 'password', 'asdasd')

        redisConn = RedisDBConnectorBASIC(config_file_path)
        self.assertEqual(False, redisConn.connect())
        self.assertEqual(False, redisConn.testConnection())
        self.assertEqual(False, redisConn.close())


    def test_connection_success(self):
        getConfig(config_file_path, DEBUG, reload=True)

        redisConn = RedisDBConnectorBASIC(config_file_path)
        self.assertEqual(True, redisConn.connect())
        self.assertEqual(True, redisConn.testConnection())
        self.assertEqual(True, redisConn.close())


    def test_close_not_opened_connection(self):
        getConfig(config_file_path, DEBUG, reload=True)
        redisConn = RedisDBConnectorBASIC(config_file_path)
        self.assertEqual(False, redisConn.close())


    def test_modelname_does_not_exist(self):
        getConfig(config_file_path, DEBUG, reload=True)

        redisConn = RedisDBConnectorBASIC(config_file_path)
        self.assertEqual(True, redisConn.connect())
        self.assertEqual(False, redisConn.insertData('idontexist', {'a': 4, 'b': 2, 'c': 3}))
        self.assertEqual(True, redisConn.close())


    def test_negative_batchsize(self):
        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('General', 'batchsize', -1)

        redisConn = RedisDBConnectorBASIC(config_file_path)
        self.assertEqual(True, redisConn.connect())
        self.assertEqual(False, redisConn.insertData(tableName, {'a': 4, 'b': 2, 'c': 3}))
        self.assertEqual(True, redisConn.close())


    def test_insert_duplicate_data(self):
        getConfig(config_file_path, DEBUG, reload=True)
        redisConn = RedisDBConnectorBASIC(config_file_path)

        self.assertEqual(True, redisConn.connect())
        self.assertEqual(True, redisConn.insertData(tableName, {'a': 5, 'b': 2, 'c': 3}))
        self.assertEqual(True, redisConn.insertData(tableName, {'a': 5, 'b': 2, 'c': 3}))
        self.assertEqual(True, redisConn.close())


    def test_streaming_insert(self):
        self.assertEqual(True, utils.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True)
        redisConn = RedisDBConnectorBASIC(config_file_path)

        self.assertEqual(True, redisConn.connect())
        self.assertEqual(True, redisConn.insertData(tableName, {'a': 4, 'b': 2, 'c': 3}))
        self.assertEqual(True, redisConn.insertData(tableName, {'a': 2, 'b': 2, 'c': 3}))
        self.assertEqual(True, redisConn.insertData(tableName, {'a': 9, 'b': 2, 'c': 3}))
        self.assertEqual(True, redisConn.close())

        self.assertEqual(3, utils.countSortedSetMembers(tableName))



    def test_batch_insert(self):
        self.assertEqual(True, utils.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('General', 'batchsize', 2)

        redisConn = RedisDBConnectorBASIC(config_file_path)

        self.assertEqual(True, redisConn.connect())
        self.assertEqual(True, redisConn.insertData(tableName, {'a': 4, 'b': 2, 'c': 3}))
        self.assertEqual(0, utils.countSortedSetMembers(tableName))

        self.assertEqual(True, redisConn.insertData(tableName, {'a': 2, 'b': 2, 'c': 3}))
        self.assertEqual(2, utils.countSortedSetMembers(tableName))

        self.assertEqual(True, redisConn.close())


    def test_batch_insert_closing_connection_before_finish(self):
        self.assertEqual(True, utils.deleteKey(tableName))

        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('General', 'batchsize', 2)

        redisConn = RedisDBConnectorBASIC(config_file_path)

        self.assertEqual(True, redisConn.connect())
        self.assertEqual(True, redisConn.insertData(tableName, {'a': 4, 'b': 2, 'c': 3}))
        self.assertEqual(0, utils.countSortedSetMembers(tableName))

        self.assertEqual(True, redisConn.close())

        self.assertEqual(1, utils.countSortedSetMembers(tableName))



if __name__ == '__main__':

    unittest.main()
