/*
 ==========================================================================

 Copyright (C) 2018 Leonardo Baroncelli
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
#include <string>
#include "RTA_DLTEST_DB.hpp"

// run with:
// number of threads = 1
// batch size = 1

vector < map <string, double> > randomEventsGenerator(int size);

int main(int argc, char* argv[]){

  if (argc != 2) return -1;
  string localConfFileTestPath = argv[1];

  // events generation
  vector < map < string, double > > events = randomEventsGenerator(1000);

  auto rta_dltest_db = make_shared<RTA_DLTEST_DB>("mysql", localConfFileTestPath);
  rta_dltest_db->start();


  for(vector < map < string, double > >::iterator it = events.begin(); it!=events.end(); ++it ) {

    map < string, double > currentEvent = *it;

    rta_dltest_db->insertEvent(       currentEvent["eventidfits"],
                                      currentEvent["timerealtt"],
                                      currentEvent["ra_deg"],
                                      currentEvent["dec_deg"],
                                      currentEvent["energy"],
                                      currentEvent["detx"],
                                      currentEvent["dety"],
                                      currentEvent["observationid"],
                                      currentEvent["datarepositoryid"],
                                      currentEvent["mcid"],
                                      currentEvent["insert_time"],
                                      currentEvent["status"] );
  }


  rta_dltest_db->waitAndClose();

  return 0;
}















vector < map <string, double> > randomEventsGenerator(int size){

  map <string, double> args;

  vector < map <string, double> > events;

  /*  CREAZIONE EVENTI RANDOM  */
  for(int i=0; i<size; i++){
    args["eventidfits"] = rand();
    args["timerealtt"] = rand();
    args["detx"] = rand();
    args["ra_deg"] = rand();
    args["dec_deg"] = rand();
    args["energy"] = rand();
    args["dety"] = rand();
    args["observationid"] = rand();
    args["datarepositoryid"] = rand();
    args["mcid"] = rand();
    args["insert_time"] = rand();
    args["status"] = rand();

    events.push_back(args);

  }

  return events;

}
