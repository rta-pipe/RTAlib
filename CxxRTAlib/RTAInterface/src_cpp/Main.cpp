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

#include <stdlib.h>
#include <iostream>
#include <utility>
#include <map>
#include <chrono>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

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

  /* initialize random seed: */
  srand (time(NULL));

  string database = argv[1];

  string configFilePath = argv[2];

  int size = atoi(argv[3]);

  cout << "Database: " << database << endl;

  cout << "configFilePath: " << configFilePath << endl;

  cout << "Size: " << size << endl;

  map <string, string> args;

  vector < map <string, string> > events;

  /*  CREAZIONE EVENTI RANDOM  */
  for(int i=0; i<size; i++){
    args["eventidfits"] =to_string(rand());
    args["timerealtt"] =to_string(rand());
    args["ra_deg"] =to_string(rand());
    args["dec_deg"] =to_string(rand());
    args["energy"] =to_string(rand());
    args["detx"] =to_string(rand());
    args["dety"] =to_string(rand());
    args["observationid"] =to_string(rand());
    args["datarepositoryid"] =to_string(rand());
    args["status"] =to_string(1);
    args["mcid"] =to_string(1);
    args["insert_time"] =to_string(rand());

    events.push_back(args);

  }

  auto start = std::chrono::system_clock::now();

  RTA_DLTEST_DB * rtaTestDb = new RTA_DLTEST_DB(database, configFilePath);

  for(vector < map <string, string> >::iterator it=events.begin(); it!=events.end(); it++) {

    map < string, string > currentEvent = *it;

    rtaTestDb->insertEvent( currentEvent["eventidfits"],
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

  rtaTestDb->waitAndClose();

  auto stop = std::chrono::system_clock::now();

  std::chrono::duration<double> diff = stop-start;

  cout << "Tempo impiegato per inserire " << size << " eventi = " << diff.count() << " s" << endl;

  cout << "Event rate: " << size/diff.count() << endl;

  cout << endString << endl;

}
