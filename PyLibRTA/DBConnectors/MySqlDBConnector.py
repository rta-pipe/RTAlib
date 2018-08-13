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

import mysql.connector
from mysql.connector import errorcode


class MySqlDBConnector(DBConnector):

    def __init__(self, configFilePath=''):
        super().__init__(configFilePath);
        self.host = self.configs['MySql']['host']
        self.username = self.configs['MySql']['username']
        self.password = self.configs['MySql']['password']
        self.dbname = self.configs['MySql']['dbname']


    # https://dev.mysql.com/doc/connector-python/en/connector-python-connectargs.html
    def connect(self):
        """Connects to MySql.

        Keyword arguments:
        --

        Return value:
        True  -- if the connection is estrablished
        False -- otherwise
        """
        try:
            self.conn = mysql.connector.connect(user=self.username, password=self.password, host=self.host, database=self.dbname)
            print("Connected to MySql")
            return True
        except mysql.connector.Error as err:
            print(err)
            self.conn = None
            return False

    def testConnection(self):
        return self.conn.is_connected()

    def disconnect(self):
        """Disconnects to MySql.

        Keyword arguments:
        --

        Return value:
        --
        """
        print("MySql disconnection")
        if self.conn != 0:
            self.conn.close()
            self.conn = None


    def insertData(self, tableName, **kwargs):
        """Inserts the input dictionary data 'kwargs' into the table 'tableName'.

        Keyword arguments:
        tableName -- the name of the table in which the data is inserted to (required)
        kwargs    -- the dictionary that holds the data in terms of column name - column value

        Return value:
        True  -- if the data is inserted correctly
        False -- otherwise
        """
        if self.conn:
            insertQuery = "INSERT INTO "+tableName+self.buildQueryHeaderAndValuesFromDictionary(kwargs)
            print("[MySqlConnector] Executing {}..".format(insertQuery))
            try:
                cursor = self.conn.cursor()
                cursor.execute(insertQuery)
                self.conn.commit()
                cursor.close()
                return True
            except mysql.connector.Error as err:
                print(err)
                return False
        else:
            print("[insertData] Please connect to MySql before inserting data (self.conn = None)")
            return False



    def checkIfTableExist(self, tablename):
        """Check if table 'tableName' exists in the database. (FOR NOW THIS METHOD IS NOT USED)

        Keyword arguments:
        tableName -- the table name (required)

        Return value:
        True  -- if table 'tableName' exists
        False -- otherwise
        """

        dbcur = self.conn.cursor()
        dbcur.execute("""
        SELECT COUNT(*)
        FROM information_schema.tables
        WHERE table_name = '{0}'
        """.format(tablename.replace('\'', '\'\'')))
        if dbcur.fetchone()[0] == 1:
            dbcur.close()
            return True
        dbcur.close()
        return False

    def buildQueryHeaderAndValuesFromDictionary(self, dict):
        """Using the key/value of the input dictionary, build the right part of
        the INSERT query e.g. (column1, column2) VALUES(value1, value2)

        Keyword arguments:
        dict -- the dictionary (required)

        Return value:
        The right part of the INSERT query
        """
        queryH = '('
        queryV = 'VALUES ('
        for key, val in dict.items():
            queryH += str(key)+','
            queryV += str(val)+','
        queryH = queryH[:-1]
        queryV = queryV[:-1]
        queryH += ')'
        queryV += ')'
        return queryH+' '+queryV
