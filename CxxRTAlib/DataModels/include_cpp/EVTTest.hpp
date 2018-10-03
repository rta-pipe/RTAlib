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

#ifndef EVT_TEST_H
#define EVT_TEST_H

#include"EVTbase.hpp"

class EVTTest : public EVTbase {

public:

  EVTTest(string eventidfits, string timerealtt,string ra_deg, string dec_deg, string energy, string detx, string dety,  string observationid, string datarepositoryid, string mcid, string insert_time, string status) : EVTbase() {
  // EVTTest(map <string, string> eventFields) : EVTbase() {

    // cout << "EvtTest" << endl;

    eventData["eventidfits"] = eventidfits;
    eventData["timerealtt"] = timerealtt;
    eventData["observationid"] = observationid;
    eventData["datarepositoryid"] = datarepositoryid;
    eventData["ra_deg"] = ra_deg;
    eventData["dec_deg"] = dec_deg;
    eventData["energy"] = energy;
    eventData["detx"] = detx;
    eventData["dety"] = dety;
    eventData["status"] = status;
    eventData["mcid"] = mcid;
    eventData["insert_time"] = insert_time;

  };

  map <string,string> & getData();

  map <string,string> eventData;

};

#endif
