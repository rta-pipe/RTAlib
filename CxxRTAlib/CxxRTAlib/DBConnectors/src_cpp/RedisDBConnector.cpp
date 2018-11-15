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

#include"RedisDBConnector.hpp"

bool RedisDBConnector::connect(Mutex* mux){

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Connecting.." << endl;
  #endif



  #ifdef DEBUG
  cout << "Hostname: " << hostname << endl;
  cout << "Database: " << database << endl;
  cout << "Batchsize: " << batchsize << endl;
  cout << "Indexon: " << indexon << endl;
  cout << "indexon_clean: " << indexon_clean << endl;
  #endif
  c = connection(idConnector, hostname.c_str(),password.c_str(),database.c_str());

  if( c != NULL ) {


    #ifdef DEBUG
    cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Connected" << endl;
    #endif

    return true;

  }else{

    cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Connection FAILED!" << endl;

    return false;

  }

}

bool RedisDBConnector::disconnect(){

  if ( close_connection(c, flagTransaction, idConnector)) {

    #ifdef DEBUG
    cout << "[RedisDBConnectorC++ "<<  idConnector <<"] disconnected." << endl;
    #endif

    return true;

  }else{

    return false;

  }


}


bool RedisDBConnector::insertData(string modelname, map < string, string > args){

  insertDataCall ++;

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Insert data()" << endl;
  cout << "Batchsize: " << batchsize << endl;
  cout << "Modelname: " << modelName << endl;
  cout << "idConnector: " << idConnector << endl;
  #endif

  string query = buildQuery( modelName, batchsize, args);

  if( batchsize == 1 ){

    inserted = streamingInsert(query);

    if(inserted ) {
      #ifdef DEBUG
      cout << "\n[RedisDBConnectorC++ " <<idConnector << "] event inserted." << endl;
      #endif
    } else {
      cout << "\n[RedisDBConnectorC++ " <<idConnector << "] Error: event NOT inserted." << endl;
    }

    #ifdef DEBUG
    cout << "[RedisDBConnectorC++ "<<  idConnector <<"] inserted value INSERT DATA FUNCTION: " << inserted << endl;
    #endif

  }else if(batchsize > 1){

    flagTransaction = 1;

    inserted = batchInsert(query, batchsize);

    if(inserted) {
       #ifdef DEBUG
       cout << "\n[RedisDBConnectorC++ " << idConnector << "] event inserted." << endl;
       #endif
    } else {
       cout << "\n[RedisDBConnectorC++ " << idConnector << "] Error: event NOT inserted." << endl;
    }

  }else{
    cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Batchsize cannot be lower than 0!!" << endl;
    return EXIT_FAILURE;
  }

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Command sent: " << commandsSent << " Insert data call: " << insertDataCall << endl;
  #endif

  if(inserted)
    return true;
  else
    return false;
}


bool RedisDBConnector::streamingInsert( string query){

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] streaming Insert() " << endl;
  #endif

  commandsSent = streamingInsert_c(c, idConnector, modelName.c_str(), score.c_str(), query.c_str());

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] CXX streaming commandsSent: " << commandsSent << endl;
  #endif

  return true;

}


bool RedisDBConnector::batchInsert(string query, int batchsize){


  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] batchInsert()" << endl;
  #endif

  if( batchInsert_c(commandsSent, c, idConnector, modelName.c_str(), score.c_str(), query.c_str(), batchsize) ){
    commandsSent++;
    #ifdef DEBUG
    cout << "[RedisDBConnectorC++ "<<  idConnector <<"] CXX batch commandsSent: " << commandsSent << endl;
    #endif
    return true;
  }
  else{
    // #ifdef DEBUG
    cout << "[RedisDBConnectorC++ "<<  idConnector <<"] CXX batch command NO Sent: " << commandsSent << endl;
    // #endif
    return false;
  }
}


string RedisDBConnector :: buildQuery( string modelName, int batchsize, map <string,string> args){

  string query = "'{";

  for (map<string,string>::iterator it=args.begin(); it!=args.end(); ++it) {

    query += it->first + " : " + it->second + ", ";

  }
  string finalQuery = query.substr(0,query.size()-2);

  finalQuery += " }'";

  score = args[indexon_clean];
  // cout << queryH << endl;

  return finalQuery;
}

bool RedisDBConnector::executeQuery(string query){

  executeQuery_c(c, idConnector, query.c_str());

}
