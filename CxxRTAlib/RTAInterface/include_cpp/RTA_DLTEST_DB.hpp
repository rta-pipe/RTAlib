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

#ifndef RTA_DL_TEST_DB_H
#define RTA_DL_TEST_DB_H

#include"RTA_DL_DB.hpp"
#include"EVTTest.hpp"
#include"DBConnector.hpp"

class RTA_DLTEST_DB : public RTA_DL_DB {
public:

  RTA_DLTEST_DB(string database, string configFilePath) : RTA_DL_DB(database, configFilePath) {
    cout << "RTA_TEST_DB" << endl;
  };

  int insertEvent(string eventidfits, string time, string ra_deg, string dec_deg, string energy, string detx, string dety, string observationid, string datarepositoryid, string status);

  
};

#endif
