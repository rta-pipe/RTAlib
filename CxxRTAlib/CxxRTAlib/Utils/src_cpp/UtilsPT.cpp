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

#include "UtilsPT.hpp"

UtilsPT::UtilsPT(){

}

void UtilsPT::mysqlDeleteElements(string configFilePath, string tableName){

  Mutex* mux = Mutex::getIstance();

  MySqlDBConnector mysql(0, configFilePath);

  mysql.connect(mux);

  mysql.executeQuery("DELETE FROM "+ tableName);

  mysql.disconnect();

}

void UtilsPT::redislDeleteElements(string configFilePath, string tableName){

  Mutex* mux = Mutex::getIstance();

  RedisDBConnector redis(0, configFilePath);

  redis.connect(mux);

  redis.executeQuery("DEL "+ tableName);

  redis.disconnect();

}
