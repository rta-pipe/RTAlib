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



#include "RTA_DLTEST_DB.hpp"


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

  if( argc != 4 ) {

    cout << "Error: wrong input number." << "\n" <<
            " * Database: mysql or redis-basic" << "\n" <<
            " * Configuration file path: ../PyRTAlib/rtalibconfig" << "\n" <<
            " * Size: event number to simulate" << "\n" << endl;

    cout << endString << endl;

    exit(EXIT_FAILURE);

  }

  string database = argv[1];

  string configFilePath = argv[2];

  int size = atoi(argv[3]);

  if( size <= 0 ) {

    cout << "Error: event numer to simulate MUST BE grater than 0" << endl;

    exit(EXIT_FAILURE);

  }

  int count = 0;

  cout << "Database: " << database << endl;

  cout << "configFilePath: " << configFilePath << endl;

  cout << "Size: " << size << endl;

  cout << endl;

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


  auto rtaTestDb = make_shared<RTA_DLTEST_DB>(database, configFilePath);

  sleep(1);

  auto start = std::chrono::system_clock::now();

  rtaTestDb->start();

  #ifdef DEBUG
  cout << "Inserting events...\n " <<endl;
  #endif

  for(vector < map <string, string> >::iterator it=events.begin(); it!=events.end(); it++) {

    map < string, string > currentEvent = *it;

    count +=rtaTestDb->insertEvent(  currentEvent["eventidfits"],
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

  cout << "\n"<< count <<" events inserted correctly."<< endl;

  auto stop = std::chrono::system_clock::now();

  int numberofthreads = rtaTestDb->getNumberOfThreads();

  std::chrono::duration<double> diff;

  if(numberofthreads>1){
    std::chrono::duration<double> overhead(0.5*rtaTestDb->getNumberOfThreads());

    diff = stop - start/* - overhead*/; // BE CAREFUL! POSSIBILE BUG ON 0.5 hardcoded value (sleep time of RTA_DL_DB between each thread start)

  }
  else
    diff = stop-start;


  cout << "Tempo impiegato per inserire " << size << " eventi = " << diff.count() << " s" << endl;

  cout << "Event rate: " << size/diff.count() << endl;

  cout << endString << endl;

}
