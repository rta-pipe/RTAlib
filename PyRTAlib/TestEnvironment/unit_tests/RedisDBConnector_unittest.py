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

from PyRTAlib.Utils         import Config
from PyRTAlib.DBConnectors  import RedisDBConnectorBASIC



DEBUG = False
config_file_path = '../'

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
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        redisConn = RedisDBConnectorBASIC(config_file_path)
        redisConn.connect()
        self.assertEqual(True, redisConn.testConnection())
        config.reload(config_file_path)

    def test_testConnection_wrong_password_basic(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('Redis', 'password', 'asdasd')
        redisConn = RedisDBConnectorBASIC(config_file_path)
        redisConn.connect()
        self.assertEqual(False, redisConn.testConnection())
        config.reload(config_file_path)


    def test_testConnection_success_basic(self):
        redisConn = RedisDBConnectorBASIC(config_file_path)
        redisConn.connect()
        self.assertEqual(True, redisConn.testConnection())


    def test_unknown_key_basic(self):
        redisConn = RedisDBConnectorBASIC(config_file_path)
        redisConn.connect()
        val = redisConn.conn.get('h243hihbj23b4j23hb4j2h3')
        self.assertEqual(None, val)

    def test_insert_data_basic(self):
        redisConn = RedisDBConnectorBASIC(config_file_path)
        redisConn.connect()
        dict1 = {'a': 4, 'b': 2, 'c': 3}
        dict2 = {'a': 2, 'b': 2, 'c': 3}
        dict3 = {'a': 9, 'b': 2, 'c': 3}

        self.assertEqual(True, redisConn.insertData('rtalib_test_table',dict1))
        self.assertEqual(True, redisConn.insertData('rtalib_test_table',dict2))
        self.assertEqual(True, redisConn.insertData('rtalib_test_table',dict3))

    def test_insert_model_doesnt_exist(self):
        redisConn = RedisDBConnectorBASIC(config_file_path)
        redisConn.connect()
        dict = {'a': 4, 'b': 2, 'c': 3}
        self.assertEqual(False, redisConn.insertData('idontexist',dict))

    def test_insert_duplicate_data(self):
        redisConn = RedisDBConnectorBASIC(config_file_path)
        redisConn.connect()
        dict1 = {'a': 5, 'b': 2, 'c': 3}
        dict2 = {'a': 5, 'b': 2, 'c': 3}
        self.assertEqual(True, redisConn.insertData('rtalib_test_table',dict1))
        self.assertEqual(True, redisConn.insertData('rtalib_test_table',dict2))

if __name__ == '__main__':

    unittest.main()
