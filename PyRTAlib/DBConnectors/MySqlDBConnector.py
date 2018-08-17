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
        self.host     = self.configs['MySql']['host']
        self.username = self.configs['MySql']['username']
        self.password = self.configs['MySql']['password']
        self.dbname   = self.configs['MySql']['dbname']
        self.cursor = None


    def close(self):
        if self.conn and self.conn.is_connected() and self.conn.in_transaction:
            print("[MySqlConnector] Commiting last transaction before exiting")
            self.conn.commit()
        self.disconnect()


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
            self.cursor = self.conn.cursor()
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
        if self.conn and self.conn.is_connected():
            print("[MySqlConnector] Disconnected")
            self.cursor.close()
            self.conn.close()


    def insertData(self):
        pass

    def insertData_1(self, query):

        if self.conn:
            print("[MySqlConnector] Executing {}..".format(query))

            if self.commandsSent == 0 and self.batchsize > 1:

                print("[MySqlConnector] Starting transaction..")
                try:
                    self.conn.start_transaction(consistent_snapshot=True, isolation_level=None)

                except mysql.connector.Error as err:
                    print("[MySqlConnector] Err: {}".format(err))
                    return False

            try:
                self.cursor.execute(query)

            except mysql.connector.Error as err:
                print("[MySqlConnector] Error from database: {}".format(err))
                return False


            self.commandsSent += 1

            if self.commandsSent >= self.batchsize or self.batchsize == 1:
                try:
                    print("[MySqlConnector] Committing transaction..")
                    self.conn.commit()
                    self.commandsSent = 0
                    return True

                except mysql.connector.Error as err:
                    print("[MySqlConnector] Failed to commit transaction to database: {}".format(err))
                    return False
            else:
                    return True
        else:
            print("[MySqlConnector] Error, self.conn is None")

    def insertData(self, tableName, *args):
        """Inserts the input list data 'args' into the table 'tableName' within a transaction.
        Before committing the transaction it waits until the number of commands sent is equal to
        Config.General.batchsize.
        If the batchsize is equal to 1, no explicit transaction is executed.
        The 'is_connected' test is skipped here to decrease latency.

        Keyword arguments:
        tableName -- the name of the table in which the data is inserted to (required)
        args    -- the list holding the data in terms of columns values

        Return value:
        True  -- if the data is inserted correctly
        False -- otherwise
        """
        if self.conn:
            insertQuery = self.buildQueryFromList(tableName, *args)
            print("[MySqlConnector] Executing {}..".format(insertQuery))

            if self.commandsSent == 0 and self.batchsize > 1:

                print("[MySqlConnector] Starting transaction..")
                try:
                    self.conn.start_transaction(consistent_snapshot=True, isolation_level=None)

                except mysql.connector.Error as err:
                    print("[MySqlConnector] Err: {}".format(err))
                    return False

            try:
                self.cursor.execute(insertQuery)

            except mysql.connector.Error as err:
                print("[MySqlConnector] Error from database: {}".format(err))
                return False


            self.commandsSent += 1

            if self.commandsSent >= self.batchsize or self.batchsize == 1:
                try:
                    print("[MySqlConnector] Committing transaction..")
                    self.conn.commit()
                    self.commandsSent = 0
                    return True

                except mysql.connector.Error as err:
                    print("[MySqlConnector] Failed to commit transaction to database: {}".format(err))
                    return False
            else:
                    return True
        else:
            print("[MySqlConnector] Error, self.conn is None")


    def insertData_3(self, tableName, **kwargs):

        if self.conn:
            insertQuery = self.buildQueryFromDictionary(tableName, **kwargs)
            print("[MySqlConnector] Executing {}..".format(insertQuery))

            if self.commandsSent == 0 and self.batchsize > 1:

                print("[MySqlConnector] Starting transaction..")
                try:
                    self.conn.start_transaction(consistent_snapshot=True, isolation_level=None)

                except mysql.connector.Error as err:
                    print("[MySqlConnector] Err: {}".format(err))
                    return False

            try:
                self.cursor.execute(insertQuery)

            except mysql.connector.Error as err:
                print("[MySqlConnector] Error from database: {}".format(err))
                return False


            self.commandsSent += 1

            if self.commandsSent >= self.batchsize or self.batchsize == 1:
                try:
                    print("[MySqlConnector] Committing transaction..")
                    self.conn.commit()
                    self.commandsSent = 0
                    return True

                except mysql.connector.Error as err:
                    print("[MySqlConnector] Failed to commit transaction to database: {}".format(err))
                    return False
            else:
                    return True
        else:
            print("[MySqlConnector] Error, self.conn is None")


    def executeQuery(self, query):
        if self.conn and not self.conn.in_transaction:
            try:
                self.cursor.close()
                self.cursor = self.conn.cursor()
                self.cursor.execute(query)
                self.conn.commit()
                return True
            except mysql.connector.Error as err:
                print("[MySqlConnector] Failed to execute query {}. Error: {}".format(query, err))
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

    def buildQueryFromDictionary(self, tableName, **kwargs):
        """Using the key/value of the input dictionary, builds the the INSERT query
        INSERT INTO table_name(column1, column2) VALUES(value1, value2)

        Keyword arguments:
        kwargs -- the dictionary (required)

        Return value:
        The the INSERT query
        """
        queryS = "INSERT INTO "+tableName
        queryH = '('
        queryV = 'VALUES ('
        for key, val in kwargs.items():
            queryH += str(key)+','
            queryV += str(val)+','
        queryH = queryH[:-1]
        queryV = queryV[:-1]
        queryH += ')'
        queryV += ')'
        return queryS+queryH+' '+queryV

    def buildQueryFromList(self, tableName, *args):
        """Using the values of the input list, build the the INSERT query
        INSERT INTO table_name VALUES(value1, value2)

        Keyword arguments:
        list -- the dictionary (required)

        Return value:
        The right part of the INSERT query
        """

        queryS = "INSERT INTO "+tableName+" "
        queryV = 'VALUES ('
        for val in args:
            queryV += str(val)+','
        queryV = queryV[:-1]
        queryV += ')'
        return queryS+queryV
