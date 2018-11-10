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
#include <iomanip>

#include "RTA_DLTEST_DB.hpp"
#include "ConfigTestFileManager.hpp"

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;

vector < map < string, string > > randomEventGenerator(int size);
int performance_test(vector<int> threads, vector<int> batchsizes, int numberOfEvents, vector< std::chrono::duration<double> > &timesVector,vector<double> &evtRateVector, string database, string configFilePath, int numberOfIterationPerTest);
double computeMeanTime(vector< std::chrono::duration<double> > & timesVector, int numberOfIterationPerTest, double mean);
double computeStandardDeviationTime(vector< std::chrono::duration<double> > & timesVector);
double computeMeanEvt(vector<double> &evtRateVector, int numberOfIterationPerTest, double mean);
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

int performance_test(vector<int> threads, vector<int> batchsizes, int numberOfEvents, vector< std::chrono::duration<double> > & timesVector, vector< double> &evtRateVector, string database, string configFilePath, int numberOfIterationPerTest){

  vector < map <string, string> > events = randomEventGenerator(numberOfEvents);

  RTA_DLTEST_DB * rtaTestDb = new RTA_DLTEST_DB(database, configFilePath);


  timesVector.clear();


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

  // cout << "\n"<< count <<" events inserted correctly."<< endl;

  std::chrono::duration<double> diff;

  diff = stop-start;

  timesVector.push_back(diff);
  // cout << "[performance_test] timesVector size: " << timesVector.size() << endl;
  // cout << "Tempo impiegato per inserire " << numberOfEvents << " eventi = " << diff.count() << " s" << endl;
  //
  // cout << "Event rate: " << numberOfEvents/diff.count() << endl;
  evtRateVector.push_back(numberOfEvents/diff.count());

}

double computeMeanTime(vector< std::chrono::duration<double> > & timesVector, int numberOfIterationPerTest){

  double mean = 0;

  for(vector< std::chrono::duration<double> > ::iterator it=timesVector.begin(); it!=timesVector.end(); ++it) {

    auto currentTime = *it;
    // cout << "[Compute mean] currentTime: " <<  currentTime.count()  << endl;

    mean += currentTime.count();

  }

  // cout << "[Compute mean] main inside for: "<< std::fixed<< std::setprecision(5)  <<mean << endl;

  return mean /= numberOfIterationPerTest;

}


double computeMeanEvt(vector< double > & evtRateVector, int numberOfIterationPerTest){

  // cout<< "[Compute mean] timesvector size: " << timesVector.size() << endl;
  double mean = 0;

  for(vector< double > ::iterator it=evtRateVector.begin(); it!=evtRateVector.end(); ++it) {

    auto currentEvtRate = *it;
    // cout << "[Compute mean] currentTime: " <<  currentTime.count()  << endl;

    mean += currentEvtRate;

  }

  // cout << "[Compute mean] main inside for: "<< std::fixed<< std::setprecision(5)  <<mean << endl;

  return mean /= numberOfIterationPerTest;

}

double computeStandardDeviationTime(vector< std::chrono::duration<double> > & timesVector, int numberOfIterationPerTest, double mean){

  double var= 0;
  double sd = 0;

  for(vector< std::chrono::duration<double> >::iterator it=timesVector.begin(); it!=timesVector.end(); ++it) {

    auto currentTime = *it;
    double ct = currentTime.count();

    var += ( ct - mean ) * (ct - mean);

  }
  var /= numberOfIterationPerTest;
  sd = sqrt(var);

  // cout << "[compute std] std: " << sd << endl;

  return sd;


}

double computeStandardDeviationEvt(vector< double > & evtRateVector, int numberOfIterationPerTest, double mean){

  double var= 0;
  double sd = 0;

  for(vector <double >::iterator it=evtRateVector.begin(); it!=evtRateVector.end(); ++it) {

    auto currentEvtRate = *it;

    var += ( currentEvtRate - mean ) * (currentEvtRate - mean);

  }
  var /= numberOfIterationPerTest;
  sd = sqrt(var);

  // cout << "[compute std] std: " << sd << endl;

  return sd;


}

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
  vector<int> threads({ 1 });
  vector<int> batchsizes({1});
  double meanTime = 0;
  double stdTime = 0;
  double meanEvtRate = 0;
  double stdEvtRate = 0;

  long int sd = 0;
  vector< std::chrono::duration<double> > timesVector;
  vector< double> evtRateVector;

  map < string, string > kv;
  vector < map < string, string > > entry;
  map < string, vector < map < string, string > > > section;

  cout << "\n**************************\n******  START TEST  ******\n**************************\n" << endl;
  cout << "Number of events: " << numberOfEvents << endl;
  cout << "--> Number of threads: x, Batch size: y" << endl;
  cout << "Events/sec, Execution time" << endl;

  auto totalTimeStart = std::chrono::system_clock::now();

  for( vector<int>::iterator it_t = threads.begin(); it_t != threads.end(); ++it_t ) {

    int currentThread = *it_t;

    for( vector<int>::iterator it_b = batchsizes.begin(); it_b != batchsizes.end(); ++it_b ) {

      int currentBatchSize = *it_b;
      kv["modelname"]= tableName;
      kv["mjdref"]= myConf->file["General"]["mjdref"].getString();
      kv["debug"]= myConf->file["General"]["debug"].getString();
      kv["batchsize"]= currentBatchSize;
      kv["numberofthreads"]= currentThread;
      entry.push_back(kv);
      section["General"] = entry;

      ConfigTestFileManager::writeConfigFile(section);

      for( int i = 0; i < numberOfIterationPerTest; i++ ) {

        performance_test(threads, batchsizes, numberOfEvents, timesVector,evtRateVector, database, configurationFilePath, numberOfIterationPerTest);

        meanTime = computeMeanTime(timesVector, numberOfIterationPerTest);

        stdTime = computeStandardDeviationTime(timesVector, numberOfIterationPerTest, meanTime);

        meanEvtRate = computeMeanEvt(evtRateVector, numberOfIterationPerTest);

        stdEvtRate = computeStandardDeviationEvt(evtRateVector, numberOfIterationPerTest, meanEvtRate);

      }

        cout << "\n\n--> Number of threads: " <<  currentThread << " , Batch size: " << currentBatchSize << endl;
        cout << meanEvtRate << " +- " << stdEvtRate << endl;
        cout << meanTime << " +- " << stdTime << "\n\n" << endl;



    }

  }

  auto totalTimeStop = std::chrono::system_clock::now();
  std::chrono::duration<double> diffTotal;
  diffTotal = totalTimeStop-totalTimeStart;

  cout << "\n**************************\n******  END TEST  ******\n**************************\n" << endl;
  cout << "Number of events: " << numberOfEvents << endl;
  cout << "Number of iteration per test: " << numberOfIterationPerTest <<  endl;
  cout << "Total execution time: " << diffTotal.count() << endl;
  cout << "\n\n" << endl;



  return 0;



}
