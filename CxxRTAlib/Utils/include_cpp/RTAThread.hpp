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

#ifndef RTA_THREAD_H
#define RTA_THREAD_H

#include <iostream>
 
#include "EVTbase.hpp"
#include "DBConnector.hpp"
#include "ThreadStatistic.hpp"

#include "Thread.h"
#include "CTABuffer.h"


using CTAUtils :: Thread;
using CTAAlgorithm::CTABuffer;

using std::cout;
using std::endl;

class RTAThread : public Thread{
public:
	RTAThread(int idThread, string _modelname, DBConnector *_dbConnector, CTABuffer *buff, ThreadStatistic * threadStatistic) : Thread(){
		cout << "RTAThread Constructor" << endl;
		cout << "[RTAThread] threadStatistic for thread id: " << threadStatistic->thread_id << endl;
		modelname = _modelname;
		dbConnector = _dbConnector;
		eventBuffer = buff;
		id = idThread;
		totalEvents = 0;
	}
	void *run();
	int id;
	int totalEvents;
	string modelname;

	DBConnector * dbConnector;
	CTABuffer * eventBuffer;
	ThreadStatistic * threadStatistic;

};

#endif
