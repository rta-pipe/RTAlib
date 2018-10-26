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



For each data model (e.g. evt3astri) we have:

    - The model itself (hashset) (e.g. evt3astri:id)

    - The unique id handler (string) (e.g. evt3astri:uniqueid)

    - The model indexed keys name set (set) (e.g. indexset:evt3astri = [time, anotherkey] )

    - One or more real indexes (zset) (e.g. evt3astri:time = [6,2,3,5,1,7,4,..] )

Redis web-interface:
    http://agilepipedev.iasfbo.inaf.it/php-redis-admin-interface/?view&s=0&d=0&key=test
    auth Redis@RTA18#
"""




class RedisDBConnector(DBConnector):

    modelsIndexes = 'ModelsIndexes'


    def __init__(self, configFilePath=''):
        super().__init__(configFilePath)
        self.cachedIndexes = {}
        """
            cachedIndexes: {
                'evt3astri' : [time, anotherkey],
                'evt3cta' : [obsid]
            }
        """

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


    def cacheAllKeyIndexes(self):
        members = self.getSetElements(self.modelsIndexes)
        for member in members:
            indexedKeys = self.getSetElements(member)
            self.cachedIndexes[member] = indexedKeys
        print(self.cachedIndexes)


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
        When connection is established it caches all the index keys.

        Keyword arguments:
        db -- the database name (default 0)

        Return value:
        True  -- if connection is established
        False -- otherwise
        """
        self.conn = redis.Redis(
                                    host=self.config.get('Redis','host'),
                                    port=6379,
                                    db=self.config.get('Redis','dbname'),
                                    password=self.config.get('Redis','password')
                                )
        if self.testConnection():
            self.cacheAllKeyIndexes()
            return True
        else:
            return False
        return



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



    def insertData(self, hashName, dataDict):
        """ Inserts the input dictionary data 'kwargs' into the hashset 'hashName'.
        Stategy:
            - it computes a new uniqueid for the 'hashName' HASHSET.
            - it insert the new HASHSET in the Redis DB
            - check if there is an index list associated with the hashset
            - if the index list exists, it downloads from the

        Keyword arguments:
         -- (default 0)

        Return value:

        """
        pass

    def createIndex(self, hashName, keyName):
        """ Creates a new indexlist for the key 'keyName' in the HASHSET 'hashName'.
        It downloads all the HASHSET 'hashName' and inserts into the ZSET all the
        value associated to the key 'keyName'.
        It also updates the local self.indexLists.

        Keyword arguments:
        hashName -- the name of the HASHSET in which the key is located (required)
        keyName  -- the name of the key that is need to be indexed (required)

        Return value:
        True  - if no exception occurs
        False - otherwise
        """
        pass



    def updateIndex(self, hashName, keyName, value):
        """ Update the ZSET 'hashName':'keyName', inserting the new value.

        Keyword arguments:
        hashName -- the name of the HASHSET in which the key is located (required)
        keyName  -- the name of the key that is need to be indexed (required)
        value    -- the value to be inserted in the ZSET

        Return value:
        True  - if no exception occurs
        False - otherwise
        """
        pass


        """
        OLD CODE

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
            print(self.cachedIndexes)

        let's try to avoid too many Redis calls
        def getUniqueId(self, modelname):
            self.conn.incr('uniqueId:'+modelname) # if key doesnt exist it will be created with value 0
            uniqueId = self.decodeResponse(self.conn.get('uniqueId:'+modelname))
            return int(uniqueId)-1

        def getUniqueIds(self, modelname, howMany):
            self.conn.incrby('uniqueId:'+modelname, howMany) # if key doesnt exist it will be created with value howMany
            uniqueId = self.decodeResponse(self.conn.get('uniqueId:'+modelname))
            return list(range(int(uniqueId)-howMany, howMany))



        def decodeResponse(self, response):
            By default, all Redis responses are returned as bytes in Python 3. This
            function converts the bytecode back to utf-8

            Keyword arguments:
            response -- Redis bytecode response (required)

            Return value:
            The utf-8 decoding of the response
            return response.decode('utf-8')

        def getSetElements(self, setname):
            return self.decodeResponseList(self.conn.smembers(setname))


        def decodeResponseList(self, responseListBytecode):

            responseList = []
            for lb in responseListBytecode:
                responseList.append(lb.decode('utf-8'))
            return responseList





        def getListElements(self, listname):
            listlength = self.conn.llen(listname)
            return self.decodeResponseList(self.conn.lrange(listname, 0, listlength))

        def getKeys(self, pattern):
            return self.decodeResponseList(self.conn.keys(pattern))


        def checkIfTableExist(self, tablename):
            Check if table 'tableName' exists in the database. (FOR NOW THIS METHOD IS NOT USED)

            Keyword arguments:
            tableName -- the table name (required)

            Return value:
            True  -- if table 'tableName' exists
            False -- otherwise


            dbcur = self.conn.cursor()#raw=True)
            dbcur.execute
            SELECT COUNT(*)
            FROM information_schema.tables
            WHERE table_name = '{0}'
            .format(tablename.replace('\'', '\'\'')))
            if dbcur.fetchone()[0] == 1:
                dbcur.close()
                return True
            dbcur.close()
            return False


        def buildQueryFromList(self, tableName, *args):
            Using the values of the input list, build the the INSERT query
            INSERT INTO table_name VALUES(value1, value2)

            Keyword arguments:
            list -- the dictionary (required)

            Return value:
            The right part of the INSERT query
            
            queryS = "INSERT INTO "+tableName+" "
            queryV = 'VALUES ('
            for val in args:
                queryV += str(val)+','
            queryV = queryV[:-1]
            queryV += ')'
            return queryS+queryV


        """
