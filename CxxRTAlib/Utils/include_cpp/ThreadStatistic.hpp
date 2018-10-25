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

#ifndef THREAD_STATISTICS_H
#define THREAD_STATISTICS_H

#include <utility>      // std::pair
#include <cstdio>
#include <ctime>
#include <iostream>



using std::pair;
using std::cout;
using std::endl;

class ThreadStatistic {
  public:
    ThreadStatistic(int tid) : startTime(0), endTime(0), insertedEvents(0), thread_id(tid) {}
    std::clock_t startTime;
    std::clock_t endTime;
    int insertedEvents;
    int thread_id;

    // Total events , Execution time
    void printThreadId();
    void start();
    void finish();
    void addEvent();

    pair<int,double >& getStatistics();

};



#endif
