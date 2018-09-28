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


#include "MySqlDBConnector.h"


int MySqlDBConnector::connect(){

  string hostname = config->file["MySql"]["host"].getString();
  string username = config->file["MySql"]["username"].getString();
  string password = config->file["MySql"]["password"].getString();
  string database = config->file["MySql"]["dbname"].getString();

  try {

    driver = sql::mysql::get_driver_instance();

    /* Using the Driver to create a connection */
    //con.reset(driver->connect (hostname, username, password)); // connect to mysql
    con = driver->connect (hostname, username, password); // connect to mysql

    con->setSchema(database);

    cout << "Connection ok!"<< endl;


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

    return EXIT_FAILURE;
  } catch (std::runtime_error &e) {

    cout << "# ERR: runtime_error in " << __FILE__;
    cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
    cout << "# ERR: " << e.what() << endl;
    cout << "not ok 1 - examples/connect.php" << endl;

    return EXIT_FAILURE;
  }


  cout << endl;

  cout << endl;
  return EXIT_SUCCESS;

}


int MySqlDBConnector::testConnection(){}

int MySqlDBConnector::insertData(map < string, string > args){

  cout << "INSERT DATA FUNCTION" << endl;

  string modelName = config->file["General"]["evt3modelname"].getString();
  int batchsize = config->file["General"]["batchsize"].getInt();

  #ifdef DEBUG
  cout << "Batchsize: " << batchsize << endl;
  cout << "Modelname: " << modelName << endl;
  #endif


  string query = buildQuery(modelName, batchsize, args);

  if( batchsize == 1)

    streamingInsert(query);

  else if(batchsize > 1)

    batchInsert(query, batchsize);

  else
    cout << "[MySqlConnector] Error, self.conn is None" << endl;

    return EXIT_FAILURE;

}



int MySqlDBConnector::streamingInsert(string query) {

  cout << "[MySqlConnector] Streaming insert.." << endl;
  int num_rows = 0;
  stringstream msg;

  try {

      boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

      /* executeUpdate() returns the number of affected = inserted rows */
      // num_rows += stmt->executeUpdate(*it);
      num_rows += stmt->executeUpdate(query);
      if (ONE_ROW != num_rows) {
        msg.str("");
        msg << "Expecting " << ONE_ROW << "rows, reported " << num_rows;
        throw runtime_error(msg.str());
      }

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
      cout << "not ok 1 - RTA-DL3-Sim/DBConnector.cpp" << endl;

      return EXIT_FAILURE;
    } catch (std::runtime_error &e) {

      cout << endl;
      cout << "# ERR: runtime_error in " << __FILE__;
      cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
      cout << "# ERR: " << e.what() << endl;
      cout << "not ok 1 - RTA-DL3-Sim/DBConnector.cpp" << endl;

      return EXIT_FAILURE;
    }

}

int MySqlDBConnector::batchInsert(string query, int batchsize) {

  boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

  cout << "[MySqlConnector]: Batch insert.." << endl;

  if(commandsSent==0) {

    stmt->execute("START TRANSACTION");

  }else if(commandsSent!=batchsize) {

    try {


    stmt->execute(query);

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
        cout << "not ok 1 - RTA-DL3-Sim/DBConnector.cpp" << endl;

        return EXIT_FAILURE;
      } catch (std::runtime_error &e) {

        cout << endl;
        cout << "# ERR: runtime_error in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
        cout << "not ok 1 - RTA-DL3-Sim/DBConnector.cpp" << endl;

        return EXIT_FAILURE;
      }
  }else if(commandsSent==batchsize){

    stmt->execute("COMMIT");
    commandsSent=0;

  }

  #ifdef DEBUG
  cout << "commandsSent:" << commandsSent << endl;
  #endif

}

string MySqlDBConnector::buildQuery(string modelName, int batchsize, map <string,string> args) {

  cout << "BUILDQUERY FUNCTION" << endl;

  #ifdef DEBUG
  cout <<"Tablename: " << modelName << endl;
  #endif

  string query;

  string queryS = "INSERT INTO "+modelName;
  string queryH = "(";
  string queryV = "VALUES(";

  cout << "\nQuery construnction ..." << endl;

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

int MySqlDBConnector::disconnect(){

  try
    {
        con->close();
        cout << "Disconnected to MySql!" << endl;
        return 0;
    }
    catch(sql::SQLException &e)
    {
        cout << "# ERR: " << e.what() << endl;
        //CLogger::LogEvent("Failed To Close Connection to DataBase Server" ,e.what());
        return -1;
    }
}
