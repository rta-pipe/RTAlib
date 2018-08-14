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
from random import randint, uniform

from PyLibRTA.Utils         import parseRTALIBConfigFile
from PyLibRTA.DBConnectors  import RedisDBConnector, MySqlDBConnector
from PyLibRTA.RTAInterface  import RTA_DL_DB

class FileParser(unittest.TestCase):

    def test_file_not_found_wrong_path(self):
        self.assertEqual(False, parseRTALIBConfigFile('akjdiajwnd'))

    def test_file_not_found_wrong_env_var_name(self):
        del os.environ['RTACONFIGFILE']
        os.environ['XXXRTACONGFILEXXX'] = './'
        self.assertEqual(False, parseRTALIBConfigFile())

    def test_file_not_found_wrong_env_var_path(self):
        os.environ['RTACONFIGFILE'] = './ajdoiwajdoiwd'
        self.assertEqual(False, parseRTALIBConfigFile())

    def test_file_found_with_relative_path(self):
        self.assertEqual(True, bool(parseRTALIBConfigFile('./')))

    def test_file_found_with_environment_variable(self):
        os.environ['RTACONFIGFILE'] = './'
        self.assertEqual(True, bool(parseRTALIBConfigFile()))



class MySqlConnector(unittest.TestCase):

    def test_connect_wrong_password(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.password = 'asdasd'
        self.assertEqual(False, mysqlConn.connect())

    def test_connect_wrong_username(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.username = 'gioacchino'
        self.assertEqual(False, mysqlConn.connect())

    def test_connect_wrong_database(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.dbname = 'evttttest'
        self.assertEqual(False, mysqlConn.connect())

    def test_connect_success(self):
        mysqlConn = MySqlDBConnector('./')
        self.assertEqual(True, mysqlConn.connect())

    def test_insert_data_wrong_table(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(False, mysqlConn.insertData('lest_fable',a=1,b=2,c=3))

    def test_insert_data(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(True, mysqlConn.insertData('test_table',a=1,b=2,c=3))


class RedisConnector(unittest.TestCase):

    def test_testConnection_wrong_password(self):
        redisConn = RedisDBConnector('./')
        redisConn.password = 'asdasd'
        redisConn.connect()
        self.assertEqual(False, redisConn.testConnection())

    def test_testConnection_success(self):
        redisConn = RedisDBConnector('./')
        redisConn.connect()
        self.assertEqual(True, redisConn.testConnection())


class DLDBInterface(unittest.TestCase):

    os.environ['RTACONFIGFILE'] = './'

    def test_connection_mysql(self):
        RTADLDB = RTA_DL_DB('mysql')
        self.assertEqual(True, RTADLDB.isConnectionAlive())

    def test_connection_redis(self):
        RTADLDB = RTA_DL_DB('redis')
        self.assertEqual(True, RTADLDB.isConnectionAlive())

    def test_insert_mysql(self):
        RTADLDB = RTA_DL_DB('mysql')
        res = RTADLDB.insertEvent(  evtid=randint(0, 9999999),
                                    eventidfits=randint(0, 9999999),
                                    observationid=randint(0, 9999999),
                                    datarepositoryid=randint(0, 9999999),
                                    ra_deg=uniform(-180,180),
                                    dec_deg=uniform(-90, 90),
                                    energy=uniform(0, 0.5),
                                    detx=uniform(0, 0.1),
                                    dety=uniform(0, 0.1),
                                    mcid=1,
                                    status=0,
                                    timerealtt=randint(0, 99999999),
                                    insert_time=randint(0, 99999999)
                                )
        self.assertEqual(True, RTADLDB.isConnectionAlive())




    def test_insert_redis(self):
        RTADLDB = RTA_DL_DB('redis')
        res = RTADLDB.insertEvent(  evtid=randint(0, 9999999),
                                    eventidfits=randint(0, 9999999),
                                    observationid=randint(0, 9999999),
                                    datarepositoryid=randint(0, 9999999),
                                    ra_deg=uniform(-180,180),
                                    dec_deg=uniform(-90, 90),
                                    energy=uniform(0, 0.5),
                                    detx=uniform(0, 0.1),
                                    dety=uniform(0, 0.1),
                                	mcid=1,
                                    status=0,
                                    timerealtt=randint(0, 99999999),
                                    insert_time=randint(0, 99999999)
                                )



if __name__ == '__main__':
    unittest.main()
