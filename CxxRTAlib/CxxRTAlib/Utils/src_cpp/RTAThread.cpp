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

#include "RTAThread.hpp"
#include "RedisDBConnector.hpp"


void *RTAThread::run()  {

  // threadStatistic->start();
  #ifdef DEBUG
  cout <<  "[Thread " << id << "]  Started! " << endl;
  #endif

  bool con = dbConnector->connect(mux);
  #ifdef DEBUG
  cout <<  "[Thread " << id << "]  Connected! " << endl;
  #endif

  if( !con ) {

    // #ifdef DEBUG
    cout << "[Thread " << id <<"] CXX_RTA_DL_X_DB Connection Error!" << endl;
    // #endif
    exit(EXIT_FAILURE); //  double free or corruption (fasttop): 0x00000000015a6060 ***
    return NULL;

  }else{
    bool running = true;
    while(running) {


      EVTbase * currentEvent = ( EVTbase *)eventBuffer->get();

      if (! currentEvent->last) {

        map < string, string > fetchData = currentEvent->getData();
        dbConnector->insertData( modelname, fetchData);
        delete currentEvent;
        totalEvents++;
        // threadStatistic->addEvent();


        #ifdef DEBUG
        cout << "[Thread " << id<<"] Inserted!" << endl;
        #endif
      }
      else {
        running = false;
      }
    }

    dbConnector->disconnect();

//    #ifdef DEBUG
    cout << "[Thread " << id<< "]: totalEvents inserted " << totalEvents << endl;
  //  cout << "[Thread " << id<<"] Quitting.." << endl;
//    #endif

  }

  return NULL;

}
