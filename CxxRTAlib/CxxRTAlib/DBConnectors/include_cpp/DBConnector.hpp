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
#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H


/* Standard C++ includes */
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <memory>

#include "Config.hpp"
#include "Mutex.hpp"

using std::string;
using std::pair;
using std::vector;
using std::cout;
using std::endl;
using std::map;
using std::make_shared;
using std::shared_ptr;
using std::unique_ptr;

class DBConnector {
public:
  DBConnector(string filepath="");
  virtual bool connect(Mutex* mux);
  virtual bool disconnect();
  virtual bool insertData(string modelName, map < string, string > args);
  virtual bool executeQuery(string query);
  // virtual bool selectData(string query);

  Config * config;
};

#endif
