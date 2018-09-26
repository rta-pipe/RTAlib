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
//
// #include "DBConnector.h"
// #include "hiredis.h"
//
//
//
// class RedisDBConnector : public DBConnector {
// public:
//   RedisDBConnector(string hostname, string username, string password, string database, string tablename)
//   :DBConnector(hostname, username, password, database, tablename){ cout << "RedisDBConnector" << endl; };
//   virtual int connect(string _hostname, string _username, string _password, string _database);
//   virtual int disconnect();
//   virtual int testConnection();
//   virtual int insertData(string modelName, vector < pair <string,double> > args);
//
//
//   string buildQuery(string modelName, vector < pair <string,double> > args);
//
//   string hostname;
//   string username;
//   string password;
//   string database;
//   string tablename;
//
//
// };
