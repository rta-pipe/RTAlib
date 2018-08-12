import unittest
from PyLibRTA.DBConnectors import RedisDBConnector, MySqlDBConnector

class RedisConnector(unittest.TestCase):

    def test_testConnection_wrong_password(self):
        redisConn = RedisDBConnector('127.0.0.1', '', 'askdjajwhequ3eiu3en')
        redisConn.connect(0)
        self.assertEqual(False, redisConn.testConnection())

    def test_testConnection_success(self):
        redisConn = RedisDBConnector('127.0.0.1', '', 'Redis@RTA18#')
        redisConn.connect(0)
        self.assertEqual(True, redisConn.testConnection())




class MySqlConnector(unittest.TestCase):

    def test_connect_wrong_password(self):
        mysqlConn = MySqlDBConnector('127.0.0.1', 'zollino', 'awd213123018#')
        self.assertEqual(False, mysqlConn.connect('evt_test'))

    def test_connect_wrong_username(self):
        mysqlConn = MySqlDBConnector('127.0.0.1', 'gioacchino', 'Giancarlo@2018#')
        self.assertEqual(False, mysqlConn.connect('evt_test'))

    def test_connect_wrong_database(self):
        mysqlConn = MySqlDBConnector('127.0.0.1', 'zollino', 'Giancarlo@2018#')
        self.assertEqual(False, mysqlConn.connect('evttttest'))

    def test_connect_success(self):
        mysqlConn = MySqlDBConnector('127.0.0.1', 'zollino', 'Giancarlo@2018#')
        self.assertEqual(True, mysqlConn.connect('evt_test'))


    def test_insert_data_wrong_table(self):
        mysqlConn = MySqlDBConnector('127.0.0.1', 'zollino', 'Giancarlo@2018#')
        mysqlConn.connect('evt_test')
        self.assertEqual(False, mysqlConn.insertData('lest_fable',a=1,b=2,c=3))

    def test_insert_data_wrong_table(self):
        mysqlConn = MySqlDBConnector('127.0.0.1', 'zollino', 'Giancarlo@2018#')
        mysqlConn.connect('evt_test')
        self.assertEqual(True, mysqlConn.insertData('test_table',a=1,b=2,c=3))


if __name__ == '__main__':
    unittest.main()
