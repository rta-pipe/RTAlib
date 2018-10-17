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

bool RedisDBConnector::connect(){

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Connecting.." << endl;
  #endif

  int dp = indexon.find(":");
  indexon_clean = indexon.substr(dp+1,indexon.size());

  // #ifdef DEBUG
  // cout << "Hostname: " << hostname << endl;
  // cout << "Database: " << database << endl;
  // cout << "Batchsize: " << batchsize << endl;
  // cout << "Indexon: " << indexon << endl;
  // cout << "indexon_clean: " << indexon_clean << endl;
  // #endif

  if( connection(idConnector, hostname.c_str(),password.c_str(),database.c_str()) ) {

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

  if ( close_connection(idConnector)) {

    // #ifdef DEBUG
    cout << "[RedisDBConnectorC++ "<<  idConnector <<"] disconnecting.." << endl;
    // #endif

    return true;

  }else{

    return false;

  }


}


bool RedisDBConnector :: insertData(string modelname, map < string, string > args){

  insertDataCall ++;

  // #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Insert data()" << endl;
  // cout << "Batchsize: " << batchsize << endl;
  // cout << "Modelname: " << modelName << endl;
  // cout << "idConnector: " << idConnector << endl;
  // #endif

  string query = buildQuery( modelName, batchsize, args);

  if( batchsize == 1){

    // commandsSent = streamingInsert(query);
    inserted = streamingInsert(query);

    if(inserted ) {
      cout << "\n[RedisDBConnectorC++ " <<idConnector << "] event inserted." << endl;
    } else {
      cout << "\n[RedisDBConnectorC++ " <<idConnector << "] event NOT inserted." << endl;
    }

    // #ifdef DEBUG
    //cout << "[RedisDBConnectorC++ "<<  idConnector <<"] inserted value INSERT DATA FUNCTION: " << inserted << endl;
    // #endif

  }else if(batchsize > 1){

    flagTransaction = 1;

    // commandsSent = batchInsert(query, batchsize);
     inserted = batchInsert( query, batchsize);

  }else{
    cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Error, self.conn is None" << endl;
    return EXIT_FAILURE;
  }

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] Command sent: " << commandsSent << " Insert data call: " << insertDataCall << endl;
  #endif

  if(inserted== true) {

    return true;

  }else{

    return false;

  }

}


int RedisDBConnector :: streamingInsert( string query){

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] streaming Insert() " << endl;
  #endif

  commandsSent = streamingInsert_c(idConnector, modelName.c_str(), score.c_str(), query.c_str());

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] CXX streaming commandsSent: " << commandsSent << endl;
  #endif

  return commandsSent;

}


int RedisDBConnector :: batchInsert( string query, int batchsize){

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] batchInsert()" << endl;
  #endif

  commandsSent = batchInsert_c(idConnector, modelName.c_str(), score.c_str(),query.c_str(),batchsize);

  #ifdef DEBUG
  cout << "[RedisDBConnectorC++ "<<  idConnector <<"] CXX batch commandsSent: " << commandsSent << endl;
  #endif

  return commandsSent;

}


string RedisDBConnector :: buildQuery( string modelName, int batchsize, map <string,string> args){



  string query;

  string queryH = "'{";

  for (map<string,string>::iterator it=args.begin(); it!=args.end(); ++it) {

    queryH += it->first + " : " + it->second + ", ";

  }

  queryH += "}'";

  score = args[indexon_clean];



  return queryH;
}
