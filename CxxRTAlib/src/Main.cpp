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
#include <stdlib.h>
#include <iostream>
#include <utility>

#include "MySqlDBConnector.h"
#include "FileReader.h"

using std::cout;
using std::endl;
using std::string;
using std::make_pair;


const char* startString = {
"################################################################\n"
"###                   - test RTAlib/CXXRTAlib -              ###"
};

const char* endString = {
"### test RTAlib/CXXRTAlib exiting .......................... ###\n"
"################################################################"
};

int main(int argc, char *argv[]) {

  cout << startString << endl;

	if(argc != 6) {
		cout << "Insert 6 input parameters:\n"
		"1 - hostname: Insert host name.\n"
		"2 - username: Input database user id.\n"
		"3 - user password: Input database user password\n"
		"4 - database name: Insert database name.\n"
		"5 - table name: Insert table name.\n"
		 << endl;

    cout << endString << endl;
		return 0;
   }

  string hostname = argv[1];
  string username = argv[2];
  string password = argv[3];
  string database = argv[4];
  string tablename = argv[5];

  FileReader::readFromFile("/home/zollino/RTAlib/PyRTAlib/rtalibconfig");

  cout << "Acquisizione input: OK!" << endl;

  MySqlDBConnector * mysqlDBConnector = new MySqlDBConnector(hostname, username, password, database, tablename);

  mysqlDBConnector->connect(hostname, username, password, database);

  int i = mysqlDBConnector->insertData(tablename/*, args*/);

  int r = mysqlDBConnector->disconnect();

	cout << endString << endl;

  return 0;


}
