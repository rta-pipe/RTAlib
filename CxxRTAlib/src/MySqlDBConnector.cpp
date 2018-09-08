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

#include"MySqlDBConnector.h"

using std::cout;
using std::endl;


/*MySqlDBConnector :: MySqlDBConnector(string _hostname, string _username, string _password, string _database, string _tablename){

  string hostname = _hostname;
  string username = _username;
  string password = _password;
  string database = _database;
  string tablename = _tablename;
  con = mysql_init(NULL);

  cout << "MySqlDBConnector"  << endl;
  cout << "Hostname: " << hostname << endl;
  cout << "User: " << username << endl;
  cout << "Password: " << password << endl;
  cout << "Database: " << database << endl;
  cout << "Table name: " << tablename << endl;

}*/

int MySqlDBConnector::connect(string hostname, string username, string password, string database){

  con = mysql_init(NULL);

  if (!con) {
    cout << "Init faild, out of memory?" << endl;
    return EXIT_FAILURE;
  }

  cout << "\nMYSQLDBCONNECTOR CONNECT" << endl;
  cout << hostname.c_str() << endl;
  cout << username.c_str() << endl;
  cout << password.c_str() << endl;
  cout << database.c_str() << endl;

  if (!mysql_real_connect(con,       /* MYSQL structure to use */
			  hostname.c_str(),            /* server hostname or IP address */
			  username.c_str(),            /* mysql user */
			  password.c_str(),            /* password */
			  database.c_str(),            /* default database to use, NULL for none */
			  0,                           /* port number, 0 for default */
			  NULL,                        /* socket file or named pipe name */
        CLIENT_FOUND_ROWS)) {        /* connection flags */
    cout << "Connect failed" << endl;
  } else {
    cout << "Connect OK!" << endl;
  }
}


string MySqlDBConnector :: buildQuery(string modelName/*, vector < pair <string,double> > args*/) {

  cout << "BUILDQUERY FUNCTION" << endl;

  //cout <<"Tablename: " << modelName << endl;

  string query = "INSERT INTO " + modelName + "(eventidfits, observationid, datarepositoryid, ra_deg, dec_deg, energy, detx, dety, mcid, status, timerealtt, insert_time) VALUES(1,2,3,4,5,6,7,8,9,10,11,12)";


  return query;
}


int MySqlDBConnector :: insertData(string modelName/*, vector < pair <string,double> > args*/){

  cout << "INSERT DATA FUNCTION" << endl;

  int batchsize = 1;  // DA LEGGERE DA rtaconfig

  //cout << "Modelname: " << modelName << endl;

  string query = buildQuery(modelName/*, args*/);

  if( batchsize == 1) /* da rtaconfig batchsize*/

    streamingInsert(query);

  else if(batchsize > 1)

    batchInsert(query);

  else
    cout << "[MySqlConnector] Error, self.conn is None" << endl;

    return EXIT_FAILURE;


}

int MySqlDBConnector :: testConnection(){

 }

int MySqlDBConnector :: streamingInsert(string query) {

  cout << "[MySqlConnector] Streaming insert.." << endl;

  if (mysql_real_query( con, query.c_str(), strlen( query.c_str() ) ) ) {

    cout << query.c_str() << endl;

    cout << "Query failed: " << mysql_error(con) << endl;


  } else {

    cout << "Query OK!" << endl;


  }

}

int MySqlDBConnector :: batchInsert(string query) {

  int commandsSent = 0;

  int batchsize =1; // DA LEGGERE DA rtaconfig

  cout << "[MySqlConnector] Batch insert.." << endl;

  if(commandsSent==0) {

     mysql_autocommit(con, 1);  //START TRANSACTION

     commandsSent++;


  }

  if (mysql_real_query( con, query.c_str(), strlen( query.c_str() ) ) ) {

    cout << query.c_str() << endl;

    cout << "Query failed: " << mysql_error(con) << endl;


  } else {

    cout << "Query OK!" << endl;


  }


  //SE COMMAND SENT => BATCHSIZE EXECUTE COMMIT E COMMAND SENT = 0
  if(commandsSent >= batchsize) {
    mysql_commit(con);
    commandsSent = 0;
  }


}

int MySqlDBConnector :: disconnect(){

  mysql_close(con);

  printf("Disconnect to MySql.\n");

  return EXIT_SUCCESS;

}
