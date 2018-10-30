import sys
import os
from os.path import dirname, abspath, join

rootFolder = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))
sys.path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.Utils         import Config
from PyRTAlib.DBConnectors  import RedisDBConnectorBASIC
from PyRTAlib.DBConnectors  import MySqlDBConnector

def getConfig(config_file_path, debug, reload = False):
    config = Config(config_file_path)
    if reload:
        config.reload(config_file_path)
    config.set('General', 'debug', debug)
    return config

class UtilsRedis:

    def __init__(self, config_file_path):
        self.redisConnector = RedisDBConnectorBASIC(config_file_path)
        self.redisConnector.connect()

    def deleteKey(self, key):
        if self.redisConnector.testConnection():
            self.redisConnector.conn.delete(key)
            return True
        else:
            return False

    def countSortedSetMembers(self, key):
        if self.redisConnector.testConnection():
            return self.redisConnector.conn.zcard(key)
        else:
            return False

class UtilsMySql:

    def __init__(self, config_file_path):
        self.mySqlConnector = MySqlDBConnector(config_file_path)
        self.mySqlConnector.connect()

    def truncateTable(self, tableName):
        if self.mySqlConnector.testConnection():
            self.mySqlConnector.executeQuery('delete from '+tableName)
            return True
        else:
            return False

    def countRowsInTable(self, tableName):
        if self.mySqlConnector.testConnection():
            self.mySqlConnector.executeQuery('SELECT COUNT(*) FROM '+tableName)
            return int(self.mySqlConnector.cursor.fetchone()[0])
        else:
            return False
