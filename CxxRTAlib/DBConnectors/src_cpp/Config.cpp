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

#include "Config.hpp"

Config* Config::_instance = 0;

Config::Config(string filepath){

  char* pPath;
  pPath = getenv("RTACONFIGFILE");
  if (pPath!=NULL){

    filepath = pPath;

  }else if(filepath==""){
    cout << "[Config] Cant configure. Neither the filepath parameter or the RTACONFIGFILE environment variable have been provided." << endl;
  }

  filepath += "/rtalibconfig";

	#ifdef DEBUG
    cout << filepath << endl;
  #endif

  // try loading file
  try {
    file = IniParser::load(filepath);
  } catch (IniParser::ParserException e) {
    std::cerr << "Error while loading file!\n";
  }

}

Config* Config::getIstance(string filepath){

  if(_instance == 0)
    _instance = new Config(filepath);

  return _instance;
}