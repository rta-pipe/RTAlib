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

RTA_DL_DB::RTA_DL_DB(string database, string configFilePath){

  if (database != "mysql" && database != "redis-basic"){

    cout << "[RTA_DL_DB] Error! Database '{ " << database << " }' is not supported. Supported databases: \n- { mysql }\n- { redis-basic }" << endl;
    exit(EXIT_FAILURE);

  }

  #ifdef DEBUG
  cout << "RTA_DL_DB configFilePath: " << configFilePath << endl;
  #endif

  config = Config::getIstance(configFilePath);

  numberofthreads = config->file["General"]["numberofthreads"].getInt();
  modelname = config->file["General"]["modelname"].getString();
  Mutex* mux = Mutex::getIstance();


  if( numberofthreads == 1 ) {

      // Synchronous (master thread) execution
      dbConnector = getConnector(0, database, configFilePath);

      if( dbConnector->connect(mux) == false ) {

        cout << "CXX_RTA_DL_X_DB Connection Error!" << endl;

        exit(EXIT_FAILURE);

      }

  }else if( numberofthreads > 1) {

    // Multi threading mode
    eventBuffer = new CTABuffer("rta_dl_buffer",1000);
    cout << "Number of threads: " << numberofthreads << endl;


    // for( int i = 0; i < numberofthreads; i++ ) {
    //  // ThreadStatistic * ts = new ThreadStatistic(i);
    //  // thread_statistics_array.push_back(ts);
    // }

    for( int i = 0; i < numberofthreads; i++ ) {
      dbConnector = getConnector(i, database, configFilePath);

      // ThreadStatistic * ts = thread_statistics_array[i];
      // ts->printThreadId();

      Thread * t = new RTAThread(i, mux, modelname, dbConnector, eventBuffer); //, ts
      thread_array.push_back(t);

    }

  }



}

void RTA_DL_DB::start() {
  if( numberofthreads == 1 ) {

  }else {
    for( int i = 0; i < numberofthreads; i++ ) {
      //sleep(0.5);
      #ifdef DEBUG
      cout << "[RTA_DL_DB] Starting thread: " <<i << endl;
      #endif
      thread_array[i]->start();
    }
  }
}

DBConnector * RTA_DL_DB::getConnector(int id, string databaseEngine, string configFilePath) {

  if(databaseEngine == "mysql"){

    DBConnector * mySqlDBConnector = new MySqlDBConnector(id,configFilePath);

    return mySqlDBConnector;

  } else if(databaseEngine == "redis-basic"){

    DBConnector * redisDBConnector = new RedisDBConnector( id,configFilePath);

    return redisDBConnector;

  }

}

int RTA_DL_DB::_insertEvent( EVTbase *event ) {

  string modelname = config->file["General"]["modelname"].getString();

  // cout << "\n[RTA_DL_DB] New event!" << endl;

  // Transform data for visualization and notify GUIs
  //  TODO

  if( numberofthreads == 1 ) {

    map < string, string > eventData =  event->getData();

    // #ifdef DEBUG
    // cout << "DEBUG event print in RTA_DL_DB" << " \n" <<
    //         eventData["eventidfits"] << " \n" <<
    //         eventData["timerealtt"] << " \n"<<
    //         eventData["ra_deg"] << " \n"<<
    //         eventData["dec_deg"] << " \n"<<
    //         eventData["energy"] << " \n"<<
    //         eventData["detx"] << " \n"<<
    //         eventData["dety"] << " \n"<<
    //         eventData["observationid"] << " \n"<<
    //         eventData["datarepositoryid"] << " \n"<<
    //         eventData["status"] << " \n"<<
    //         eventData["mcid"] << " \n"<<
    //         eventData["insert_time"]  << endl;
    // #endif

    // Synchronous (master thread)
    return dbConnector->insertData(modelname, eventData);


  }else if( numberofthreads > 1 ){

    // Multi threading mode
    eventBuffer->put(event);
    #ifdef DEBUG
    cout<< "\n[RTA_DL_DB] Event inserted in buffer" << endl;
    #endif

    return 1;

  }

}


bool RTA_DL_DB::waitAndClose() {

  #ifdef DEBUG
  cout << "\n[RTA_DL_DB] waitAndClose" << endl;
  #endif

  if( numberofthreads > 1 ) {

    for(int i=0; i < numberofthreads; i++ ) {
      EVTbase * stopEvent = new EVTbase(true);
      _insertEvent(stopEvent);
    }

    for(int i=0; i < numberofthreads; i++ ) {
      thread_array[i]->join();
      #ifdef DEBUG
      cout << "[RTA_DL_DB] thread "<< i << " joined" << endl;
      #endif
    }

    // cout << "[RTA_DL_DB] Collecting statistics.. " << endl;
    // for(int i=0; i < numberofthreads; i++ ) {
    //   // thread_statistics_array[i]->getStatistics();
    // }

  }else{

    if ( dbConnector->disconnect() == true ) {
      #ifdef DEBUG
      cout << "[RTA_DL_DB] disconnect" << endl;
      #endif
      return true;

    }else {

      return false;

    }

  }

}

int RTA_DL_DB::getNumberOfThreads() {
  return numberofthreads;
}
