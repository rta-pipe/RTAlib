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

#include"RTA_DLTEST_DB.hpp"

const char* startString = {
"################################################################\n"
"###                   - RTAlib/CXXRTAlib -                   ###"
};

const char* endString = {
"### test RTAlib/CXXRTAlib exiting .......................... ###\n"
"################################################################"
};

int main(int argc, char *argv[]) {

  cout << startString << endl;

  string database = argv[1];

  string configFilePath = argv[2];

  cout << "Database: " << database << endl;

  cout << "configFilePath: " << configFilePath << endl;

  RTA_DLTEST_DB * rtaTestDb = new RTA_DLTEST_DB(database, configFilePath);

  cout << endString << endl;

}
