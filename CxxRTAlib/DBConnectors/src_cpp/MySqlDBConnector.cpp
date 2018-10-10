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


bool MySqlDBConnector::connect(){

  #ifdef DEBUG
  cout << "Hostname: " << hostname << endl;
  cout << "Database: " << database << endl;
  cout << "Modelname: " << modelName << endl;
  cout << "Batchsize: " << batchsize << endl;
  cout << "Password: " << password << endl;
  #endif

  try {

    driver = sql::mysql::get_driver_instance();

    /* Using the Driver to create a connection */
    con = driver->connect (hostname, username, password); // connect to mysql

    con->setSchema(database);

    #ifdef DEBUG
    cout << "Connection ok!"<< endl;
    #endif




  } catch (sql::SQLException &e) {
    /*
    The MySQL Connector/C++ throws three different exceptions:

    - sql::MethodNotImplementedException (derived from sql::SQLException)
    - sql::InvalidArgumentException (derived from sql::SQLException)
    - sql::SQLException (derived from std::runtime_error)#include <chrono>
    */
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
    /* Use what() (derived from std::runtime_error) to fetch the error message */
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;

    return false;
  } catch (std::runtime_error &e) {

    cout << "# ERR: runtime_error in " << __FILE__;
    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
    cout << "# ERR: " << e.what() << endl;
    cout << "not ok 1 - examples/connect.php" << endl;

    return false;
  }

  return true;

}


int MySqlDBConnector::testConnection(){}

bool MySqlDBConnector::insertData(string modelName, map < string, string > args){

  insertDataCall ++;

  #ifdef DEBUG
  cout << "INSERT DATA FUNCTION" << endl;
  cout << "Batchsize: " << batchsize << endl;
  cout << "Modelname: " << modelName << endl;
  #endif

  string query = buildQuery(modelName, batchsize, args);

  if( batchsize == 1){

    inserted = streamingInsert(query);

    #ifdef DEBUG
    cout << "inserted value INSERT DATA FUNCTION: " << inserted << endl;
    #endif

  }else if(batchsize > 1){

    flagTransaction = 1;

    inserted = batchInsert(query, batchsize);

  }else{
    cout << "[MySqlConnector] Error, self.conn is None" << endl;
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



bool MySqlDBConnector::streamingInsert(string query) {

  #ifdef DEBUG
  cout << "[MySqlConnector] Streaming insert.." << endl;
  cout << query << endl;
  #endif

  int num_rows = 0;
  stringstream msg;

  try {

      boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

      /* executeUpdate() returns the number of affected = inserted rows */
      num_rows += stmt->executeUpdate(query);
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

      // return commandsSent;
      return false;
    } catch (std::runtime_error &e) {

      cout << endl;
      cout << "# ERR: runtime_error in " << __FILE__;
      cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
      cout << "# ERR: " << e.what() << endl;
      cout << "not ok 1 - MySqlDBConnector.cpp" << endl;

      commandsSent = 0;

      // return commandsSent;
      return false;
    }

    // return commandsSent;
    return true;

}

// bool MySqlDBConnector :: executeQuery(string query) {
//
//   int num_rows = 0;
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
//         cout << "Count = " << res->getString("COUNT(*)") << endl;
//         // count = res->getString("COUNT(*)");
//         row++;
//       }
//
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

bool MySqlDBConnector::batchInsert(string query, int batchsize) {

  boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

  if(commandsSent==0) {

    try {

      stmt->executeUpdate("START TRANSACTION");

      stmt->executeUpdate(query);

      strTrCall++;
      commandsSent++;

    }catch (sql::SQLException &e){

      cout << "Exeption START TRANSACTION" << endl;
      cout << "# ERR: " << e.what();

      return false;

    }

  }else if(commandsSent < batchsize) {

    try {

      stmt->executeUpdate(query);

      commandsSent++;

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

        // return EXIT_FAILURE;
        return false;
      } catch (std::runtime_error &e) {

        cout << endl;
        cout << "# ERR: runtime_error in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
        cout << "not ok 1 - MySqlDBConnector.cpp" << endl;

        // return EXIT_FAILURE;
        return false;
      }
  }else if(commandsSent >= (batchsize)){

    try{

      stmt->execute("COMMIT");
      commandsSent=0;
      commitCall++;
      batchInsert(query,batchsize);

    }catch(sql::SQLException &e){

      cout << "Exeption COMMIT TRANSACTION" << endl;
      cout << "# ERR: " << e.what();
      return false;
    }

  }


  #ifdef DEBUG
  cout << "commandsSent:" << commandsSent << endl;
  #endif

  // return commandsSent;
  return true;

}

string MySqlDBConnector::buildQuery(string modelName, int batchsize, map <string,string> args) {

  //cout << "BUILDQUERY FUNCTION" << endl;

  #ifdef DEBUG
  cout <<"Tablename: " << modelName << endl;
  #endif

  string query;

  string queryS = "INSERT INTO "+modelName;
  string queryH = "(";
  string queryV = "VALUES(";

  //cout << "\nQuery construnction ..." << endl;

  for (map<string,string>::iterator it=args.begin(); it!=args.end(); ++it) {

    queryH += it->first + ",";
    queryV += it->second + ",";

  }

  queryH = queryH.substr(0,queryH.size()-1);
  queryV = queryV.substr(0,queryV.size()-1);

  queryH += ") ";
  queryV += ")";

  query = queryS + queryH + queryV;

  #ifdef DEBUG
  cout << query << endl;
  #endif

  #ifdef DEBUG
  cout << "Querie construct!" << endl;
  #endif

  return query;

}


bool MySqlDBConnector::disconnect(){

  try
    {
        if(flagTransaction==1) {

          boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
          stmt->execute("COMMIT");
          commitCall++;
          commandsSent=0;

          #ifdef DEBUG
          cout << "Start transaction called " << strTrCall << " times." << endl;
          cout << "Commit transaction called " << commitCall << " times." << endl;
          #endif

        }
        con->close();
        delete con;
        #ifdef DEBUG
          cout << "Insert data function called " << insertDataCall << " times." << endl;
        #endif

        cout << "Disconnected to MySql!" << endl;
        return true;
    }
    catch(sql::SQLException &e)
    {
        cout << "# ERR: " << e.what() << endl;
        //CLogger::LogEvent("Failed To Close Connection to DataBase Server" ,e.what());
        return false;
    }
}
