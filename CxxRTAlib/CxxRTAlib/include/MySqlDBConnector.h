/*
 ==========================================================================

 Copyright (C) 2018 Giancarlo Zollino
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ==========================================================================
*/

#ifndef MYSQL_DB_CONNECTOR_H
#define MYSQL_DB_CONNECTOR_H

#include "DBConnector.h"

#include <string>
#include <boost/scoped_ptr.hpp>

#include "mysql_connection.h"
#include "cppconn/statement.h"
#include "mysql_driver.h"
#include "examples.h"

#define ONE_ROW 1

using namespace std;

class MySqlDBConnector : public DBConnector {
public:
  MySqlDBConnector(string filepath="") : DBConnector(filepath){ cout << "MySqlDBConnector" << endl; };
  virtual int connect();
  virtual int disconnect();
  virtual int testConnection();
  virtual int insertData(map < string, string > args);

  string buildQuery(string modelName, int batchsize, map <string,string> args);
  int streamingInsert(string query);
  int batchInsert(string query, int batchsize);

  sql::Driver *driver;
  sql::Connection *con;
  //boost::shared_ptr <sql::Connection> con;
  int commandsSent = 0;
  int insertDataCall = 0;
  int strTrCall = 0;
  int commitCall = 0;
  int flagTransaction = 0;

  string hostname;
  string username;
  string password;
  string database;
  string modelName;
  int batchsize;


};

#endif
