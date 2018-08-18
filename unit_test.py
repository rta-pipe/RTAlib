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

from PyRTAlib.Utils         import parseRTALIBConfigFile
from PyRTAlib.DBConnectors  import RedisDBConnector, MySqlDBConnector
from PyRTAlib.DataModels    import EVT3_ASTRI
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB

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
        mysqlConn.close()

    def test_connect_wrong_username(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.username = 'gioacchino'
        self.assertEqual(False, mysqlConn.connect())
        mysqlConn.close()

    def test_connect_wrong_database(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.dbname = 'evttttest'
        self.assertEqual(False, mysqlConn.connect())
        mysqlConn.close()

    def test_insert_data_wrong_table(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(False, mysqlConn.insertData('INSERT INTO lest_fable VALUES(1,2,3,4)'))
        mysqlConn.close()

    def test_insert_data_not_enough_data(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(False, mysqlConn.insertData('INSERT INTO test_table VALUES(1,2)'))
        mysqlConn.close()

    def test_connect_success(self):
        mysqlConn = MySqlDBConnector('./')
        self.assertEqual(True, mysqlConn.connect())
        mysqlConn.close()

    def test_insert_data_normal_use(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(True, mysqlConn.insertData('INSERT INTO test_table VALUES(1,2,3,4)'))
        mysqlConn.close()

    ##############################
    # TODO table doesnt exist
    ##############################

    def test_batch(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        mysqlConn.batchsize = 2

        self.assertEqual(True,mysqlConn.executeQuery('delete from test_table'))

        self.assertEqual(True, mysqlConn.insertData('INSERT INTO test_table VALUES(1,2,3,4)'))
        self.assertEqual(1, mysqlConn.conn.in_transaction)
        self.assertEqual(1, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.insertData('INSERT INTO test_table VALUES(5,6,7,8)'))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(2, numberOfRows)

        mysqlConn.close()

    def test_streaming(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        mysqlConn.batchsize = 1

        self.assertEqual(True,mysqlConn.executeQuery('delete from test_table'))

        self.assertEqual(True, mysqlConn.insertData('INSERT INTO test_table VALUES(1,2,3,4)'))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(1, numberOfRows)


        self.assertEqual(True, mysqlConn.insertData('INSERT INTO test_table VALUES(5,6,7,8)'))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(2, numberOfRows)

        mysqlConn.close()



    



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




class DL3ASTRIDB_interface(unittest.TestCase):

    os.environ['RTACONFIGFILE'] = './'

    def test_connection_mysql(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        self.assertEqual(True, RTA_DL3ASTRI.isConnectionAlive())
        RTA_DL3ASTRI.close()


    def test_insert_mysql(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = 1
        res = RTA_DL3ASTRI.insertEvent(
                                            randint(0, 9999999), #eventidfits=randint(0, 9999999),
                                            randint(0, 9999999), #time=randint(0, 9999999),
                                            uniform(-180,180),   #ra_deg=uniform(-180,180),
                                            uniform(-90, 90),    #dec_deg=uniform(-90, 90),
                                            uniform(0, 0.5),     #energy=uniform(0, 0.5),
                                            uniform(0, 0.1),     #detx=uniform(0, 0.1),
                                            uniform(0, 0.1),     #dety=uniform(0, 0.1),
                                            1                    #mcid=1
                                        )
        self.assertEqual(True, res)

        RTA_DL3ASTRI.close()

    """
    def test_insert_mysql(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = 1
        res = RTA_DL3ASTRI.insertEvent( randint(0, 9999999), #eventidfits=randint(0, 9999999),
                                        randint(0, 9999999), #time=randint(0, 9999999),
                                        uniform(-180,180),   #ra_deg=uniform(-180,180),
                                        uniform(-90, 90),    #dec_deg=uniform(-90, 90),
                                        uniform(0, 0.5),     #energy=uniform(0, 0.5),
                                        uniform(0, 0.1),     #detx=uniform(0, 0.1),
                                        uniform(0, 0.1),     #dety=uniform(0, 0.1),
                                        1                    #mcid=1
                                      )
        self.assertEqual(True, res)

        RTA_DL3ASTRI.close()

    def test_insert_3_mysql(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = 1
        res = RTA_DL3ASTRI.insertEvent_3(   randint(0, 9999999), #eventidfits=randint(0, 9999999),
                                            randint(0, 9999999), #time=randint(0, 9999999),
                                            uniform(-180,180),   #ra_deg=uniform(-180,180),
                                            uniform(-90, 90),    #dec_deg=uniform(-90, 90),
                                            uniform(0, 0.5),     #energy=uniform(0, 0.5),
                                            uniform(0, 0.1),     #detx=uniform(0, 0.1),
                                            uniform(0, 0.1),     #dety=uniform(0, 0.1),
                                            1                    #mcid=1
                                      )
        self.assertEqual(True, res)

        RTA_DL3ASTRI.close()


    """

    def test_insert_mysql_package_version(self):
        with RTA_DL3ASTRI_DB('mysql') as RTA_DL3ASTRI:
            res = RTA_DL3ASTRI.insertEvent( randint(0, 9999999), #eventidfits=randint(0, 9999999),
                                            randint(0, 9999999), #time=randint(0, 9999999),
                                            uniform(-180,180),   #ra_deg=uniform(-180,180),
                                            uniform(-90, 90),    #dec_deg=uniform(-90, 90),
                                            uniform(0, 0.5),     #energy=uniform(0, 0.5),
                                            uniform(0, 0.1),     #detx=uniform(0, 0.1),
                                            uniform(0, 0.1),     #dety=uniform(0, 0.1),
                                            1                    #mcid=1
                                          )
            self.assertEqual(True, res)
            # close() is called automagically :)


    """
    def test_connection_redis(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('redis')
        self.assertEqual(True, RTA_DL3ASTRI.isConnectionAlive())


    def test_insert_redis(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('redis')
        res = RTA_DL3ASTRI.insertEvent( randint(0, 9999999),
                                        randint(0, 9999999),
                                        randint(0, 9999999),
                                        randint(0, 9999999),
                                        uniform(-180,180),
                                        uniform(-90, 90),
                                        uniform(0, 0.5),
                                        uniform(0, 0.1),
                                        uniform(0, 0.1),
                                    	1,
                                        0,
                                        randint(0, 99999999),
                                        randint(0, 99999999)
                                    )
        self.assertEqual(True, res)
    """


if __name__ == '__main__':
    unittest.main()
