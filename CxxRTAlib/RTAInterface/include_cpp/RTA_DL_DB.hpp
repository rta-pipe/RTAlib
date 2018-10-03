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

#include<iostream>

#include "Config.hpp"
#include "DBConnector.hpp"
#include "MySqlDBConnector.hpp"
#include "RedisDBConnector.hpp"

#include "EVTTest.hpp"

using std::cout;
using std::endl;
using std::string;

class RTA_DL_DB {
public:

  RTA_DL_DB(string database, string configFilePath = "");
  DBConnector * getConnector(string databaseEngine, string configFilePath);
  int _insertEvent(EVTTest & event);
  int waitAndClose();

  DBConnector * dbConnector;

  Config * config;

};

#endif
