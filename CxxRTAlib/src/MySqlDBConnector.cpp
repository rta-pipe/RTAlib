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
    con.reset(driver->connect (hostname, username, password)); // connect to mysql

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

int MySqlDBConnector::startTransaction(){

  /* Creating a "simple" statement - "simple" = not a prepared statement */
	boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

  stmt->execute("START TRANSACTION");

}

int MySqlDBConnector::commitTransaction(){

  /* Creating a "simple" statement - "simple" = not a prepared statement */
	boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

  stmt->execute("COMMIT");

}


int MySqlDBConnector::insertData(/*, vector < pair <string,double> > args*/){

  cout << "INSERT DATA FUNCTION" << endl;

  string modelName = config->file["General"]["evt3modelname"].getString();

  int batchsize = config->file["General"]["batchsize"].getInt();  // DA LEGGERE DA rtaconfig

  //cout << "Modelname: " << modelName << endl;

  vector<string> query = buildQuery(modelName, batchsize/*, args*/);

  if( batchsize == 1) /* da rtaconfig batchsize*/

    streamingInsert(query);

  else if(batchsize > 1)

    batchInsert(query);

  else
    cout << "[MySqlConnector] Error, self.conn is None" << endl;

    return EXIT_FAILURE;

}



int MySqlDBConnector::streamingInsert(vector<string> query) {

  cout << "[MySqlConnector] Streaming insert.." << endl;
  int num_rows = 0;
  stringstream msg;
  string qr;

  try {

      boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

      /* executeUpdate() returns the number of affected = inserted rows */
      /*num_rows += stmt->executeUpdate(query[0]);
      if (ONE_ROW != num_rows) {
        msg.str("");
        msg << "Expecting " << ONE_ROW << "rows, reported " << num_rows;
        throw runtime_error(msg.str());
      }*/

      for(vector<string>::iterator it = query.begin(); it!=query.end(); it++) {

        /* executeUpdate() returns the number of affected = inserted rows */
        num_rows += stmt->executeUpdate(*it);
        if (ONE_ROW != num_rows) {
          msg.str("");
          msg << "Expecting " << ONE_ROW << "rows, reported " << num_rows;
          throw runtime_error(msg.str());
        }

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

int MySqlDBConnector::batchInsert(vector<string> query) {

  int commandsSent = 0;
  int num_rows = 0;
  stringstream msg;

  int batchsize =1; // DA LEGGERE DA rtaconfig

  cout << "[MySqlConnector] Batch insert.." << endl;

  int s = startTransaction();

  //  START STREAMING INSERT

/*  try {

      boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

      for(vector<string>::iterator it = query.begin(); it!=query.end(); it++) {

        /* executeUpdate() returns the number of affected = inserted rows */
/*        num_rows += stmt->executeUpdate(*it);
        if (ONE_ROW != num_rows) {
          msg.str("");
          msg << "Expecting " << ONE_ROW << "rows, reported " << num_rows;
          throw runtime_error(msg.str());
        }

    }

    } catch (sql::SQLException &e) {
      /*
      The MySQL Connector/C++ throws three different exceptions:

      - sql::MethodNotImplementedException (derived from sql::SQLException)
      - sql::InvalidArgumentException (derived from sql::SQLException)
      - sql::SQLException (derived from std::runtime_error)
      */
/*      cout << endl;
      cout << "# ERR: DbcException in " << __FILE__;
      cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
      /* Use what(), getErrorCode() and getSQLState() */
/*      cout << "# ERR: " << e.what();
      cout << " (MySQL error code: " << e.getErrorCode();
      cout << ", SQLState: " << e.getSQLState() << " )" << endl;

      if (e.getErrorCode() == 1047) {
        /*
        Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
        Message: Unknown command
        */
/*        cout << "# ERR: Your server seems not to support PS at all because its MYSQL <4.1" << endl;
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
*/

    streamingInsert(query);

  // END STREAMING INSERT

  int c = commitTransaction();

}

vector<string> MySqlDBConnector::buildQuery(string modelName, int batchsize/*, vector < pair <string,double> > args*/) {

  cout << "BUILDQUERY FUNCTION" << endl;

  cout <<"Tablename: " << modelName << endl;

  string insertQueriesArray[batchsize];
  vector<string> allQueries;
  //double **dataWR = data;
  int status = 0;

  cout << "\nQuery construnction ..." << endl;

  time_t seconds;
  seconds = time(NULL);

  for(int i = 0 ; i < batchsize; i ++) {

  insertQueriesArray[i] = "INSERT INTO " + modelName + "(eventidfits, observationid, datarepositoryid, ra_deg, dec_deg, energy, detx, dety, mcid, status, timerealtt, insert_time) VALUES (1,2,3,4,5,6,7,8,9,10,11,12)";
  /*insertQueriesArray[i] = "INSERT INTO " + modelName + "(eventidfits, observationid, datarepositoryid, ra_deg, dec_deg, energy, detx, dety, mcid, status, timerealtt, insert_time) VALUES ("

  	    + FileWriter::convertToString(dataWR[i][0]) + " ,"

  	    + FileWriter::convertToString( idObs ) + " , "

  	    + FileWriter::convertToString( idRepo ) + " ,"

  	    + FileWriter::convertToString( dataWR[i][2] ) + " ,"

  	    + FileWriter::convertToString( dataWR[i][3] ) + ", "

  	    + FileWriter::convertToString( dataWR[i][4] ) + " , "

  	    + FileWriter::convertToString( dataWR[i][5] ) + " ,"

  	    + FileWriter::convertToString( dataWR[i][6] ) + " ,"

  	    + FileWriter::convertToString( dataWR[i][7] ) + " ,"

  	    + FileWriter::convertToString( status ) + " , "

  	    + FileWriter::convertToString( dataWR[i][1] ) + " , "

        + FileWriter::convertToString( seconds ) + ")";*/

        allQueries.push_back(insertQueriesArray[i]);

  }

  cout << "All queries are constructed." << endl;

  return allQueries;

}

int MySqlDBConnector::disconnect(){

  try
    {
        con->close();
        //delete con;
        //this->driver->threadEnd();
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
