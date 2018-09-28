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
#include <map>

#include "MySqlDBConnector.h"
#include "FileReader.h"
#include "IniParser.hpp"
//#include "Iniparser.h"

using std::cout;
using std::endl;
using std::string;
//using std::make_pair;


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

  std::map <string, string> args;


  args["eventidfits"] ="50";
  args["observationid"] ="60";
  args["datarepositoryid"] ="70";
  args["ra_deg"] ="80";
  args["dec_deg"] ="90";
  args["energy"] ="100";
  args["detx"] ="110";
  args["dety"] ="120";
  args["mcid"] ="1";
  args["status"] ="1";
  args["timerealtt"] ="130";
  args["insert_time"] ="140";


  MySqlDBConnector * mysqlDBConnector = new MySqlDBConnector("/home/zollino/RTAlib/PyRTAlib");

  mysqlDBConnector->connect();

  int i = mysqlDBConnector->insertData(args);

  int r = mysqlDBConnector->disconnect();

	cout << endString << endl;

  return 0;


}
