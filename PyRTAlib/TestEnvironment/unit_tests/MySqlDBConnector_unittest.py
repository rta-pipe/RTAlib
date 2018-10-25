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
from PyRTAlib.DBConnectors  import MySqlDBConnector


DEBUG = False
config_file_path = '../'


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

    ## CONNECTION --------------------------------------------------------------
    def test_connect_wrong_password(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('MySql', 'password', 'asdasd')
        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(False, mysqlConn.connect())
        self.assertEqual(False, mysqlConn.close())
        config.reload(config_file_path)

    def test_connect_wrong_username(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('MySql', 'username', 'gioacchino')
        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(False, mysqlConn.connect())
        self.assertEqual(False, mysqlConn.close())
        config.reload(config_file_path)

    def test_connect_wrong_database(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('MySql', 'dbname', 'evttttest')
        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(False, mysqlConn.connect())
        self.assertEqual(False, mysqlConn.close())
        config.reload(config_file_path)

    def test_connect_success(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        mysqlConn = MySqlDBConnector(config_file_path)
        self.assertEqual(True, mysqlConn.connect())
        self.assertEqual(True, mysqlConn.close())
        config.reload(config_file_path)
    ## ------------------------------------------------------------------------


    ## WRITING IN DB ----------------------------------------------------------
    def test_insert_data_wrong_table(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector(config_file_path)
        mysqlConn.connect()
        self.assertEqual(False, mysqlConn.insertData('lest_fable', {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(True, mysqlConn.close())
        config.reload(config_file_path)

    def test_insert_data_not_enough_data(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector(config_file_path)
        mysqlConn.connect()
        self.assertEqual(False, mysqlConn.executeQuery('INSERT INTO rtalib_test_table VALUES(1,2)'))
        self.assertEqual(True, mysqlConn.close())
        config.reload(config_file_path)

    def test_insert_data_succesfully(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector(config_file_path)
        mysqlConn.connect()
        self.assertEqual(MySqlDBConnector.SUCCESS_AND_COMMIT, mysqlConn.insertData('rtalib_test_table', {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(True, mysqlConn.close())
        config.reload(config_file_path)


    def test_streaming(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector(config_file_path)
        mysqlConn.connect()

        self.assertEqual(True,mysqlConn.executeQuery('delete from rtalib_test_table'))

        self.assertEqual(MySqlDBConnector.SUCCESS_AND_COMMIT, mysqlConn.insertData('rtalib_test_table', {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM rtalib_test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(1, numberOfRows)


        self.assertEqual(MySqlDBConnector.SUCCESS_AND_COMMIT, mysqlConn.insertData('rtalib_test_table', {'a': 5, 'b':6, 'c':7, 'd':8}))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM rtalib_test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(2, numberOfRows)

        mysqlConn.close()
        config.reload(config_file_path)


    def test_batch(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('General', 'batchsize', 2)
        mysqlConn = MySqlDBConnector(config_file_path)
        mysqlConn.connect()

        self.assertEqual(True, mysqlConn.executeQuery('delete from rtalib_test_table'))

        self.assertEqual(MySqlDBConnector.SUCCESS, mysqlConn.insertData('rtalib_test_table', {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(1, mysqlConn.conn.in_transaction)
        self.assertEqual(1, mysqlConn.commandsSent)

        self.assertEqual(MySqlDBConnector.SUCCESS_AND_COMMIT, mysqlConn.insertData('rtalib_test_table', {'a': 5, 'b':6, 'c':7, 'd':8}))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.close())
        config.reload(config_file_path)


        config.set('General', 'debug', DEBUG)
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector(config_file_path)
        mysqlConn.connect()
        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM rtalib_test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(2, numberOfRows)
        config.reload(config_file_path)

    def test_batch_connection_close_before_finish(self):
        config = Config(config_file_path)
        config.set('General', 'debug', DEBUG)
        config.set('General', 'batchsize', 2)
        mysqlConn = MySqlDBConnector(config_file_path)
        mysqlConn.connect()

        self.assertEqual(True,mysqlConn.executeQuery('delete from rtalib_test_table'))

        self.assertEqual(MySqlDBConnector.SUCCESS, mysqlConn.insertData('rtalib_test_table', {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(1, mysqlConn.conn.in_transaction)
        self.assertEqual(1, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.close())  # <--- committing on closing..

        config.reload(config_file_path)

        config.set('General', 'debug', DEBUG)
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector(config_file_path)
        mysqlConn.connect()

        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM rtalib_test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(1, numberOfRows)

        self.assertEqual(True, mysqlConn.close())
        config.reload(config_file_path)
    ##--------------------------------------------------------------------------

if __name__ == '__main__':

    unittest.main()
