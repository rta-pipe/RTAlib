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
    def __init__(self, url, username, password):
        super().__init__(url, username, password);
        self.conn = None;


    # Connection argument
    # https://dev.mysql.com/doc/connector-python/en/connector-python-connectargs.html
    def connect(self, db):
        try:
            self.conn = mysql.connector.connect(user=self.username, password=self.password, host=self.url, database=db)
            print("Connected to MySql")
            return True
        except mysql.connector.Error as err:
            print(err)
            self.conn = None
            return False

    def testConnection(self):
        return self.conn.is_connected()

    def disconnect(self):
        print("MySql disconnection")
        if self.conn != 0:
            self.conn.close()
            self.conn = None



    def insertData(self, tableName, **kwargs):
        if self.checkTableExists(tableName):
            cursor = cnx.cursor()
            insertQuery = "INSERT INTO "+tableName+self.buildQueryHeaderAndValuesFromDictionary(kwargs)
            print("[MySqlConnector] Executing {}..".format(insertQuery))
            cursor.execute(insertQuery)
            cnx.commit()
            cursor.close()
            return True
        else:
            print('[MySqlDBConnector Error] Table {} doesnt exist'.format(tableName))
            return False

    def checkTableExists(self, tablename):
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
