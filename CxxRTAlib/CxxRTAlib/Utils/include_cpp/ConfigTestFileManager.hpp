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

#ifndef CONFI_TEST_FILE_MANAGER_H
#define CONFI_TEST_FILE_MANAGER_H

#include "Config.hpp"
#include "IniParser.hpp"
#include <vector>

using std:: vector;

class ConfigTestFileManager {
public:
  static void writeConfigFile(string destinationPath, map < string, vector < map < string, string > > > ma = map < string, vector < map < string, string > > >());
  static void clearConfFile(string filepath);


private:
  ConfigTestFileManager();

};
#endif
