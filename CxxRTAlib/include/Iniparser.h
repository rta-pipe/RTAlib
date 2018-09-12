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

#ifndef INIPARSER_H
#define INIPARSER_H

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using std::ifstream;
using std::string;
using std::cout;
using std::endl;
using std::vector;

class Iniparser{
public:
  //static string readFromFile(string filename);
  static void get(string filename,string section, string key, string value, string& valueReadParse);


private:
  Iniparser();

};

#endif
