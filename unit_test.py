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
import time
from random import randint, uniform

from PyRTAlib.Utils         import Config
from PyRTAlib.DBConnectors  import MySqlDBConnector, RedisDBConnectorBASIC
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB_old
from PyRTAlib.DTRInterface  import DTR





"""
            )       )   (      (
   (     ( /(    ( /(   )\ )   )\ )   (
   )\    )\())   )\()) (()/(  (()/(   )\ )
 (((_)  ((_)\   ((_)\   /(_))  /(_)) (()/(
 )\___    ((_)   _((_) (_))_| (_))    /(_))_
((/ __|  / _ \  | \| | | |_   |_ _|  (_)) __|
 | (__  | (_) | | .` | | __|   | |     | (_ |
  \___|  \___/  |_|\_| |_|    |___|     \___|

"""
class ConfigFile(unittest.TestCase):

    def test_no_path_no_env_var_provided(self):
        if 'RTACONFIGFILE' in os.environ:
            del os.environ['RTACONFIGFILE']
        config = Config('', False)
        self.assertRaises(Exception, config.parseConfigFile, '')

    def test_file_not_found_wrong_path(self):
        config = Config('', False)
        self.assertRaises(FileNotFoundError, config.parseConfigFile, 'akjdiajwnd')

    def test_file_not_found_wrong_env_var_path(self):
        os.environ['RTACONFIGFILE'] = './ajdoiwajdoiwd'
        config = Config('', False)
        self.assertRaises(FileNotFoundError, config.parseConfigFile, '')

    def test_file_found_with_relative_path(self):
        config = Config('./')
        self.assertEqual(True, bool(config.get()))

    def test_file_found_with_environment_variable(self):
        os.environ['RTACONFIGFILE'] = './'
        config = Config()
        self.assertEqual(True, bool(config.get()))

    def test_priority_to_env_var(self):
        os.environ['RTACONFIGFILE'] = './'
        config = Config('./pluto')
        self.assertEqual(True, bool(config.get()))

    def test_singleton(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config = Config('./')
        self.assertEqual('yes', config.get('General', 'debug'))
        config.reload('./')

    def test_get(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        self.assertEqual('yes', config.get('General', 'debug'))

    def test_get_cast(self):
        config = Config('./')
        config.set('General', 'batchsize', '1')
        self.assertEqual(1, config.get('General', 'batchsize', 'int'))










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
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('MySql', 'password', 'asdasd')
        mysqlConn = MySqlDBConnector('./')
        self.assertEqual(False, mysqlConn.connect())
        mysqlConn.close()
        config.reload('./')

    def test_connect_wrong_username(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('MySql', 'username', 'gioacchino')
        mysqlConn = MySqlDBConnector('./')
        self.assertEqual(False, mysqlConn.connect())
        mysqlConn.close()
        config.reload('./')

    def test_connect_wrong_database(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('MySql', 'dbname', 'evttttest')
        mysqlConn = MySqlDBConnector('./')
        self.assertEqual(False, mysqlConn.connect())
        mysqlConn.close()
        config.reload('./')

    def test_connect_success(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        mysqlConn = MySqlDBConnector('./')
        self.assertEqual(True, mysqlConn.connect())
        mysqlConn.close()
    ## ------------------------------------------------------------------------


    ## WRITING IN DB ----------------------------------------------------------
    def test_insert_data_wrong_table(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(False, mysqlConn.insertData('lest_fable', {'a': 1, 'b':2, 'c':3, 'd':4}))
        mysqlConn.close()
        config.reload('./')

    def test_insert_data_not_enough_data(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(False, mysqlConn.executeQuery('INSERT INTO test_table VALUES(1,2)'))
        mysqlConn.close()
        config.reload('./')

    def test_insert_data_succesfully(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()
        self.assertEqual(True, mysqlConn.insertData('test_table', {'a': 1, 'b':2, 'c':3, 'd':4}))
        mysqlConn.close()
        config.reload('./')


    def test_batch(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('General', 'batchsize', 2)
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()

        self.assertEqual(True,mysqlConn.executeQuery('delete from test_table'))

        self.assertEqual(True, mysqlConn.insertData('test_table', {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(1, mysqlConn.conn.in_transaction)
        self.assertEqual(1, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.insertData('test_table', {'a': 5, 'b':6, 'c':7, 'd':8}))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        # TODO FIX BUG.
        # self._cmysql.commit()
        # _mysql_connector.MySQLInterfaceError: Commands out of sync; you can't run this command now
        #self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM test_table'))
        #numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        #self.assertEqual(2, numberOfRows)

        mysqlConn.close()
        config.reload('./')


    def test_streaming(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()

        self.assertEqual(True,mysqlConn.executeQuery('delete from test_table'))

        self.assertEqual(True, mysqlConn.insertData('test_table', {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(1, numberOfRows)


        self.assertEqual(True, mysqlConn.insertData('test_table', {'a': 5, 'b':6, 'c':7, 'd':8}))
        self.assertEqual(0, mysqlConn.conn.in_transaction)
        self.assertEqual(0, mysqlConn.commandsSent)

        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(2, numberOfRows)

        mysqlConn.close()
        config.reload('./')

    def test_batch_connection_close_before_finish(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('General', 'batchsize', 2)
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()

        self.assertEqual(True,mysqlConn.executeQuery('delete from test_table'))

        self.assertEqual(True, mysqlConn.insertData('test_table', {'a': 1, 'b':2, 'c':3, 'd':4}))
        self.assertEqual(1, mysqlConn.conn.in_transaction)
        self.assertEqual(1, mysqlConn.commandsSent)

        mysqlConn.close()
        config.reload('./')


        config.set('General', 'debug', 'yes')
        config.set('General', 'batchsize', 1)
        mysqlConn = MySqlDBConnector('./')
        mysqlConn.connect()

        self.assertEqual(True, mysqlConn.executeQuery('SELECT COUNT(*) FROM test_table'))
        numberOfRows = int(mysqlConn.cursor.fetchone()[0])
        self.assertEqual(1, numberOfRows)

        mysqlConn.close()
        config.reload('./')
    ##--------------------------------------------------------------------------










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

    def test_testConnection_wrong_password_basic(self):
        config = Config('./')
        config.set('General', 'debug', 'yes')
        config.set('Redis', 'password', 'asdasd')
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()
        self.assertEqual(False, redisConn.testConnection())
        config.reload('./')


    def test_testConnection_success_basic(self):
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()
        self.assertEqual(True, redisConn.testConnection())

    def test_unknown_key_basic(self):
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()
        val = redisConn.conn.get('h243hihbj23b4j23hb4j2h3')
        self.assertEqual(None, val)

    def test_insert_data_basic(self):
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()
        dict1 = {'a': 4, 'b': 2, 'c': 3}
        dict2 = {'a': 2, 'b': 2, 'c': 3}
        dict3 = {'a': 9, 'b': 2, 'c': 3}

        self.assertEqual(True, redisConn.insertData('testmodel',dict1))
        self.assertEqual(True, redisConn.insertData('testmodel',dict2))
        self.assertEqual(True, redisConn.insertData('testmodel',dict3))

    def test_insert_model_doesnt_exist(self):
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()
        dict = {'a': 4, 'b': 2, 'c': 3}
        self.assertEqual(False, redisConn.insertData('idontexist',dict))

    def test_insert_duplicate_data(self):
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()
        dict1 = {'a': 5, 'b': 2, 'c': 3}
        dict2 = {'a': 5, 'b': 2, 'c': 3}
        self.assertEqual(True, redisConn.insertData('testmodel',dict1))
        self.assertEqual(True, redisConn.insertData('testmodel',dict2))















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
def deleteTable(config):
    mysqlConn = MySqlDBConnector('./')
    mysqlConn.connect()
    res = mysqlConn.executeQuery('delete from '+config.get('General', 'evt3modelname'))
    mysqlConn.close()
    return res

def getConfiguration(debug='yes', numberOfThreads=1, batchsize=1, dtr='no'):
    config = Config('./')
    config.set('General', 'debug', debug)
    config.set('General', 'numberofthreads', numberOfThreads)
    config.set('General', 'batchsize', batchsize)
    config.set('Dtr', 'active', dtr)
    return config

def checkNumberOfRows(config):
    config.set('General', 'batchsize', 1)
    mysqlConn = MySqlDBConnector('./')
    mysqlConn.connect()
    res = mysqlConn.executeQuery('SELECT COUNT(*) FROM '+config.get('General', 'evt3modelname'))
    resNumberOfRows = int(mysqlConn.cursor.fetchone()[0])
    mysqlConn.close()
    config.reload('./')
    return str(res)+'-'+str(resNumberOfRows)



class DL3ASTRIDB_interface(unittest.TestCase):

    os.environ['RTACONFIGFILE'] = './'

    def test_insert_mysql_multi_thread_streaming_wait_and_close(self):
        print("/\____/\____/\____/\____/\__multi__/\__streaming__/\__wc__/\____/\____/\____/\_")

        # Configuration
        config = getConfiguration(debug='yes', numberOfThreads=2, batchsize=1)

        # Delete old data
        self.assertEqual(True,deleteTable(config))

        # Connect and insert random event
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('mysql')
        RTA_DL3ASTRI.insertEvent( *RTA_DL3ASTRI.getRandomEvent() )


        time.sleep(1)

        stats = RTA_DL3ASTRI.waitAndClose()

        print('Stats: {}'.format(stats))
        self.assertEqual(1, stats[0])

        # Check number of rows
        self.assertEqual('True-1', checkNumberOfRows(config))






    def test_insert_mysql_multi_thread_batch_wait_and_close(self):
        print("/\____/\____/\____/\____/\__multi__/\__batch__/\__wc__/\____/\____/\____/\_")

        # Configuration
        config = getConfiguration(debug='yes', numberOfThreads=2, batchsize=2)

        # Delete old data
        self.assertEqual(True,deleteTable(config))

        # Connect and insert random event
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('mysql')
        RTA_DL3ASTRI.insertEvent( *RTA_DL3ASTRI.getRandomEvent() )

        time.sleep(1)

        stats = RTA_DL3ASTRI.waitAndClose()

        print('Stats: {}'.format(stats))
        self.assertEqual(1, stats[0])

        # Check number of rows
        self.assertEqual('True-1', checkNumberOfRows(config))


    def test_insert_mysql_multi_thread_streaming_force_close(self):
        print("/\____/\____/\____/\____/\__multi__/\__streaming__/\__fc__/\____/\____/\____/\_")

        # Configuration
        config = getConfiguration(debug='yes', numberOfThreads=2, batchsize=1)

        # Delete old data
        self.assertEqual(True,deleteTable(config))

        # Connect and insert random event
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('mysql')
        RTA_DL3ASTRI.insertEvent( *RTA_DL3ASTRI.getRandomEvent() )

        time.sleep(1)

        RTA_DL3ASTRI.forceClose() # <---------

        # Check number of rows
        self.assertEqual('True-1', checkNumberOfRows(config))



    def test_insert_mysql_multi_thread_batch_wait_and_close(self):
        print("/\____/\____/\____/\____/\__multi__/\__batch__/\__fc__/\____/\____/\____/\_")

        # Configuration
        config = getConfiguration(debug='yes', numberOfThreads=2, batchsize=2)

        # Delete old data
        self.assertEqual(True,deleteTable(config))

        # Connect and insert random event
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('mysql')
        RTA_DL3ASTRI.insertEvent( *RTA_DL3ASTRI.getRandomEvent() )

        time.sleep(1)

        RTA_DL3ASTRI.forceClose() # <---------


        # Check number of rows
        self.assertEqual('True-1', checkNumberOfRows(config))


    def test_insert_mysql_single_thread_streaming_force_close(self):
        print("/\____/\____/\____/\____/\__single__/\__streaming__/\__fc__/\____/\____/\____/\_")

        # Configuration
        config = getConfiguration(debug='yes', numberOfThreads=1, batchsize=1)

        # Delete old data
        self.assertEqual(True,deleteTable(config))

        # Connect and insert random event
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('mysql')
        RTA_DL3ASTRI.insertEvent( *RTA_DL3ASTRI.getRandomEvent() )

        time.sleep(1) # lets give the threads some time

        RTA_DL3ASTRI.forceClose()

        # Check number of rows
        self.assertEqual('True-1', checkNumberOfRows(config))


    def test_insert_mysql_single_thread_streaming_wait_and_close_pure_multithread(self):
        print("/\____/\____/\____/\____/\__single__/\__streaming__/\__wc__/\__pure_multit__/\____/\____/\_")

        # Configuration
        config = getConfiguration(debug='yes', numberOfThreads=1, batchsize=1)

        # Delete old data
        self.assertEqual(True,deleteTable(config))

        # Connect and insert random event
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('mysql', pure_multithreading=True)
        RTA_DL3ASTRI.insertEvent( *RTA_DL3ASTRI.getRandomEvent() )


        time.sleep(1) # lets give the threads some time

        stats = RTA_DL3ASTRI.waitAndClose()

        print('Stats: {}'.format(stats))
        self.assertEqual(1, stats[0])

        # Check number of rows
        self.assertEqual('True-1', checkNumberOfRows(config))


    """
        ------------------------> REDIS
    """
    def test_insert_redis_single_thread_streaming_force_close(self):

        # Configuration
        config = getConfiguration(debug='yes', numberOfThreads=1, batchsize=1)

        # Delete old data
        redisConn = RedisDBConnectorBASIC('./')
        redisConn.connect()
        redisConn.conn.delete(config.get('General', 'evt3modelname'))


        # Connect and insert random event
        RTA_DL3ASTRI = RTA_DL3ASTRI_DB_old('redis-basic')
        RTA_DL3ASTRI.insertEvent( *RTA_DL3ASTRI.getRandomEvent() )

        time.sleep(1) # lets give the threads some time

        RTA_DL3ASTRI.forceClose()

        self.assertEqual(1, redisConn.conn.zcard(config.get('General', 'evt3modelname')) )
        config.reload('./')




if __name__ == '__main__':
    unittest.main()
