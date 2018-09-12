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


#include "DBConnector.h"

DBConnector::DBConnector(string filepath){

  config = Config::getIstance(filepath);
  // cout << "DBConnector constructor" << endl;
  // cout << "filepath: "<< filepath << endl;


}

int DBConnector :: connect(){}
int DBConnector :: disconnect(){}
int DBConnector :: testConnection(){}
int DBConnector :: insertData(string modelName, std::vector < pair < string, double > > args){}
