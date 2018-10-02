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

#include"RTA_DL_DB.hpp"

RTA_DL_DB :: RTA_DL_DB(string database, string configFilePath){

  if (database != "mysql" && database != "redis-basic"){

    cout << "[RTA_DL_DB] Error! Database '{ " << database << " }' is not supported. Supported databases: \n- { mysql }\n- { redis-basic }" << endl;
    exit(EXIT_FAILURE);

  }

  config = Config::getIstance(configFilePath);

  // Synchronous (master thread) execution
  dbConnector = getConnector(database, configFilePath);
  dbConnector.connect();


}

DBConnector RTA_DL_DB :: getConnector(string databaseEngine, string configFilePath) {

  if(databaseEngine == "mysql"){

    return MySqlDBConnector(configFilePath);

  } else if(databaseEngine == "redis-basic"){

    return RedisDBConnector(configFilePath);

  }

}

int RTA_DL_DB :: _insertEvent( EVTTest event) {

  string modelname = config->file["General"]["modelname"].getString();

  map < string, string > eventData =  event.getData();

  cout << "\n[RTA_DL_DB] New event!" << endl;

  // Transform data for visualization and notify GUIs
  //  TODO

  // Synchronous (master thread)
  return dbConnector.insertData(modelname, eventData);

   // Multi threading mode
  //  TODO


}
