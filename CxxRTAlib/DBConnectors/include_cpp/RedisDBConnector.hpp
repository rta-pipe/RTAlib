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

#ifndef REDIS_DB_CONNECTOR_HPP
#define REDIS_DB_CONNECTOR_HPP

#include "DBConnector.hpp"
#include "redisDBConnector.h"


class RedisDBConnector : public DBConnector {
public:
  RedisDBConnector(int id,string filepath="") : DBConnector(filepath ){ //cout << "RedisDBConnector" << endl;
                                                                        // cout << "[RedisDBConnector] idConnector: " << id << endl;
                                                                        idConnector = id;
                                                                        c = 0;
                                                                        commandsSent = 0;
                                                                        insertDataCall = 0;
                                                                        strTrCall = 0;
                                                                        commitCall = 0;
                                                                        flagTransaction = 0;
                                                                        inserted = false;
                                                                        batchsize = 0;

                                                                        hostname = config->file["Redis"]["host"].getString();
                                                                        username = config->file["Redis"]["username"].getString();
                                                                        password = config->file["Redis"]["password"].getString();
                                                                        database = config->file["Redis"]["dbname"].getString();
                                                                        indexon = config->file["Redis"]["indexon"].getString();
                                                                        modelName = config->file["General"]["modelname"].getString();
                                                                        batchsize = config->file["General"]["batchsize"].getInt();

                                                                        int dp = indexon.find(":");
                                                                        indexon_clean = indexon.substr(dp+1,indexon.size());

                                                                       };
  bool connect(Mutex* mux);
  bool disconnect();
  bool insertData(string modelname, map < string, string > args);

  string buildQuery(string modelName, int batchsize, map <string,string> args);
  bool streamingInsert(string query);
  bool batchInsert(string query, int batchsize);

  redisContext *c;

  int idConnector;
  int commandsSent;
  int insertDataCall;
  int strTrCall;
  int commitCall;
  int flagTransaction;
  bool inserted;
  int batchsize;

  string hostname;
  string username;
  string password;
  string database;
  string modelName;
  string indexon;
  string indexon_clean;
  string score;


};

#endif
