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

#include "MySqlDBConnector.hpp"

bool MySqlDBConnector::connect(Mutex* mux){

  #ifdef DEBUG
  cout << hostname << endl;
  cout << username<< endl;
  cout << database << endl;
  #endif

  string query ="SELECT table_name FROM information_schema.tables where table_schema='" + database+"'";

  try{

    // mux->mutexLock();
    mySession = make_shared<Session>( SessionOption::HOST, hostname,
                                      SessionOption::PORT, 33060,
                                      SessionOption::USER, username,
                                      SessionOption::PWD, password,
                                      SessionOption::DB, database);

                                      // mux->mutexUnlock();
    RowResult res = mySession->sql(query).execute();
    int c = res.count();
    if (c==0){
      cout <<"[MySqlDBConnector " <<idConnector << "] connect ERROR: No database selected"<<endl;
      return false;
    }




  }
  catch (const mysqlx::Error &err){
    cout <<"[MySqlDBConnector " <<idConnector << "] connect ERROR: " << err <<endl;
    return false;
  }catch (std::exception &ex){
    cout <<"[MySqlDBConnector " <<idConnector << "] connect STD EXCEPTION: " <<ex.what() <<endl;
    return false;
  }catch (const char *ex){
    cout <<"[MySqlDBConnector " <<idConnector << "] connect EXCEPTION: " <<ex <<endl;
    return false;
  }

  return true;

}

bool MySqlDBConnector::disconnect() {

    if(flagTransaction==1) {

      try{

        mySession->commit();

      }catch (const mysqlx::Error &err){
        cout <<"[MySqlDBConnector " <<idConnector << "] streaming ERROR: " << err <<endl;
        return false;
      }catch (std::exception &ex){
        cout <<"[MySqlDBConnector " <<idConnector << "] streaming STD EXCEPTION: " <<ex.what() <<endl;
        return false;
      }catch (const char *ex){
        cout <<"[MySqlDBConnector " <<idConnector << "] streaming EXCEPTION: " <<ex <<endl;
        return false;
      }


  }

    mySession->close();

    #ifdef DEBUG
    cout << "[MySqlConnector  " <<idConnector << "]Disconnected to MySql!" << endl;
    #endif

    return true;

}
string MySqlDBConnector::buildQuery(string modelName, int batchsize, map <string,string> args){

  string query;

  string queryS = "INSERT INTO "+modelName;
  string queryH = "(";
  string queryV = "VALUES(";

  for (map<string,string>::iterator it=args.begin(); it!=args.end(); ++it) {

    queryH += "`" + it->first + "`" + ",";
    queryV += it->second + ",";

  }

  queryH = queryH.substr(0,queryH.size()-1);
  queryV = queryV.substr(0,queryV.size()-1);


  queryH += ") ";
  queryV += ")";

  query = queryS + queryH + queryV;

  #ifdef DEBUG
  cout << "[MySqlDBConnector "<<  idConnector <<"] Query: \n" << query << endl;
  #endif

  return query;

}

bool MySqlDBConnector::streamingInsert(string query){

  #ifdef DEBUG
  cout << "[MySqlDBConnector "<<  idConnector <<"] streaming Insert() " << endl;
  cout << query << endl;
  #endif

  try{

    RowResult res = mySession->sql(query).execute();

    commandsSent++;

  }catch (const mysqlx::Error &err)
  {
    cout <<"[MySqlDBConnector " <<idConnector << "] streaming ERROR: " << err <<endl;
    commandsSent = 0;
    return false;
  }
  catch (std::exception &ex)
  {
    cout <<"[MySqlDBConnector " <<idConnector << "] streaming STD EXCEPTION: " <<ex.what() <<endl;
    commandsSent = 0;
    return false;
  }
  catch (const char *ex)
  {
    cout <<"[MySqlDBConnector " <<idConnector << "] streaming EXCEPTION: " <<ex <<endl;
    commandsSent = 0;
    return false;
  }

  return true;

}

