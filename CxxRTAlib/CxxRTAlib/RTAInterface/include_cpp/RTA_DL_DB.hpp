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

#ifndef RTA_DL_DB_H
#define RTA_DL_DB_H


#include "MySqlDBConnector.hpp"
#include "RedisDBConnector.hpp"
#include "RTAThread.hpp"
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::make_shared;
using std::shared_ptr;
using std::unique_ptr;

class RTA_DL_DB {
public:

  RTA_DL_DB(string database, string configFilePath = "");

  shared_ptr<DBConnector> dbConnector;
  Config * config;
  CTABuffer * eventBuffer;
  bool pure_multithreading;


  int numberofthreads;
  string modelname;

  std::vector< shared_ptr < Thread > > thread_array;
  // std::vector<ThreadStatistic*> thread_statistics_array;

  void start();
  shared_ptr<DBConnector> getConnector(int id,string databaseEngine, string configFilePath);
  int _insertEvent(EVTbase *event);
  bool waitAndClose();
  int getNumberOfThreads();

};

#endif
