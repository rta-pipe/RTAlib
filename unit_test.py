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
        self.assertEqual(False, mysqlConn.insertData('lest_fable',1,2,3))
        mysqlConn.close()

    def test_insert_data_not_enough_data(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(False, mysqlConn.insertData('test_table',1,2))
        mysqlConn.close()

    def test_connect_success(self):
        mysqlConn = MySqlDBConnector('./')
        self.assertEqual(True, mysqlConn.connect())
        mysqlConn.close()

    def test_insert_data_normal_use(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(True, mysqlConn.insertData('test_table',5,6,7,8))
        mysqlConn.close()

    def test_transaction(self):
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()

        mysqlConn.batchsize = 3
        self.assertEqual(True,mysqlConn.executeQuery('delete from test_table'))

        self.assertEqual(True, mysqlConn.insertData('test_table',1,2,3,4))
        self.assertEqual(1, mysqlConn.conn.in_transaction)
        self.assertEqual(1, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.insertData('test_table',5,6,7,8))
        self.assertEqual(1, mysqlConn.conn.in_transaction)
        self.assertEqual(2, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.insertData('test_table',9,10,11,12))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)







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


class EVT3ASTRI_datamodel(unittest.TestCase):

    def test_get_query(self):
        evt3 = EVT3_ASTRI(1,2,3,4,5,6,7,8,9,10,11,12,13)
        query = 'INSERT INTO test_evt3(evtid, eventidfits, observationid, datarepositoryid, ra_deg, dec_deg, energy, detx, dety, mcid, status, timerealtt, insert_time) VALUES(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13)'
        evt3query = evt3.getInsertQuery('test_evt3')
        self.assertEqual(len(query), len(evt3query))
        queryHalf = int(len(query)/2)
        self.assertEqual(query[:queryHalf], evt3query[:queryHalf])
        self.assertEqual(query[queryHalf:], evt3query[queryHalf:])




class DL3ASTRIDB_interface(unittest.TestCase):

    os.environ['RTACONFIGFILE'] = './'

    def test_connection_mysql(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        self.assertEqual(True, RTA_DL3ASTRI.isConnectionAlive())
        RTA_DL3ASTRI.close()


    def test_insert_1_mysql(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = 1
        res = RTA_DL3ASTRI.insertEvent_1( randint(0, 9999999), #evtid=randint(0, 9999999),
                                        randint(0, 9999999), #eventidfits=randint(0, 9999999),
                                        randint(0, 9999999), #observationid=randint(0, 9999999),
                                        randint(0, 9999999), #datarepositoryid=randint(0, 9999999),
                                        uniform(-180,180),   #ra_deg=uniform(-180,180),
                                        uniform(-90, 90),    #dec_deg=uniform(-90, 90),
                                        uniform(0, 0.5),     #energy=uniform(0, 0.5),
                                        uniform(0, 0.1),     #detx=uniform(0, 0.1),
                                        uniform(0, 0.1),     #dety=uniform(0, 0.1),
                                    	1,                   #mcid=1,
                                        0,                   #status=0,
                                        randint(0, 99999999),#timerealtt=randint(0, 99999999),
                                        randint(0, 99999999) #insert_time=randint(0, 99999999)
                                      )
        self.assertEqual(True, res)

        RTA_DL3ASTRI.close()

    def test_insert_mysql(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = 1
        res = RTA_DL3ASTRI.insertEvent( randint(0, 9999999), #evtid=randint(0, 9999999),
                                        randint(0, 9999999), #eventidfits=randint(0, 9999999),
                                        randint(0, 9999999), #observationid=randint(0, 9999999),
                                        randint(0, 9999999), #datarepositoryid=randint(0, 9999999),
                                        uniform(-180,180),   #ra_deg=uniform(-180,180),
                                        uniform(-90, 90),    #dec_deg=uniform(-90, 90),
                                        uniform(0, 0.5),     #energy=uniform(0, 0.5),
                                        uniform(0, 0.1),     #detx=uniform(0, 0.1),
                                        uniform(0, 0.1),     #dety=uniform(0, 0.1),
                                    	2,                   #mcid=1,
                                        0,                   #status=0,
                                        randint(0, 99999999),#timerealtt=randint(0, 99999999),
                                        randint(0, 99999999) #insert_time=randint(0, 99999999)
                                      )
        self.assertEqual(True, res)

        RTA_DL3ASTRI.close()

    def test_insert_3_mysql(self):
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
        RTA_DL3ASTRI.dbConnector.batchsize = 1
        res = RTA_DL3ASTRI.insertEvent_3(   evtid=randint(0, 9999999),
                                            eventidfits=randint(0, 9999999),
                                            observationid=randint(0, 9999999),
                                            datarepositoryid=randint(0, 9999999),
                                            ra_deg=uniform(-180,180),
                                            dec_deg=uniform(-90, 90),
                                            energy=uniform(0, 0.5),
                                            detx=uniform(0, 0.1),
                                            dety=uniform(0, 0.1),
                                        	mcid=3,
                                            status=0,
                                            timerealtt=randint(0, 99999999),
                                            insert_time=randint(0, 99999999)
                                      )
        self.assertEqual(True, res)

        RTA_DL3ASTRI.close()



    def test_insert_mysql_package_version(self):
        with RTA_DL3ASTRI_DB('mysql') as RTA_DL3ASTRI:
            res = RTA_DL3ASTRI.insertEvent( randint(0, 9999999), #evtid=randint(0, 9999999),
                                            randint(0, 9999999), #eventidfits=randint(0, 9999999),
                                            randint(0, 9999999), #observationid=randint(0, 9999999),
                                            randint(0, 9999999), #datarepositoryid=randint(0, 9999999),
                                            uniform(-180,180),   #ra_deg=uniform(-180,180),
                                            uniform(-90, 90),    #dec_deg=uniform(-90, 90),
                                            uniform(0, 0.5),     #energy=uniform(0, 0.5),
                                            uniform(0, 0.1),     #detx=uniform(0, 0.1),
                                            uniform(0, 0.1),     #dety=uniform(0, 0.1),
                                        	4,                   #mcid=1,
                                            0,                   #status=0,
                                            randint(0, 99999999),#timerealtt=randint(0, 99999999),
                                            randint(0, 99999999) #insert_time=randint(0, 99999999)
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
