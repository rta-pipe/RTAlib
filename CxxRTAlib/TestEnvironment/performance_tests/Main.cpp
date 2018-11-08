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

#include<ctime>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<map>
#include<vector>
#include <chrono>

#include "RTA_DLTEST_DB.hpp"

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;

vector < map < string, string > > randomEventGenerator(int size);
int performance_test(string database, string configFilePath, int numberOfIterationPerTest);
// long int computeMean();
// long int computeStandardDeviation();
int count = 0;



vector < map < string, string > > randomEventGenerator(int numberOfEvents) {

  map <string, string> args;

  vector < map <string, string> > events;

  /*  CREAZIONE EVENTI RANDOM  */
  for(int i=0; i<numberOfEvents; i++){
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

  return events;
}

int performance_test(int numberOfEvents, string database, string configFilePath, int numberOfIterationPerTest){

  cout << configFilePath << endl;

  vector < map <string, string> > events = randomEventGenerator(numberOfEvents);

  RTA_DLTEST_DB * rtaTestDb = new RTA_DLTEST_DB(database, configFilePath);

  for( vector<int>::iterator it_t = threads.begin(); it_t != threads.end(); ++it_t ) {

    for( vector<int>::iterator it_b = batchsizes.begin(); it_b != batchsizes.end(); ++it_b ) {

      int batchsizeValue = *it_b;

      for( int i = 0; i < numberOfIterationPerTest; i++ ) {

        #ifdef DEBUG
        cout << "Inserting events...\n " <<endl;
        #endif

        rtaTestDb->start();

        auto start = std::chrono::system_clock::now();

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

        auto stop = std::chrono::system_clock::now();

        cout << "\n"<< count <<" events inserted correctly."<< endl;

        std::chrono::duration<double> diff;

        diff = stop-start;

        timesVector.push_back(diff);

        cout << "Tempo impiegato per inserire " << numberOfEvents << " eventi = " << diff.count() << " s" << endl;

        cout << "Event rate: " << numberOfEvents/diff.count() << endl;

      }

    }

  }

}

// long int computeMean(){
//
//   for(vector < time_t >::iterator it=timesVector.begin(); it!=timesVector.end(); ++it) {
//
//     double currentTime = *it;
//
//     mean += currentTime;
//
//   }
//
//   return mean /= numberOfIterationPerTest;
//
// }

// long int computeStandardDeviation(){
//
//   for(vector < time_t >::iterator it=timesVector.begin(); it!=timesVector.end(); ++it) {
//
//     double currentTime = *it;
//
//     var += ( currentTime -mean ) * (currentTime - mean);
//
//   }
//   var /= numberOfIterationPerTest;
//   sd = sqrt(var);
//
//
// }

int main(int argc, char * argv[]) {

  if (argc != 6) {

    cout << "Please enter:\
                    \n - the database to be used (mysql or redis-basic)\
                    \n - the table name to be used\
                    \n - the number of events to be inserted\
                    \n - the number of times a test must be repeated (>1)\
                    \n - the path to the configuration file" << endl;

    exit(EXIT_FAILURE);

  }

  string database = argv[1];
  string tableName = argv[2];
  int numberOfEvents = atoi(argv[3]);
  int numberOfIterationPerTest = atoi(argv[4]);
  string configurationFilePath = argv[5];

  // Test configuration
  const vector<int> threads({0, 1, 2 });
  const vector<int> batchsizes({1, 100, 1000});
  long int var = 0;
  long int mean = 0;
  long int sd = 0;
  vector< std::chrono::duration<double> > timesVector;
  vector< long int > meansvector;
  vector< long int > meansVector;
  vector< long int > sdVector;

  cout << "\n**************************\n******  START TEST  ******\n**************************\n" << endl;
  cout << "Number of events: " << numberOfEvents << endl;
  cout << "--> Number of threads: x, Batch size: y" << endl;
  cout << "Events/sec, Execution time" << endl;


  performance_test(numberOfEvents, database, configurationFilePath, numberOfIterationPerTest);



  return 0;



}
