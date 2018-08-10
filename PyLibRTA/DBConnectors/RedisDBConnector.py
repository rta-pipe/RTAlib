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



class RedisDBConnector(DBConnector):
    def __init__(self, url, username='', password=''):
        super().__init__(url, username, password)
        self.conn = None;


    """
    By default, all responses are returned as bytes in Python 3
    """
    def decodeResponse(self, response):
        return response.decode("utf-8")

    ### TODO Connecting with username and password
    def connect(self, db=0):
        super().connect(db)
        self.conn = redis.StrictRedis(host=self.url, port=6379, db=db )
        print("Connected to Redis")
        return True

    def disconnect(self):
        if self.conn != 0:
            self.conn.disconnect()
        print("Disconnected from Redis")

    # TODO look for a smarter way
    def testConnection(self):
        self.conn.set('testconnection', '1')
        val = self.decodeResponse(self.conn.get('testconnection'))
        if val == '1':
            return True
        else:
            return False

    """

    """
    def createHash(self, key, expirationTime, **kwargs, *args):
        for key, value in kwargs.items():
            print("The value of {} is {}".format(key, value))
        for num in args:
            print(num)

        # Create Indexes

        #
