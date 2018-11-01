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
from os import environ

rootFolder = dirname(dirname(dirname(dirname(realpath(__file__)))))
path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.Utils         import Config
from PyRTAlib.DBConnectors  import MySqlDBConnector
from UtilsUT import UtilsMySql
from UtilsUT import getConfig


if 'RTALIBCONFIG' not in environ:
    print('\n[MySqlDBConnector__unittest.py] No RTALIBCONFIG env var found. Please export it and let it points to the rtalibconfig_testing configuration file.\n')
    exit(1)
config_file_path = environ['RTALIBCONFIG']

DEBUG = False
utils = UtilsMySql(config_file_path)
tableName = 'rtalib_test_table'


"""
   *          )   (              (                   )       )      )                             )    (
 (  `      ( /(   )\ )     (     )\ )       (     ( /(    ( /(   ( /(           (      *   )   ( /(    )\ )
 )\))(     )\()) (()/(   ( )\   (()/(       )\    )\())   )\())  )\())  (       )\   ` )  /(   )\())  (()/(
((_)()\   ((_)\   /(_))  )((_)   /(_))    (((_)  ((_)\   ((_)\  ((_)\   )\    (((_)   ( )(_)) ((_)\    /(_))
(_()((_) __ ((_) (_))   ((_)_   (_))      )\___    ((_)   _((_)  _((_) ((_)   )\___  (_(_())    ((_)  (_))
|  \/  | \ \ / / / __|   / _ \  | |      ((/ __|  / _ \  | \| | | \| | | __| ((/ __| |_   _|   / _ \  | _ \
| |\/| |  \ V /  \__ \  | (_) | | |__     | (__  | (_) | | .` | | .` | | _|   | (__    | |    | (_) | |   /
|_|  |_|   |_|   |___/   \__\_\ |____|     \___|  \___/  |_|\_| |_|\_| |___|   \___|   |_|     \___/  |_|_\
"""

class MySqlConnector(unittest.TestCase):


    def test_connection_with_wrong_password(self):
        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('MySql', 'password', 'asdasd')

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(False, mysqlConn.connect())


    def test_connection_with_wrong_username(self):
        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('MySql', 'password', 'asdasd')
        getConfig(config_file_path, DEBUG).set('MySql', 'username', 'gioacchino')

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(False, mysqlConn.connect())


    def test_connection_with_wrong_database(self):
        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('MySql', 'dbname', 'evttttest')

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(False, mysqlConn.connect())


    def test_connectionion_success(self):
        getConfig(config_file_path, DEBUG, reload=True)

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(True, mysqlConn.connect())
        self.assertEqual(True, mysqlConn.testConnection())
        self.assertEqual(True, mysqlConn.close())


    def test_close_not_opened_connection(self):
        getConfig(config_file_path, DEBUG, reload=True)
        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(False, mysqlConn.testConnection())
        self.assertEqual(False, mysqlConn.close())


    def test_insert_data_wrong_table(self):
        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('General', 'batchsize', 1)

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(True, mysqlConn.connect())
        self.assertEqual(False, mysqlConn.insertData('lest_fable', {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(True, mysqlConn.close())


    def test_negative_batchsize(self):
        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('General', 'batchsize', -1)

        mysqlConn = MySqlDBConnector(config_file_path)

        self.assertEqual(True, mysqlConn.connect())
        self.assertEqual(False, mysqlConn.insertData(tableName, {'a': 4, 'b': 2, 'c': 3}))
        self.assertEqual(True, mysqlConn.close())


    def test_insert_data_not_enough_data(self):
        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('General', 'batchsize', 1)

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(True, mysqlConn.connect())
        self.assertEqual(False, mysqlConn.executeQuery('INSERT INTO '+tableName+' VALUES(1,2)'))
        self.assertEqual(True, mysqlConn.close())

    def test_execute_query_without_connection(self):
        getConfig(config_file_path, DEBUG, reload=True)

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(False, mysqlConn.executeQuery('INSERT INTO '+tableName+' VALUES(1,2,3,4)'))


    def test_streaming_insert(self):
        self.assertEqual(True, utils.truncateTable(tableName))

        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('General', 'batchsize', 1)

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(True, mysqlConn.connect())

        self.assertEqual(MySqlDBConnector.SUCCESS_AND_COMMIT, mysqlConn.insertData(tableName, {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(1, utils.countRowsInTable(tableName))

        self.assertEqual(MySqlDBConnector.SUCCESS_AND_COMMIT, mysqlConn.insertData(tableName, {'a': 5, 'b':6, 'c':7, 'd':8}))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(2, utils.countRowsInTable(tableName))

        self.assertEqual(True, mysqlConn.close())


    def test_batch_insert(self):
        self.assertEqual(True, utils.truncateTable(tableName))

        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('General', 'batchsize', 2)

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(True, mysqlConn.connect())

        self.assertEqual(MySqlDBConnector.SUCCESS, mysqlConn.insertData(tableName, {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(1, mysqlConn.conn.in_transaction)
        self.assertEqual(1, mysqlConn.commandsSent)

        self.assertEqual(0, utils.countRowsInTable(tableName))

        self.assertEqual(MySqlDBConnector.SUCCESS_AND_COMMIT, mysqlConn.insertData(tableName, {'a': 5, 'b':6, 'c':7, 'd':8}))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(2, utils.countRowsInTable(tableName))

        self.assertEqual(True, mysqlConn.close())


    def test_batch_closing_connection_before_finish(self):
        self.assertEqual(True, utils.truncateTable(tableName))

        getConfig(config_file_path, DEBUG, reload=True)
        getConfig(config_file_path, DEBUG).set('General', 'batchsize', 2)

        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(True, mysqlConn.connect())

        self.assertEqual(MySqlDBConnector.SUCCESS, mysqlConn.insertData(tableName, {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(1, mysqlConn.conn.in_transaction)
        self.assertEqual(1, mysqlConn.commandsSent)

        self.assertEqual(0, utils.countRowsInTable(tableName))

        self.assertEqual(True, mysqlConn.close())  # <--- committing on closing..

        self.assertEqual(1, utils.countRowsInTable(tableName))




if __name__ == '__main__':

    unittest.main()
