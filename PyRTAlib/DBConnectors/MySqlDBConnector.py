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
        self.cursor = None
        self.autocommit = False


    def close(self):
        if self.conn and self.conn.is_connected() and self.conn.in_transaction:
            if self.config.get('General','debug') == 'yes':
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
        if self.config.get('General','batchsize', 'int') == 1:
            self.autocommit = True

        try:
            self.conn = mysql.connector.connect(
                                                    user=self.config.get('MySql','username'),
                                                    password=self.config.get('MySql','password'),
                                                    host=self.config.get('MySql','host'),
                                                    database=self.config.get('MySql','dbname'),
                                                    use_pure=False,
                                                    autocommit=self.autocommit
                                                )
            self.cursor = self.conn.cursor()#raw=True)
            if self.config.get('General','debug') == 'yes':
                print("[MySqlConnector] Connected to MySql")
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
            if self.config.get('General','debug') == 'yes':
                print("[MySqlConnector] connected: {}, disconnecting..".format(self.conn.is_connected()))
            self.cursor.close()
            self.conn.close()


    def insertData(self, tableName, dataDict):
        """

        Keyword arguments:
        --

        Return value:
        True  -- if no error occurs
        False -- otherwise
        """
        query = self.buildQueryFromDictionary(tableName, dataDict)

        if self.config.get('General','debug') == 'yes':
            print(query)

        if self.conn and self.config.get('General','batchsize', 'int') == 1:
            return self.streamingInsert(query)
        elif self.conn and self.config.get('General','batchsize', 'int') > 1:
            return self.batchInsert(query)
        else:
            print("[MySqlConnector] Error, self.conn is None")
            return False

    def fakeInsertData(self, tableName, dataDict):
        """

        Keyword arguments:
        --

        Return value:
        True
        """
        query = self.buildQueryFromDictionary(tableName, dataDict)
        return True


    def streamingInsert(self, query):
        try:
            self.cursor.execute(query)
            return True

        except mysql.connector.Error as err:
            print("[MySqlConnector] Error from database: {}".format(err))
            return False


    def batchInsert(self, query):
        if self.commandsSent == 0:
            if self.config.get('General','debug') == 'yes':
                print("[MySqlConnector] Starting transaction..")
            try:
                #Transaction isolation is one of the foundations of database processing. Isolation is the I in the acronym ACID;
                # the isolation level is the setting that fine-tunes the balance between performance and reliability, consistency,
                # and reproducibility of results when multiple transactions are making changes and performing queries at the same time.
                self.conn.start_transaction()#consistent_snapshot=False, isolation_level=None)

            except mysql.connector.Error as err:
                print("[MySqlConnector] Start transaction Err: {}".format(err))
                return False

        try:
            #print("[MySqlConnector] Executing..")
            self.cursor.execute(query)

        except mysql.connector.Error as err:
            print("[MySqlConnector] Execute Err: {}".format(err))
            return False


        self.commandsSent += 1

        if self.commandsSent >= self.config.get('General','batchsize', 'int'):
            try:
                if self.config.get('General','debug') == 'yes':
                    print("[MySqlConnector] Committing.. (command sent: {}, batchsize: {})".format(self.commandsSent, self.config.get('General','batchsize', 'int')))

                self.conn.commit()
                self.commandsSent = 0
                return True

            except mysql.connector.Error as err:
                print("[MySqlConnector] Failed to commit transaction to database: {}".format(err))
                return False
        else:
                return True


    def executeQuery(self, query):
        if self.conn:# and not self.conn.in_transaction:
            if self.config.get('General','debug') == 'yes':
                print("[MySqlConnector] Executing query {}.. (conn in transaction: {}, autocommit: {})".format(query,self.conn.in_transaction, self.autocommit))

            try:
                #self.cursor.close()
                #self.cursor = self.conn.cursor()
                self.cursor.execute(query)
                if not self.autocommit:
                    self.conn.commit()
                return True
            except mysql.connector.Error as err:
                print("[MySqlConnector] Failed to execute query {}. Error: {}".format(query, err))
                return False
        elif not self.conn:
            print("[MySqlConnector] Error, self.conn is None")
            return False
        #else:
        #    print("[MySqlConnector] Error, Transaction in progress")
        #    return False



    def checkIfTableExist(self, tablename):
        """Check if table 'tableName' exists in the database. (FOR NOW THIS METHOD IS NOT USED)

        Keyword arguments:
        tableName -- the table name (required)

        Return value:
        True  -- if table 'tableName' exists
        False -- otherwise
        """

        dbcur = self.conn.cursor()#raw=True)
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

    def buildQueryFromDictionary(self, tableName, dict):
        """Using the key/value of the input dictionary, builds the the INSERT query
        INSERT INTO table_name(column1, column2) VALUES(value1, value2)

        Keyword arguments:
        dict -- the dictionary (required)

        Return value:
        The the INSERT query
        """
        queryS = "INSERT INTO "+tableName
        queryH = '('
        queryV = 'VALUES ('
        for key, val in dict.items():
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
