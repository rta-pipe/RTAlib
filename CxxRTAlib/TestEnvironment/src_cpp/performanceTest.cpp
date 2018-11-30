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
#include <ctime>
#include <iomanip>

#include "RTA_DLTEST_DB.hpp"
#include "ConfigTestFileManager.hpp"
#include "UtilsPT.hpp"
#include "FileWriter.hpp"

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;

vector < map < string, double > > randomEventGenerator(int numberOfEvents);
int performance_test(vector<int> threads, vector<int> batchsizes, int numberOfEvents, vector< std::chrono::duration<double> > &timesVector,vector<double> &evtRateVector, string database, string configFilePath, int numberOfIterationPerTest);
double computeMeanTime(vector< std::chrono::duration<double> > & timesVector, int numberOfIterationPerTest, double mean);
double computeStandardDeviationTime(vector< std::chrono::duration<double> > & timesVector);
double computeMeanEvt(vector<double> &evtRateVector, int numberOfIterationPerTest, double mean);
int count = 0;



vector < map < string, double > > randomEventGenerator(int numberOfEvents) {

  map <string, double> args;

  vector < map <string, double> > events;

  /*  CREAZIONE EVENTI RANDOM  */
  for(int i=0; i<numberOfEvents; i++){
    args["eventidfits"] = rand();
    args["timerealtt"] =rand();
    args["ra_deg"] =rand();
    args["dec_deg"] =rand();
    args["energy"] =rand();
    args["detx"] =rand();
    args["dety"] =rand();
    args["observationid"] =rand();
    args["datarepositoryid"] =rand();
    args["status"] =1;
    args["mcid"] =1;
    args["insert_time"] =rand();

    events.push_back(args);

  }

  return events;
}

int performance_test(vector<int> threads, vector<int> batchsizes, int numberOfEvents, vector< std::chrono::duration<double> > & timesVector, vector< double> &evtRateVector, string database, string configFilePath, int numberOfIterationPerTest){

  vector < map <string, double> > events = randomEventGenerator(numberOfEvents);

  RTA_DLTEST_DB * rtaTestDb = new RTA_DLTEST_DB(database, configFilePath);


  timesVector.clear();


  #ifdef DEBUG
  cout << "Inserting events...\n " <<endl;
  #endif

  rtaTestDb->start();

  auto start = std::chrono::system_clock::now();

  for(vector < map <string, double> >::iterator it=events.begin(); it!=events.end(); it++) {

    map < string, double > currentEvent = *it;

    count +=rtaTestDb->insertEvent(   to_string(currentEvent["eventidfits"]),
                                      to_string(currentEvent["timerealtt"]),
                                      to_string(currentEvent["ra_deg"]),
                                      to_string(currentEvent["dec_deg"]),
                                      to_string(currentEvent["energy"]),
                                      to_string(currentEvent["detx"]),
                                      to_string(currentEvent["dety"]),
                                      to_string(currentEvent["observationid"]),
                                      to_string(currentEvent["datarepositoryid"]),
                                      to_string(currentEvent["mcid"]),
                                      to_string(currentEvent["insert_time"]),
                                      to_string(currentEvent["status"]) );

  }

  rtaTestDb->waitAndClose();

  auto stop = std::chrono::system_clock::now();

  std::chrono::duration<double> diff;

  diff = stop-start;

  timesVector.push_back(diff);

  evtRateVector.push_back(numberOfEvents/diff.count());
  #ifdef DEBUG
  cout << "\n"<< count <<" events inserted correctly."<< endl;
  cout << "Tempo impiegato per inserire " << numberOfEvents << " eventi = " << diff.count() << " s" << endl;
  cout << "Event rate: " << numberOfEvents/diff.count() << endl;
  #endif

}

double computeMeanTime(vector< std::chrono::duration<double> > & timesVector, int numberOfIterationPerTest){

  double mean = 0;

  for(vector< std::chrono::duration<double> > ::iterator it=timesVector.begin(); it!=timesVector.end(); ++it) {

    auto currentTime = *it;
    // cout << "[Compute mean] currentTime: " <<  currentTime.count()  << endl;

    mean += currentTime.count();

  }

  mean /= numberOfIterationPerTest;
  return mean;

}


