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
  DTRinChannel = config->file["Dtr"]["inputchannel"].getString();
  Mutex* mux = Mutex::getIstance();

  //countevts = 0;

  // DTR Configuratoion
  DTRactive = config->file["Dtr"]["active"].getString();
  if ( DTRactive.compare("yes") == 0 ){
    BoolDTRactive = true;
    cout << "DTR attivo" << endl;
    redisPub = make_shared<RedisPublisher>(configFilePath);
  }else{
    BoolDTRactive = false;
  }


  if( numberofthreads == 0 ) {

    if( BoolDTRactive) {
        redisPub = make_shared<RedisPublisher>(configFilePath);
    }

    // Synchronous (master thread) execution
    dbConnector = getConnector(0, database, configFilePath);

    if( dbConnector->connect(mux) == false ) {

      cout << "CXX_RTA_DL_X_DB Connection Error!" << endl;

      exit(EXIT_FAILURE);

    }

  }else if( numberofthreads >= 1) {

    // Multi threading mode
    eventBuffer = new CTABuffer("rta_dl_buffer",1000);


    // for( int i = 0; i < numberofthreads; i++ ) {
    //  // ThreadStatistic * ts = new ThreadStatistic(i);
    //  // thread_statistics_array.push_back(ts);
    // }

    for( int i = 0; i < numberofthreads; i++ ) {

      shared_ptr<RedisPublisher> redisPubThread = NULL;

      if(BoolDTRactive){
          redisPubThread = make_shared<RedisPublisher>(configFilePath);
      }

      dbConnector = getConnector(i, database, configFilePath);

      // ThreadStatistic * ts = thread_statistics_array[i];
      // ts->printThreadId();

      auto t = make_shared<RTAThread>(i, mux, modelname, dbConnector, eventBuffer, redisPubThread, BoolDTRactive, DTRinChannel);
      thread_array.push_back(t);

    }

  }



}

void RTA_DL_DB::start() {


  if( numberofthreads == 0 ) {

  }else {
    for( int i = 0; i < numberofthreads; i++ ) {
      // #ifdef DEBUG
      cout << "[RTA_DL_DB] Starting thread: " <<i << endl;
      // #endif
      thread_array[i]->start();
    }
  }

  starttime = Clock::now();

}


shared_ptr<DBConnector> RTA_DL_DB::getConnector(int id,string databaseEngine, string configFilePath) {

  if(databaseEngine == "mysql"){

    auto mySqlDBConnector = make_shared<MySqlDBConnector>(id,configFilePath);

    return mySqlDBConnector;

  } else if(databaseEngine == "redis-basic"){

    auto redisDBConnector = make_shared<RedisDBConnector>(id,configFilePath);

    return redisDBConnector;

  }

}

bool RTA_DL_DB::_insertEvent( EVTbase *event ) {

  // countevts++;

  string modelname = config->file["General"]["modelname"].getString();



  if( numberofthreads == 0 ) {

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
    delete event;
    bool ok = dbConnector->insertData(modelname, eventData);

    #ifdef DEBUG
    cout << "BoolDTRactive value: " << BoolDTRactive << endl;
    #endif

    // Transform data for visualization and notify GUIs
    if(BoolDTRactive){
      redisPub->Publish(DTRinChannel, eventData);
    }

    return ok;


  }else if( numberofthreads >= 1 ){

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

  if( numberofthreads >= 1 ) {

    for(int i=0; i < numberofthreads; i++ ) {
      EVTbase * stopEvent = new EVTbase(true);
      _insertEvent(stopEvent);
    }

    for(int i=0; i < numberofthreads; i++ ) {
      thread_array[i]->join();
      //endtime = std::chrono::system_clock::now();
      //std::chrono::duration<double> diff = endtime - starttime;
      //std::cout << "Took time: "<< diff.count() << " s\n";

      #ifdef DEBUG
      cout << "[RTA_DL_DB] thread "<< i << " joined" << endl;
      #endif
    }

    // cout << "[RTA_DL_DB] Collecting statistics.. " << endl;
    // for(int i=0; i < numberofthreads; i++ ) {
    //   // thread_statistics_array[i]->getStatistics();
    // }

  }else{

    //endtime = std::chrono::system_clock::now();
    //std::chrono::duration<double> diff = endtime -starttime;
    //std::cout << "Took time: "<< diff.count() << " s\n";
    auto elapsed = Clock::now() - starttime;
    cout <<"[RTAlib] took: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << "ms" << endl;

    if ( dbConnector->disconnect() == true ) {
      cout << "[RTA_DL_DB] disconnected" << endl;
      return true;
    }else {
      return false;
    }
  }
}

int RTA_DL_DB::getNumberOfThreads() {
  return numberofthreads;
}
