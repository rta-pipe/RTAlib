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

#include <string>
#include <mysqlx/xdevapi.h>

#include "DBConnector.hpp"

#define ONE_ROW 1

using std::cout;
using std::endl;
using std::stringstream;
using std::runtime_error;
using std::to_string;
using mysqlx::Session;
using mysqlx::Schema;
using mysqlx::Table;
using mysqlx::RowResult;
using mysqlx::Client;
using mysqlx::SessionSettings;
using mysqlx::SessionOption;
using mysqlx::Row;
using mysqlx::Columns;
using mysqlx::Type;
using mysqlx::row_count_t;
using mysqlx::col_count_t;
using mysqlx::SSLMode;

class MySqlDBConnector : public DBConnector {
public:
  MySqlDBConnector(int id,string filepath="") : DBConnector(filepath){  //cout << "MySqlDBConnector" << endl;
                                                                        // cout << "[MySqlDBConnector] idConnector: " << id << endl;
                                                                        idConnector = id;
                                                                        commandsSent = 0;
                                                                        inserted = false;
                                                                        insertDataCall = 0;
                                                                        flagTransaction = 0;
                                                                        batchsize = 0;
                                                                        hostname = config->file["MySql"]["host"].getString();
                                                                        port = config->file["MySql"]["port"].getInt();
                                                                        if(port == 3306)
                                                                           port = 33060; // mysqlx protocol port
                                                                        else
                                                                           port++;  // convention: if mysql port is not the default one (3306), mysqlx-port = mysql-port + 1 
                                                                        username = config->file["MySql"]["username"].getString();
                                                                        password = config->file["MySql"]["password"].getString();
                                                                        database = config->file["MySql"]["dbname"].getString();
                                                                        modelName = config->file["General"]["modelname"].getString();
                                                                        batchsize = config->file["General"]["batchsize"].getInt();
                                                                      };
  bool connect(Mutex* mux);
  bool disconnect();
  bool insertData(string modelName, map < string, string > args);

  string buildQuery(string modelName, int batchsize, map <string,string> args);
  bool streamingInsert(string query);
  bool batchInsert(string query, int batchsize);
  bool executeQuery(string query);
  double getRowSize(string query);

  shared_ptr<Session> mySession;

  int idConnector;
  int commandsSent;
  bool inserted;
  int insertDataCall;
  int flagTransaction;
  int batchsize;

  string hostname;
  int port;
  string username;
  string password;
  string database;
  string modelName;



};

#endif
