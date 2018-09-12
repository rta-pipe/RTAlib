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
#include "IniParser.hpp"
//#include "Iniparser.h"

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
  //
	// if(argc != 6) {
	// 	cout << "Insert 6 input parameters:\n"
	// 	"1 - hostname: Insert host name.\n"
	// 	"2 - username: Input database user id.\n"
	// 	"3 - user password: Input database user password\n"
	// 	"4 - database name: Insert database name.\n"
	// 	"5 - table name: Insert table name.\n"
	// 	 << endl;
  //
  //   cout << endString << endl;
	// 	return 0;
  //  }


/*  string value;
  //Iniparser::get("/home/zollino/RTAlib/PyRTAlib/rtalibconfig","MySql","username","password", value);
  IniFile file;

  // try loading file
  try {
    file = IniParser::load("/home/zollino/RTAlib/PyRTAlib/rtalibconfig");
  } catch (IniParser::ParserException e) {
    std::cerr << "Error while loading file!\n";
  }

  string hostname = file["MySql"]["host"].getString();
  string username = file["MySql"]["username"].getString();
  string password = file["MySql"]["password"].getString();
  string database = file["MySql"]["dbname"].getString();
  string tablename = file["General"]["evt3modelname"].getString();


  // printing errors
  std::cout << IniParser::getErrorString(IniParser::getError()) << "\n";
*/
  MySqlDBConnector * mysqlDBConnector = new MySqlDBConnector("/home/zollino/RTAlib/PyRTAlib");

  mysqlDBConnector->connect();

  int i = mysqlDBConnector->insertData(/*, args*/);

  int r = mysqlDBConnector->disconnect();

	cout << endString << endl;

  return 0;


}
