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

It uses HASH-SETs as data structure. Fot each data model (e.g. evt3astri) we have:
    - the HASH-SET of the model (e.g. evt3astri:id)
    - a STRING for the generation of unique IDs (evt3astri:uniqueid)
    - a LIST to store the keys name that must be associated to an INDEX (e.g. evt3astri:indexlist)
    - a ZSET for each key associated to an INDEX (e.g. evt3astri:time)
"""




class RedisDBConnector(DBConnector):
    def __init__(self, url, username='', password=''):
        super().__init__(url, username, password)
        self.indexLists = []


    def decodeResponse(self, response):
        """By default, all Redis responses are returned as bytes in Python 3. This
        function converts the bytecode back to utf-8

        Keyword arguments:
        response -- Redis bytecode response (required)

        Return value:
        The utf-8 decoding of the response
        """
        return response.decode("utf-8")



    def connect(self, db = 0):
        """Connects to Redis. When connection is established it downloads from Redis
        the indexlists and caches it.

        Keyword arguments:
        db -- the database name (default 0)

        Return value:
        True  -- if connection is established
        False -- otherwise
        """
        self.conn = redis.Redis(host=self.url, port=6379, db=db, password=self.password)
        if self.testConnection():
            # TODO Download all the indexlists (we cache the indexlists)
            return True
        else
            return False
        return



    def disconnect(self):
        """

        Keyword arguments:
         -- (default 0)

        Return value:

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



    def insertData(self, hashName, **kwargs):
        """ Inserts the input dictionary data 'kwargs' into the hashset 'hashName'.
        It computes a new uniqueid for the 'hashName' HASHSET.

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
