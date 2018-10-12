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

  cout << "RTA_DL_DB configFilePath: " << configFilePath << endl;

  config = Config::getIstance(configFilePath);


  // Synchronous (master thread) execution
  dbConnector = getConnector(database, configFilePath);

  if( dbConnector->connect() == false ) {

    cout << "CXX_RTA_DL_X_DB Connection Error!" << endl;

    exit(EXIT_FAILURE);

  }


}

DBConnector * RTA_DL_DB :: getConnector(string databaseEngine, string configFilePath) {

  if(databaseEngine == "mysql"){

    DBConnector * mySqlDBConnector = new MySqlDBConnector(configFilePath);

    return mySqlDBConnector;

  } else if(databaseEngine == "redis-basic"){

    DBConnector * redisDBConnector = new RedisDBConnector(configFilePath);

    return redisDBConnector;

  }

}

int RTA_DL_DB :: _insertEvent( EVTTest & event) {

  string modelname = config->file["General"]["modelname"].getString();

  map < string, string > eventData =  event.getData();

  #ifdef DEBUG
  cout << "DEBUG event print in RTA_DL_DB" << " \n" <<
          eventData["eventidfits"] << " \n" <<
          eventData["timerealtt"] << " \n"<<
          eventData["ra_deg"] << " \n"<<
          eventData["dec_deg"] << " \n"<<
          eventData["energy"] << " \n"<<
          eventData["detx"] << " \n"<<
          eventData["dety"] << " \n"<<
          eventData["observationid"] << " \n"<<
          eventData["datarepositoryid"] << " \n"<<
          eventData["status"] << " \n"<<
          eventData["mcid"] << " \n"<<
          eventData["insert_time"]  << endl;
  #endif

  // cout << "\n[RTA_DL_DB] New event!" << endl;

  // Transform data for visualization and notify GUIs
  //  TODO

  if( numberofthreads == 1 ) {

    // Synchronous (master thread)
    return dbConnector->insertData(modelname, eventData);

  }else if( numberofthreads >1 ){

    // Multi threading mode
    //  TODO
  }





}

bool RTA_DL_DB :: waitAndClose() {

  #ifdef DEBUG
  cout << "waitAndClose" << endl;
  #endif

  if ( dbConnector->disconnect() == true ) {

    return true;

  }else {

    return false;

  }

}
