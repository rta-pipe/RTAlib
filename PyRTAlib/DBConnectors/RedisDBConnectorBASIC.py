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

from .DBConnector import DBConnector
import redis

"""
Redis wrapper that exposes a connect/disconnet/insert API.

RedisModel:
    - uniqueId:<modelName> (should be exists and should be 0 at the beginning)

Redis web-interface:
    http://agilepipedev.iasfbo.inaf.it/php-redis-admin-interface/?view&s=0&d=0&key=test
    auth Redis@RTA18#
"""



class RedisDBConnectorBASIC(DBConnector):


    def __init__(self, configFilePath=''):
        super().__init__(configFilePath)
        self.host     = self.configs['Redis']['host']
        self.password = self.configs['Redis']['password']
        self.dbname   = self.configs['Redis']['dbname']


    def getUniqueId(self, modelname):
        uniqueId = self.decodeResponse(self.conn.get('uniqueId:'+modelname))
        self.conn.incr('uniqueId:'+modelname)
        return uniqueId



    def getListElements(self, listname):
        listlength = self.conn.llen(listname)
        elements = []
        for elem in self.conn.lrange(listname, 0, listlength):
            elements.append(elem.decode('utf-8'))
        return elements

    def getSetElements(self, setname):
        members = []
        for elem in self.conn.smembers(setname):
            members.append(elem.decode('utf-8'))
        return members

    def decodeResponse(self, response):
        """By default, all Redis responses are returned as bytes in Python 3. This
        function converts the bytecode back to utf-8

        Keyword arguments:
        response -- Redis bytecode response (required)

        Return value:
        The utf-8 decoding of the response
        """
        return response.decode("utf-8")


    def connect(self):
        """Connects to Redis.

        Keyword arguments:
        db -- the database name (default 0)

        Return value:
        True  -- if connection is established
        False -- otherwise
        """
        self.conn = redis.Redis(host=self.host, port=6379, db=self.dbname, password=self.password)
        if self.testConnection():
            return True
        else:
            return False



    def disconnect(self):
        """Disconnects from Redis.

        Keyword arguments:
         -- (default 0)

        Return value:
         --
        """
        if self.conn != 0:
            self.conn.disconnect()
        print("Disconnected from Redis")



    def testConnection(self):
        """ Check if connection is alive.

        Keyword arguments:
        --

        Return value:
        True  -- if connection is alive
        False -- otherwise
        """
        if self.conn:
            try:
                self.conn.ping()
                return True
            except redis.exceptions.ResponseError as err:
                print(err)
                return False
        return False



    def insertData(self, modelName, dataDict):
        """
        """
        uniqueId = self.getUniqueId(modelName)
        self.conn.zadd(modelName+':'+uniqueId, dataDict)
        return True
