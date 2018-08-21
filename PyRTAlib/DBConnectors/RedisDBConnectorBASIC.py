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
import json

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
        self.cachedIndexes = {}
        self.cachedUniqueIds = {}
        self.pipe = None


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
            self.cacheAllKeyIndexes()
            self.cacheUniqueIds()
            self.pipe = self.conn.pipeline()
            #print(self.cachedIndexes)
            #print(self.cachedUniqueIds)
            return True
        else:
            return False


    def disconnect(self):
        """No need to disconnect from Redis. It uses a connection pool :)
        """
        pass


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
                print('[RedisConnector] Error: {}'.format(err))
                return False
        return False

    def decodeResponse(self, response):
        """By default, all Redis responses are returned as bytes in Python 3. This
        function converts the bytecode back to utf-8

        Keyword arguments:
        response -- Redis bytecode response (required)

        Return value:
        The utf-8 decoding of the response
        """
        return response.decode('utf-8')

    def decodeResponseList(self, responseListBytecode):
        """
        Keyword arguments:
        Return value:
        """
        responseList = []
        for lb in responseListBytecode:
            responseList.append(lb.decode('utf-8'))
        return responseList




    def getSetElements(self, setname):
        return self.decodeResponseList(self.conn.smembers(setname))

    def getListElements(self, listname):
        listlength = self.conn.llen(listname)
        return self.decodeResponseList(self.conn.lrange(listname, 0, listlength))

    def getKeys(self, pattern):
        return self.decodeResponseList(self.conn.keys(pattern))

    def cacheUniqueIds(self):
        keys = self.getKeys('uniqueId:*')
        for key in keys:
            id = self.decodeResponse(self.conn.get(key))
            self.cachedUniqueIds[key] = int(id)

    def cacheAllKeyIndexes(self):
        keys = self.getKeys('indexstring:*')
        for key in keys:
            indexedKey = self.decodeResponse(self.conn.get(key))
            self.cachedIndexes[key] = indexedKey
        #print(self.cachedIndexes)

    """ let's try to avoid too many Redis calls
    def getUniqueId(self, modelname):
        self.conn.incr('uniqueId:'+modelname) # if key doesnt exist it will be created with value 0
        uniqueId = self.decodeResponse(self.conn.get('uniqueId:'+modelname))
        return int(uniqueId)-1

    def getUniqueIds(self, modelname, howMany):
        self.conn.incrby('uniqueId:'+modelname, howMany) # if key doesnt exist it will be created with value howMany
        uniqueId = self.decodeResponse(self.conn.get('uniqueId:'+modelname))
        return list(range(int(uniqueId)-howMany, howMany))
    """

    def insertData(self, modelName, dataDict):
        """
        Keyword arguments:
        --
        Return value:
        True  -- if no error occurs
        False -- otherwise
        """

        try:
            index = self.cachedIndexes['indexstring:'+modelName]
            currentUniqueId = self.cachedUniqueIds['uniqueId:'+modelName]

        except KeyError as e:
            print('[RedisConnectorBASIC] Error: {}\nPlease, insert in Redis a String with key: "indexstring:{}" and value equal to the query filter attribute for that model'.format(e, modelName))
            # self.conn.delete(modelName) -> the unique key should be deleted
            return False

        if self.conn and self.batchsize == 1:
            return self.streamingInsert(modelName, dataDict, index, currentUniqueId)
        elif self.conn and self.batchsize > 1:
            return self.batchInsert(modelName, dataDict, index, currentUniqueId)
        else:
            print("[RedisConnector] Error, self.conn is None")
            return False



    def streamingInsert(self, modelName, dataDict, index, currentUniqueId):
        try:

            self.pipe.zadd(modelName+':'+str(currentUniqueId), json.dumps(dataDict), dataDict[index])
            self.pipe.incr('uniqueId:'+modelName)
            self.cachedUniqueIds['uniqueId:'+modelName] += 1
            self.pipe.execute()
            return True
        except redis.ConnectionError as e:
            print("[RedisConnectorBASIC] Error: {}".format(e))
            return False



    def batchInsert(self, modelName, dataDict, index, currentUniqueId):
        if self.commandsSent == 0:
            try:
                self.pipe = self.conn.pipeline()

            except redis.ConnectionError as e:
                print("[RedisConnectorBASIC] Error: {}".format(e))
                return False



        self.pipe.zadd(modelName+':'+str(currentUniqueId), json.dumps(dataDict), dataDict[index])
        self.cachedUniqueIds['uniqueId:'+modelName] += 1

        self.commandsSent += 1

        if self.commandsSent >= self.batchsize:
            try:
                self.commandsSent = 0
                self.pipe.incrby('uniqueId:'+modelName, self.batchsize)
                self.pipe.execute()
                return True

            except redis.ConnectionError as e:
                print("[RedisConnectorBASIC] Error: {}".format(e))
                return False

        else:
                return True



    def close(self):
        pass
