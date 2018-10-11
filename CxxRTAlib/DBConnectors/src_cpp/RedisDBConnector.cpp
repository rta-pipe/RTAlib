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
  cout << "REDISDBCONNECTOR CONNECT" << endl;
  #endif

  int dp = indexon.find(":");
  indexon_clean = indexon.substr(dp+1,indexon.size());

  #ifdef DEBUG
  cout << "Hostname: " << hostname << endl;
  cout << "Database: " << database << endl;
  cout << "Batchsize: " << batchsize << endl;
  cout << "Indexon: " << indexon << endl;
  cout << "indexon_clean: " << indexon_clean << endl;
  #endif

  if( connection(hostname.c_str(),password.c_str(),database.c_str()) == 0 ) {

    return true;

  }else{

    return false;

  }

}

bool RedisDBConnector :: disconnect(){

  if (close_connection() == 0) {

    cout << "REDISDBCONNECTOR DISCONNECT" << endl;

    return true;

  }else{

    return false;

  }


}

int RedisDBConnector :: testConnection(){

}

bool RedisDBConnector :: insertData(string modelname, map < string, string > args){

  insertDataCall ++;

  #ifdef DEBUG
  cout << "INSERT DATA FUNCTION" << endl;
  cout << "Batchsize: " << batchsize << endl;
  cout << "Modelname: " << modelName << endl;
  #endif

  string query = buildQuery(modelName, batchsize, args);

  if( batchsize == 1){

    // commandsSent = streamingInsert(query);
    inserted = streamingInsert(query);

    #ifdef DEBUG
    cout << "inserted value INSERT DATA FUNCTION: " << inserted << endl;
    #endif

  }else if(batchsize > 1){

    flagTransaction = 1;

    // commandsSent = batchInsert(query, batchsize);
     inserted = batchInsert(query, batchsize);

  }else{
    cout << "[RedisConnector] Error, self.conn is None" << endl;
    return EXIT_FAILURE;
  }

  #ifdef DEBUG
  cout << "Command sent: " << commandsSent << " Insert data call: " << insertDataCall << endl;
  #endif

  if(inserted== true) {

    return true;

  }else{

    return false;

  }
  // return insertDataCall;

}


int RedisDBConnector :: streamingInsert(string query){

  #ifdef DEBUG
  cout << "STREAMING" << endl;
  #endif

  commandsSent = streamingInsert_c(modelName.c_str(), score.c_str(), query.c_str());

  #ifdef DEBUG
  cout << "RedisDBConnector CXX streaming commandsSent: " << commandsSent << endl;
  #endif

  return commandsSent;

}


int RedisDBConnector :: batchInsert(string query, int batchsize){

  #ifdef DEBUG
  cout << "BATCHSIZE" << endl;
  #endif

  commandsSent = batchInsert_c(modelName.c_str(), score.c_str(),query.c_str(),batchsize);

  #ifdef DEBUG
  cout << "RedisDBConnector CXX batch commandsSent: " << commandsSent << endl;
  #endif

  return commandsSent;

}


string RedisDBConnector :: buildQuery(string modelName, int batchsize, map <string,string> args){

  #ifdef DEBUG
  cout <<"Tablename: " << modelName << endl;
  #endif

  string query;

  string queryH = "'{";

  for (map<string,string>::iterator it=args.begin(); it!=args.end(); ++it) {

    queryH += it->first + " : " + it->second + ", ";

  }

  queryH = queryH.substr(0,queryH.size()-2);

  int p = queryH.find(indexon_clean);

  int q = queryH.find(":", p);

  int e = queryH.find("}", q);

  if ( e == -1) {

    e = queryH.find(",", q);

  }

  score = queryH.substr(q+2, e-q-2);//queryH.size()-1

  #ifdef DEBUG
  cout << "p value: " << p << endl;
  cout << "q value: " << q << endl;
  cout << "e value: " << e << endl;
  cout << "len: " << e-q << endl;
  cout << "Score: " << score << endl;
  #endif

  queryH += "}'";

  #ifdef DEBUG
  cout << queryH << endl;
  #endif

  #ifdef DEBUG
  cout << "Querie construct!" << endl;
  #endif


  return queryH;
}