double computeMeanEvt(vector< double > & evtRateVector, int numberOfIterationPerTest){

  // cout<< "[Compute mean] timesvector size: " << timesVector.size() << endl;
  double mean = 0;

  for(vector< double > ::iterator it=evtRateVector.begin(); it!=evtRateVector.end(); ++it) {

    auto currentEvtRate = *it;

    mean += currentEvtRate;

  }

  mean /= numberOfIterationPerTest;
  return mean;

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

  if (argc != 7) {

    cout << "Please enter:\
                    \n - the database to be used (mysql or redis-basic)\
                    \n - the table name to be used\
                    \n - the number of events to be inserted\
                    \n - the number of times a test must be repeated (>1)\
                    \n - the path to the configuration file \
                    \n - the cleanerTrigger value ('1' to clean tables before start test, '0' otherwise)" << endl;

    exit(EXIT_FAILURE);

  }


  string database = argv[1];
  string tableName = argv[2];
  int numberOfEvents = atoi(argv[3]);
  int numberOfIterationPerTest = atoi(argv[4]);
  string configurationFilePath = argv[5];
  bool cleanerTrigger = atoi(argv[6]);

  // Test configuration
  vector<int> threads({ 1, 8 });
  vector<int> batchsizes({1, 10});
  double meanTime = 0;
  double stdTime = 0;
  double meanEvtRate = 0;
  double stdEvtRate = 0;
  double dataRate = 0;

  vector< std::chrono::duration<double> > timesVector;
  vector< double> evtRateVector;

  map < string, string > kv;
  vector < map < string, string > > entry;
  map < string, vector < map < string, string > > > section;

  char * val = getenv( "RTALIBDIR" );
  string envVar(val);
  string localConfFileTestPath = envVar + "/CxxRTAlib/TestEnvironment/rtalibconfigPTest";
  string cmd = "cp " + configurationFilePath + " " + localConfFileTestPath;
  if(std::system(cmd.c_str()) != 0){
    exit(EXIT_FAILURE);
  }

  string json = "[\n";
  string jsn;

  cout << "\n**************************\n******  START TEST  ******\n**************************\n" << endl;
  cout << "Number of events: " << numberOfEvents << endl;
  cout << "--> Number of threads: x, Batch size: y" << endl;
  cout << "Events/sec, Execution time" << endl;

  auto totalTimeStart = std::chrono::system_clock::now();

  for( vector<int>::iterator it_t = threads.begin(); it_t != threads.end(); ++it_t ) {

    int currentThread = *it_t;

    for( vector<int>::iterator it_b = batchsizes.begin(); it_b != batchsizes.end(); ++it_b ) {

      int currentBatchSize = *it_b;

      Config * myConf;
      myConf = Config::getIstance(localConfFileTestPath);
      kv["modelname"]= tableName;
      kv["mjdref"]= myConf->file["General"]["mjdref"].getString();
      kv["debug"]= myConf->file["General"]["debug"].getString();
      kv["numberofthreads"]= to_string(currentThread);
      kv["batchsize"]= to_string(currentBatchSize);
      entry.push_back(kv);
      section["General"] = entry;

      ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
      myConf->deleteInstance();

      double size = UtilsPT::getRowSize(localConfFileTestPath, tableName);

      for( int i = 0; i < numberOfIterationPerTest; i++ ) {

        if(cleanerTrigger) {
          if(database.compare("mysql") == 0 )
            UtilsPT::mysqlDeleteElements(localConfFileTestPath, tableName);
          else
            UtilsPT::redislDeleteElements(localConfFileTestPath, tableName);
        }


        performance_test(threads, batchsizes, numberOfEvents, timesVector,evtRateVector, database, localConfFileTestPath, numberOfIterationPerTest);

        meanTime = computeMeanTime(timesVector, numberOfIterationPerTest);

        stdTime = computeStandardDeviationTime(timesVector, numberOfIterationPerTest, meanTime);

        meanEvtRate = computeMeanEvt(evtRateVector, numberOfIterationPerTest);

        stdEvtRate = computeStandardDeviationEvt(evtRateVector, numberOfIterationPerTest, meanEvtRate);

      }

        cout << "\n\n--> Number of threads: " <<  currentThread << " , Batch size: " << currentBatchSize << endl;
        cout << std::fixed<< std::setprecision(2) << meanEvtRate << " +- " << stdEvtRate << " Hz" << endl;
        cout << std::fixed<< std::setprecision(2) << meanTime << " +- " << stdTime  << " s" << endl;
        dataRate = meanEvtRate*size/1024/1024;
        cout << dataRate << " MB/s\n\n"<< endl;

        json += " { numberOfEvents : " + to_string(numberOfEvents) +", numberofthreads : " + to_string(currentThread) + " , batchsize : " + to_string(currentBatchSize) + " , eventRate : " + to_string(meanEvtRate) + ", evt_error : " +to_string(stdEvtRate)+", time : " +to_string(meanTime)+ ", time_error :"+ to_string(stdTime)+ ", dataRate : " + to_string(dataRate)+ " },\n ";

        jsn = json.substr(0, json.size()-3);


        timesVector.clear();
        evtRateVector.clear();
        entry.clear();


    }

    jsn += "\n]";

  }

  auto totalTimeStop = std::chrono::system_clock::now();
  std::chrono::duration<double> diffTotal;
  diffTotal = totalTimeStop-totalTimeStart;

  cout << "\n**************************\n******  END TEST  ******\n**************************\n" << endl;
  cout << "Number of events: " << numberOfEvents << endl;
  cout << "Number of iteration per test: " << numberOfIterationPerTest <<  endl;
  cout << "Total execution time: " << std::fixed<< std::setprecision(2) << diffTotal.count() << " s" <<  endl;
  cout << "\n\n" << endl;

  // current date/time based on current system
  time_t now = time(0);

  // convert now to string form
  string dt = ctime(&now);
  dt.erase(remove_if(dt.begin(), dt.end(), isspace), dt.end());


  string outJSONPath = envVar + "/CxxRTAlib/TestEnvironment/";
  string outJSONNAme = outJSONPath + "jsonPerfTest"+dt;
  FileWriter::write2File(outJSONNAme,jsn);
  cout << jsn << endl;
  cout << "\n\n" << endl;



  return 0;



}
