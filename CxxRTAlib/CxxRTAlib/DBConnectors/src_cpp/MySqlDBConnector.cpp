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



/*
If you don't know ahead of time whether the SQL statement will be a SELECT or an INSERT, UPDATE,
or DELETE, use the execute method.
execute() returns true if the SQL query was a SELECT, and returns false if the statement was an INSERT,
UPDATE, or DELETE.
If the statement was a SELECT query, you can retrieve the results by calling the getResultSet method
on the Statement instance. If the statement was an INSERT, UPDATE, or DELETE statement, you can retrieve
the count of affected rows by calling getUpdateCount().
https://dba.stackexchange.com/questions/24542/execute-vs-executequery-in-mysql-connector-c
https://stackoverflow.com/questions/27480741/which-execute-function-should-i-use-in-mysql-connector-c
*/


bool MySqlDBConnector::connect(Mutex* mux){

  #ifdef DEBUG
  cout << "[MySqlDBConnector "<<  idConnector <<"] Connecting.." << endl;
  cout << "Hostname connect: " << hostname << endl;
  cout << "Database connect: " << database << endl;
  cout << "Modelname connect: " << modelName << endl;
  cout << "Batchsize connect: " << batchsize << endl;
  cout << "Password connect: " << password << endl;
  #endif

  try {

    mux->mutexLock();
    driver = sql::mysql::get_driver_instance();
    mux->mutexUnlock();
    con = driver->connect (hostname, username, password);   /* Using the Driver to create a connection */
    con->setSchema(database);


    /*
    The MySQL Connector/C++ throws three different exceptions:

    - sql::MethodNotImplementedException (derived from sql::SQLException)
    - sql::InvalidArgumentException (derived from sql::SQLException)
    - sql::SQLException (derived from std::runtime_error)#include <chrono>
    */
  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
    /* Use what() (derived from std::runtime_error) to fetch the error message */
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;

    cout << "[MySqlDBConnector "<<  idConnector <<"] Connection FAILED!" << endl;

    return false;

  } catch (std::runtime_error &e) {
    cout << "# ERR: runtime_error in " << __FILE__;
    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
    cout << "# ERR: " << e.what() << endl;
    cout << "not ok 1 - examples/connect.php" << endl;

    cout << "[MySqlDBConnector "<<  idConnector <<"] Connection FAILED!" << endl;

    return false;
  }

  #ifdef DEBUG
  cout << "[MySqlDBConnector "<<  idConnector <<"] Connected" << endl;
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



bool MySqlDBConnector::streamingInsert(string query) {

  #ifdef DEBUG
    cout << "[MySqlDBConnector "<<  idConnector <<"] streaming Insert() " << endl;
  cout << query << endl;
  #endif

  stringstream msg;

  try {

      boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

      /* executeUpdate() returns the number of affected = inserted rows */
      int num_rows = stmt->executeUpdate(query);
      if (ONE_ROW != num_rows) {
        msg.str("");
        msg << "Expecting " << ONE_ROW << "rows, reported " << num_rows;
        throw runtime_error(msg.str());
      }

      commandsSent++;

    } catch (sql::SQLException &e) {
      /*
      The MySQL Connector/C++ throws three different exceptions:

      - sql::MethodNotImplementedException (derived from sql::SQLException)
      - sql::InvalidArgumentException (derived from sql::SQLException)
      - sql::SQLException (derived from std::runtime_error)
      */
      cout << endl;
      cout << "# ERR: DbcException in " << __FILE__;
      cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
      /* Use what(), getErrorCode() and getSQLState() */
      cout << "# ERR: " << e.what();
      cout << " (MySQL error code: " << e.getErrorCode();
      cout << ", SQLState: " << e.getSQLState() << " )" << endl;

      if (e.getErrorCode() == 1047) {
        /*
        Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
        Message: Unknown command
        */
        cout << "# ERR: Your server seems not to support PS at all because its MYSQL <4.1" << endl;
      }
      cout << "not ok 1 - MySqlDBConnector.cpp" << endl;

      commandsSent = 0;

      return false;

    } catch (std::runtime_error &e) {

      cout << endl;
      cout << "# ERR: runtime_error in " << __FILE__;
      cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
      cout << "# ERR: " << e.what() << endl;
      cout << "not ok 1 - MySqlDBConnector.cpp" << endl;

      commandsSent = 0;

      return false;

    }

    return true;

}


bool MySqlDBConnector::batchInsert(string query, int batchsize) {

  #ifdef DEBUG
  cout << "[MySqlConnector  " <<idConnector << "] batchInsert()" << endl;
  #endif

  boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

  if(commandsSent==0) {

    try {

      stmt->executeUpdate("START TRANSACTION");

    }catch (sql::SQLException &e){

      cout << "Exeption START TRANSACTION" << endl;
      cout << "# ERR: " << e.what();

      return false;

    }
  }


    try {

      stmt->executeUpdate(query);


    }catch (sql::SQLException &e) {
        /*
        The MySQL Connector/C++ throws three different exceptions:

        - sql::MethodNotImplementedException (derived from sql::SQLException)
        - sql::InvalidArgumentException (derived from sql::SQLException)
        - sql::SQLException (derived from std::runtime_error)
        */
        cout << endl;
        cout << "# ERR: DbcException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        /* Use what(), getErrorCode() and getSQLState() */
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;

        if (e.getErrorCode() == 1047) {
          /*
          Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
          Message: Unknown command
          */
          cout << "# ERR: Your server seems not to support PS at all because its MYSQL <4.1" << endl;
        }
        cout << "not ok 1 - MySqlDBConnector.cpp" << endl;

        return false;

      } catch (std::runtime_error &e) {

        cout << endl;
        cout << "# ERR: runtime_error in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
        cout << "not ok 1 - MySqlDBConnector.cpp" << endl;

        return false;

      }

      commandsSent++;

  if(commandsSent >= batchsize){

    try{

      stmt->executeUpdate("COMMIT");


    }catch(sql::SQLException &e){

      cout << "Exeption COMMIT TRANSACTION" << endl;

      cout << "# ERR: " << e.what();

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

string MySqlDBConnector::buildQuery(string modelName, int batchsize, map <string,string> args) {

  string query;

  string queryS = "INSERT INTO "+modelName;
  string queryH = "(";
  string queryV = "VALUES(";

  for (map<string,string>::iterator it=args.begin(); it!=args.end(); ++it) {

    queryH += it->first + ",";
    queryV += it->second + ",";

  }

  queryH = queryH.substr(0,queryH.size()-1);
  queryV = queryV.substr(0,queryV.size()-1);

  queryH += ") ";
  queryV += ")";

  query = queryS + queryH + queryV;

  return query;

}


bool MySqlDBConnector::disconnect(){

  try{
        if(flagTransaction==1) {

          boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

          stmt->execute("COMMIT");

          commandsSent=0;

        }

        con->close();

        delete con;

        #ifdef DEBUG
          // cout << "[MySqlConnector  " <<idConnector << "] Insert data function called " << insertDataCall << " times." << endl;
          cout << "[MySqlConnector  " <<idConnector << "]Disconnected to MySql!" << endl;
        #endif

        return true;

    }catch(sql::SQLException &e){

        cout << "[MySqlConnector  " <<idConnector << "] # ERR: " << e.what() << endl;

        return false;

    }

}


/* This function can execute query that return values
  For query without result have to write a similar function
  using:
    boost::scoped_ptr< sql::ResultSet > res(stmt->execute(query));
  instead:
    boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery(query));
*/
// bool MySqlDBConnector::executeQuery(string query) {
//
//   // int num_rows = 0;
//   stringstream msg;
//
//   try {
//
//       boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
//
//       boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery(query));
//
//       size_t row = 0;
//       while (res->next()) {
//
//         /// TODO handle query result
//         // cout << "observationid = " << res->getString("observationid") << endl;
//         // cout << "a = " << res->getInt(1) << endl;
//
//         // row++;
//       }
//
//       return true;
//
//     } catch (sql::SQLException &e) {
//       /*
//       The MySQL Connector/C++ throws three different exceptions:
//
//       - sql::MethodNotImplementedException (derived from sql::SQLException)
//       - sql::InvalidArgumentException (derived from sql::SQLException)
//       - sql::SQLException (derived from std::runtime_error)
//       */
//       cout << endl;
//       cout << "# ERR: DbcException in " << __FILE__;
//       cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
//       /* Use what(), getErrorCode() and getSQLState() */
//       cout << "# ERR: " << e.what();
//       cout << " (MySQL error code: " << e.getErrorCode();
//       cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//
//       if (e.getErrorCode() == 1047) {
//         /*
//         Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
//         Message: Unknown command
//         */
//         cout << "# ERR: Your server seems not to support PS at all because its MYSQL <4.1" << endl;
//       }
//       cout << "not ok 1 - MySqlDBConnector.cpp" << endl;
//
//       commandsSent = 0;
//
//       // return commandsSent;
//       return false;
//     } catch (std::runtime_error &e) {
//
//       cout << endl;
//       cout << "# ERR: runtime_error in " << __FILE__;
//       cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
//       cout << "# ERR: " << e.what() << endl;
//       cout << "not ok 1 - MySqlDBConnector.cpp" << endl;
//
//       commandsSent = 0;
//
//       // return commandsSent;
//       return false;
//     }
//
//     // return commandsSent;
//     return true;
//
// }
