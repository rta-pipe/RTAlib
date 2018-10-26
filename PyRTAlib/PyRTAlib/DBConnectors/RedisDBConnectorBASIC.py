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
"""

class RedisDBConnectorBASIC(DBConnector):


    def __init__(self, configFilePath=''):
        super().__init__(configFilePath)

        #self.cachedIndexes = {}
        self.pipe = None
        self.indexOn = self.config.get('Redis','indexon','dict')

    def connect(self):
        """Connects to Redis.

        Keyword arguments:
        --

        Return value:
        True  -- if connection is established
        False -- otherwise
        """
        connConfig = {}
        connConfig['host']       = self.config.get('Redis','host')
        connConfig['db']         = self.config.get('Redis','dbname')
        connConfig['port']       = 6379

        password = self.config.get('Redis','password')
        if(password):
            connConfig['password'] = self.config.get('Redis','password')

        self.conn = redis.StrictRedis( **connConfig )

        if self.testConnection():
            self.pipe = self.conn.pipeline()
            return True
        else:
            return False


    def disconnect(self):
        self.conn = None


    def testConnection(self):
        """ Check if connection is alive.

        Keyword arguments:
        --

        Return value:
        True  -- if connection is alive
        False -- otherwise
        """
        if not self.conn:
            return False

        try:
            self.conn.ping()
            return True
        except redis.exceptions.RedisError as err: # pragma: no cover
            print('[RedisConnector] testConnection() -> Test connection error: {}'.format(err))

        return False


    def insertData(self, modelName, dataDict):
        """
        Keyword arguments:
        -- modelName: the key name
        -- dataDict: the key's value

        Return value:
        True  -- if no error occurs
        False -- otherwise
        """
        if modelName not in self.indexOn:
            print("[RedisConnectorBASIC] Error: insertData() -> No index exists for model '{}'. Indexes supported: {}.".format(modelName,self.indexOn))
            return False

        if self.batchsize == 1:
            return self.streamingInsert(modelName, dataDict)
        elif self.batchsize > 1:
            return self.batchInsert(modelName, dataDict)
        else:
            print("[RedisConnector] Error! insertData() -> batchsize cannot be lesser than 1!")
            return False



    def streamingInsert(self, modelName, dataDict):

        index = self.indexOn[modelName]
        element = [float(dataDict[index]), json.dumps(dataDict)]

        try:
            self.conn.zadd(modelName, *element)
            return True
        except redis.exceptions.RedisError as e: # pragma: no cover
            print("[RedisConnectorBASIC] Error: streamingInsert() -> {}".format(e))
            return False



    def batchInsert(self, modelName, dataDict):

        index = self.indexOn[modelName]

        ## Start transaction
        if self.commandsSent == 0:
            try:
                self.pipe = self.conn.pipeline()
                if self.debug:
                    print("[RedisConnectorBASIC] batchInsert() -> Transaction started!")

            except redis.exceptions.RedisError as e: # pragma: no cover
                print("[RedisConnectorBASIC] Error: batchInsert() -> {}".format(e))
                return False

        ## Inserting element
        element = [float(dataDict[index]), json.dumps(dataDict)]
        try:
            self.pipe.zadd(modelName, *element)
        except redis.exceptions.RedisError as e: # pragma: no cover
            print("[RedisConnectorBASIC] Error: batchInsert() -> {}".format(e))
            return False

        self.commandsSent += 1


        ## Handle commit
        if self.commandsSent >= self.batchsize:
            try:
                self.pipe.execute()
                if self.debug:
                    print("[RedisConnectorBASIC] batchInsert() -> commandsSent: {} batchsize: {} -> Commit executed!". format(self.commandsSent, self.batchsize))
            except redis.exceptions.RedisError as e: # pragma: no cover
                print("[RedisConnectorBASIC] Error: batchInsert() -> {}".format(e))
                return False

            self.commandsSent = 0



        return True



    def close(self):

        if self.testConnection():

            if self.debug:
                print("[RedisConnectorBASIC] close() -> Command sent: {}.  Closing connection..".format(self.commandsSent))

            if self.commandsSent > 0:
                if self.debug:
                    print("[RedisConnectorBASIC] close() -> Closing transaction..")

                try:
                    self.pipe.execute()
                except redis.exceptions.RedisError as e: # pragma: no cover
                    print("[RedisConnectorBASIC] Error! close() -> Can't commit transaction. Error: {}".format(e))
                    return False

                self.commandsSent = 0


            self.disconnect()
            return True
        else:
            print("[RedisConnectorBASIC] Error! Connection is already closed")
            self.conn = None
            return False