bool MySqlDBConnector::batchInsert(string query, int batchsize){

  #ifdef DEBUG
  cout << "[MySqlConnector  " <<idConnector << "] batchInsert()" << endl;
  #endif

  if(commandsSent==0) {

    try {

      // Start a transaction
      mySession->startTransaction();

    }catch (const mysqlx::Error &err)
    {
      cout <<"[MySqlDBConnector " <<idConnector << "] start_transaction ERROR: " << err <<endl;
      commandsSent = 0;
      return false;
    }
    catch (std::exception &ex)
    {
      cout <<"[MySqlDBConnector " <<idConnector << "] start_transaction STD EXCEPTION: " <<ex.what() <<endl;
      commandsSent = 0;
      return false;
    }
    catch (const char *ex)
    {
      cout <<"[MySqlDBConnector " <<idConnector << "] start_transaction EXCEPTION: " <<ex <<endl;
      return false;
    }

  }


  try{

    RowResult res = mySession->sql(query).execute();


  }catch (const mysqlx::Error &err)
  {
    cout <<"[MySqlDBConnector " <<idConnector << "] streaming ERROR: " << err <<endl;
    commandsSent = 0;
    return false;
  }
  catch (std::exception &ex)
  {
    cout <<"[MySqlDBConnector " <<idConnector << "] streaming STD EXCEPTION: " <<ex.what() <<endl;
    commandsSent = 0;
    return false;
  }
  catch (const char *ex)
  {
    cout <<"[MySqlDBConnector " <<idConnector << "] streaming EXCEPTION: " <<ex <<endl;
    commandsSent = 0;
    return false;
  }

  commandsSent++;

  if(commandsSent >= batchsize){

    try{

      mySession->commit();


    }catch (const mysqlx::Error &err)
    {
      cout <<"[MySqlDBConnector " <<idConnector << "] streaming ERROR: " << err <<endl;
      commandsSent = 0;
      return false;
    }
    catch (std::exception &ex)
    {
      cout <<"[MySqlDBConnector " <<idConnector << "] streaming STD EXCEPTION: " <<ex.what() <<endl;
      commandsSent = 0;
      return false;
    }
    catch (const char *ex)
    {
      cout <<"[MySqlDBConnector " <<idConnector << "] streaming EXCEPTION: " <<ex <<endl;
      commandsSent = 0;
      return false;
    }

    commandsSent = 0;

    return true;

  }


  #ifdef DEBUG
  cout << "[MySqlConnector  " <<idConnector << "] CXX batch commandsSent:" << commandsSent << endl;
  #endif

  return true;

}

bool MySqlDBConnector::insertData(string modelName, map < string, string > args){

  insertDataCall ++;

  #ifdef DEBUG
  cout << "[MySqlDBConnector "<<  idConnector <<"] Insert data()" << endl;
  cout << "Batchsize: " << batchsize << endl;
  cout << "Modelname: " << modelName << endl;
  #endif

  string query = buildQuery( modelName, batchsize, args);

  if( batchsize == 1){

    inserted = streamingInsert( query);

    if(inserted==true) {
      #ifdef DEBUG
      cout << "\n[MySqlDBConnector " <<idConnector << "] event iserted." << endl;
      #endif
    } else {
      cout << "\n[MySqlDBConnector " <<idConnector << "] event NOT iserted." << endl;
    }

  }else if(batchsize > 1){

    flagTransaction = 1;

    inserted = batchInsert( query, batchsize);

    if(inserted==true) {
      #ifdef DEBUG
      cout << "\n[MySqlDBConnector " <<idConnector << "] event iserted." << endl;
      #endif
    } else {
      cout << "\n[MySqlDBConnector " <<idConnector << "] event NOT iserted." << endl;
    }

  }else{
    cout << "[MySqlConnector " <<idConnector << "] Error, self.conn is None" << endl;
    return EXIT_FAILURE;
  }

  #ifdef DEBUG
  cout << "[MySqlDBConnector  " <<idConnector << "] Command sent: " << commandsSent << " Insert data call: " << insertDataCall << endl;
  #endif


  if(inserted== true) {

    return true;

  }else{

    return false;

  }

}

bool MySqlDBConnector::executeQuery(string query){

    RowResult res = mySession->sql(query).execute();
    // cout << "Query executed" << endl;

    // Row row = res.fetchOne();
    // int cols =res.getColumnCount();
    // cout << "Numero di colonne: " << cols <<endl;
    // int rows =res.count();
    // cout << "Numero di righe: " << rows;
    //
    // for(int j=0; j<rows; j++){
    //   for(int i=0; i<cols; i++ ){
    //
    //     cout << row[i] <<  " ";
    //     if(i == cols-1){
    //       cout << "\n";
    //     }
    //   }
    //   if(j == row-1){
    //     cout << "\n";
    //   }
    // }

  // cout << "executeQuery quitting " << endl;
  return true;

}
