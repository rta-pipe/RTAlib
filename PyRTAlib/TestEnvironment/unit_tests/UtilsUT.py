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

from sys import path
from os.path import dirname, abspath, realpath

rootFolder = dirname(dirname(dirname(dirname(realpath(__file__)))))
path.append(rootFolder+'/PyRTAlib/')

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

    def getRedisChannelListener(self, channelName):
        if self.redisConnector.testConnection():
            pubsub = self.redisConnector.conn.pubsub()
            pubsub.subscribe(channelName)
            return pubsub
        else:
            return False


    """
    async def publish_to_channel(self, channelName, message):
        await asyncio.sleep(1)
        print("[UtilsUT] Publishing message on channel {}".format(channelName))
        self.redisConnector.conn.publish(channelName, message)

    def publishOnRedisChannel(self, channelName, message):
        if self.redisConnector.testConnection():
            loop = asyncio.get_event_loop()
            task = loop.create_task(self.publish_to_channel(channelName, message))
            loop.run_until_complete(task)
            return True
        else:
            return False
    """



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
