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

    # Class variables
    SUCCESS = 200
    SUCCESS_AND_COMMIT = 201


    def __init__(self, configFilePath='', connectTo=''):
        super().__init__(configFilePath);
        self.cursor = None
        self.autocommit = False

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

        connConfig = {}
        connConfig['user']       = self.config.get('MySql','username')
        connConfig['host']       = self.config.get('MySql','host')
        connConfig['port']       = self.config.get('MySql','port', 'int')
        connConfig['database']   = self.config.get('MySql','dbname')
        connConfig['password']   = self.config.get('MySql','password')
        connConfig['use_pure']   = False
        connConfig['autocommit'] = self.autocommit
        #connConfig['pool_size'] = self.config.get('General','numberofthreads', 'int')

        try:
            self.conn = mysql.connector.connect(**connConfig)

        except mysql.connector.Error as err:
            print("[MySqlConnector] Error! connect() -> {}".format(err))
            return False

        try:
            self.cursor = self.conn.cursor()

        except mysql.connector.Error as err:
            print("[MySqlConnector] Error! cursor() -> {}".format(err))
            self.conn.close()
            self.conn = None
            return False

        if self.debug:
            print("[MySqlConnector] connect() -> Connected to MySql. {}@{} -> {}".format(connConfig['user'], connConfig['host'], connConfig['database']))

        return True


    def testConnection(self):
        if self.conn:
            return self.conn.is_connected()
        else:
            return False

    def close(self):
        if self.testConnection():


            if self.conn.in_transaction:

                if self.debug:
                    print("[MySqlConnector] close() -> Committing last transaction before disconnecting")

                try:
                    self.conn.commit()
                except mysql.connector.Error as err:
                    print("[MySqlConnector] Error! close() Can't commit transaction -> {}".format(err))

                if self.debug:
                    print("[MySqlConnector] close() -> In transaction: {}".format(self.conn.in_transaction))

            else:
                if self.debug:
                    print("[MySqlConnector] close() -> No need to commit. Exiting without committing")

            self.disconnect()
            return True
        else:
            print("[MySqlConnector] Error! close() -> connection is already closed")
            return False



    def disconnect(self):
        """Disconnects to MySql.

        Keyword arguments:
        --

        Return value:
        --
        """
        if self.debug:
            print("[MySqlConnector] disconnect() -> Disconnecting..")
        self.cursor.close()
        self.conn.close()


    def insertData(self, tableName, dataDict):
        """
        Keyword arguments:
        -- tableName
        -- dataDict

        Return value:
        True  -- if no error occurs
        False -- otherwise
        """
        query = self.buildQueryFromDictionary(tableName, dataDict)

        if self.debug:
            print('[MySqlConnector] insertData() -> \n   Query: {}\n   Batch size: {}'.format(query,self.config.get('General','batchsize', 'int')))

        if self.config.get('General','batchsize', 'int') == 1:
            return self.streamingInsert(query)
        elif self.config.get('General','batchsize', 'int') > 1:
            return self.batchInsert(query)
        else:
            print("[MySqlConnector] Error! insertData() -> batchsize cannot be lesser than 1!")
            return False


    def fakeInsertData(self, tableName, dataDict): # pragma: no cover
        query = self.buildQueryFromDictionary(tableName, dataDict)
        return True


    def streamingInsert(self, query):

        try:
            self.cursor.execute(query)
            return self.SUCCESS_AND_COMMIT

        except mysql.connector.Error as err:
            print("[MySqlConnector] streamingInsert() -> Error: {}.\nQuery: {}".format(err,query))
            return False


    def batchInsert(self, query):

        if self.commandsSent == 0:
            if self.debug:
                print("[MySqlConnector] batchInsert() -> Starting transaction..")
            try:
                #Transaction isolation is one of the foundations of database processing. Isolation is the I in the acronym ACID;
                # the isolation level is the setting that fine-tunes the balance between performance and reliability, consistency,
                # and reproducibility of results when multiple transactions are making changes and performing queries at the same time.
                self.conn.start_transaction()#consistent_snapshot=False, isolation_level=None)

            except mysql.connector.Error as err:
                print("[MySqlConnector] batchInsert() -> Start transaction err: {}".format(err))
                return False

        try:
            self.cursor.execute(query)

        except mysql.connector.Error as err:
            print("[MySqlConnector] batchInsert() -> Execute query err: {}.\nQuery: {}".format(err,query))
            return False


        self.commandsSent += 1

        if self.commandsSent >= self.config.get('General','batchsize', 'int'):
            if self.debug:
                print("[MySqlConnector] batchInsert() -> Committing.. (command sent: {}, batchsize: {})".format(self.commandsSent, self.config.get('General','batchsize', 'int')))

            try:
                self.conn.commit()
                self.commandsSent = 0
                return self.SUCCESS_AND_COMMIT

            except mysql.connector.Error as err:
                print("[MySqlConnector] batchInsert() -> Failed to commit transaction to database: {}".format(err))
                return False
        else:
                return self.SUCCESS


    def executeQuery(self, query):
        if self.testConnection():

            if self.debug:
                print("[MySqlConnector] executeQuery() -> {}.. (conn in transaction: {}, autocommit: {})".format(query,self.conn.in_transaction, self.autocommit))

            try:
                self.cursor.execute(query)
                if not self.autocommit:
                    self.conn.commit()
                return True
            except mysql.connector.Error as err:
                print("[MySqlConnector] executeQuery() -> Failed to execute query {}. Error: {}".format(query, err))
                return False

        else:
            print("[MySqlConnector] Error! executeQuery() -> connection is closed")
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
