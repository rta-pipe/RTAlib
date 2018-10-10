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

int RTA_DLTEST_DB :: insertEvent(string eventidfits, string timerealtt, string ra_deg, string dec_deg, string energy, string detx, string dety, string observationid, string  datarepositoryid, string mcid, string insert_time, string status) {

  EVTTest evtTest = EVTTest(eventidfits, timerealtt, ra_deg, dec_deg, energy, detx, dety, observationid, datarepositoryid, mcid, insert_time,status);

  commit = _insertEvent(evtTest);

  // cout << "Commit RTADLTEST: " << commit << endl;

  return commit;
}
