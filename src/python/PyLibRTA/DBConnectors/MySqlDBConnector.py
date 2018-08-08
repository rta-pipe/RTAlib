from .DBConnector import DBConnector

import mysql.connector
from mysql.connector import errorcode


class MySqlDBConnector(DBConnector):
    def __init__(self, url, username, password):
        super().__init__(url, username, password);
        self.conn = 0;


    # Connection argument
    # https://dev.mysql.com/doc/connector-python/en/connector-python-connectargs.html
    def connect(self, db):
        super().connect(db)
        try:
            self.conn = mysql.connector.connect(
                                    user=self.username,
                                    password=self.password,
                                    host=self.url,
                                    database=db
                                )
            print("Connected to MySql")
        except mysql.connector.Error as err:
            if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
                print("Something is wrong with your user name or password")
            elif err.errno == errorcode.ER_BAD_DB_ERROR:
                print("Database does not exist")
            else:
                print(err)

    def testConnection(self):
        print(self.conn)


    def disconnect(self):
        print("MySql disconnection")
        if self.conn != 0:
            self.conn.close()
