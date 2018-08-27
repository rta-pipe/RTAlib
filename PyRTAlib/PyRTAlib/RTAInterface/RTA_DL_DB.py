# ==========================================================================
#
# Copyright (C) 2018 INAF - OAS Bologna
# Author: Leonardo Baroncelli <leonardo.baroncelli26@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ==========================================================================

from abc import ABC, abstractmethod
import queue
import threading
import time


from ..DBConnectors import RedisDBConnector, MySqlDBConnector, RedisDBConnectorBASIC
from ..Utils import Config
from ..Utils import RedisPublisher
from ..DTRInterface import DTR

class RTA_DL_DB(ABC):

    def __init__(self, database, configFilePath = '', pure_multithreading = False):

        #print('___/\____/\____/\___RTAlib-init()___/\____/\____/\___')

        if database != 'mysql' and database != 'redis' and database != 'redis-basic':
            print("[RTA_DL_DB] Error! Database '{}' is not supported. Supported databases: \n- {}\n- {}".format(database,'mysql','redis-basic'))
            exit()

        self.config = Config(configFilePath) # singleton config object

        # DTR configuration ----------------------------------------------------
        self.redisPub = None
        if self.config.get('Dtr', 'active', 'bool'):
            self.redisPub = RedisPublisher(configFilePath)

        # Pure multithreading configuration ------------------------------------
        self.pure_multithreading = pure_multithreading # Synchronous/Asynchronous single thread
        if self.config.get('General', 'numberofthreads', 'int') > 1:
            self.pure_multithreading = True


        # Synchronous (master thread) execution /\____/\____/\____/\____/\____/\
        if not self.pure_multithreading:
            self.dbConnector = self.getConnector(database, configFilePath)
            self.dbConnector.connect()


        # Multi threading mode /\____/\____/\____/\____/\____/\____/\____/\____/\
        #                    /\____/\____/\____/\____/\____/\____/\____/\____/\
        else:

            """
            The queue module implements multi-producer, multi-consumer queues. It is
            especially useful in threaded programming when information must be
            exchanged safely between multiple threads.
            """
            self.eventQueue = queue.Queue(maxsize=-1) # queue size is infinite.

            self.virginQueue = True
            # batchsize = 1 -> threadsnumber = 1 ?
            #if self.config.get('General', 'batchsize', 'int') == 1):
            #    self.config.set('General', 'numberofthreads', 1)

            """
                Variables for forced threads termination
            """
            self.run = []
            for i in range(self.config.get('General','numberofthreads', 'int')):
                self.run.append(True)

            """
                Variables for threads statistics
            """
            self.start_perf = 0
            self.end_perf = 0
            self.insertions = []
            for i in range(self.config.get('General','numberofthreads', 'int')):
                self.insertions.append(0)


            """
                Running the threads. Each thread has its own db connector.
            """
            self.threads = []
            for i in range(self.config.get('General','numberofthreads', 'int')):

                if self.config.get('General','debug', 'bool'):
                    print("[RTA_DL_DB] Starting new thread!")

                dbConnector = self.getConnector(database, configFilePath)
                t = threading.Thread(target=self.consumeQueue, args=(i, dbConnector))
                self.threads.append(t)
                t.start()




    def __enter__(self):
        return self


    def __exit__(self, exc_type, exc_value, traceback):
        self.waitAndClose()


    @abstractmethod
    def insertEvent(self, *args):
        pass


    def _insertEvent(self, event):

        eventData =  event.getData()

        # Transform data for visualization and notify GUIs
        if self.redisPub:
            self.redisPub.publish(self.config.get('Dtr','inputchannel'), eventData)

        # Synchronous (master thread) execution /\____/\____/\____/\____/\____/\
        if not self.pure_multithreading:
            self.dbConnector.insertData(self.config.get('General','evt3modelname'), eventData)

        # Multi threading mode /\____/\____/\____/\____/\____/\____/\____/\____/\
        #                    /\____/\____/\____/\____/\____/\____/\____/\____/\
        else:
            self.eventQueue.put_nowait(event)

            if self.virginQueue:
                self.virginQueue = False
                self.start_perf = time.perf_counter()


    def readNewData(self):
        try:
            return self.eventQueue.get_nowait()
        except queue.Empty:
            return None

    def getConnector(self, database, configFilePath):
        if database == 'mysql':
            return MySqlDBConnector(configFilePath)
        elif database == 'redis':
            return RedisDBConnector(configFilePath)
        elif database == 'redis-basic':
            return RedisDBConnectorBASIC(configFilePath)

    def consumeQueue(self, threadId, dbConnector):
        if self.config.get('General','debug', 'bool'):
            print('-->[RTA_DL_DB thread: {} ] Starting..'.format(threadId))

        dbConnector.connect()

        while self.run[threadId]:
            event = self.readNewData()

            if isinstance(event, str):
                if self.config.get('General','debug', 'bool'):
                    print("-->[RTA_DL_DB thread: {} ] Found END string in eventList.".format(threadId))
                break

            if event is not None:
                if not dbConnector.insertData(self.config.get('General','evt3modelname'), event.getData()):
                    print("-->[RTA_DL_DB thread: {} ] DBconnector insert data error. ".format(threadId))
                    break;

                elif self.config.get('General','debug', 'bool'):
                    print("-->[RTA_DL_DB thread: {} ] Data inserted: {} ".format(threadId, event.getData()))

                self.insertions[threadId] += 1

        if self.config.get('General','debug', 'bool'):
            print("-->[RTA_DL_DB thread: {} ] Closing connection and terminating..")

        dbConnector.close()


    def waitAndClose(self):


        # Synchronous (master thread) execution /\____/\____/\____/\____/\____/\
        if not self.pure_multithreading:

            # Stopping DTR's working thread
            if self.redisPub:
                self.redisPub.publish(self.config.get('Dtr','inputchannel'), 'STOP')

            self.dbConnector.close()
            return True

        # Multi threading mode /\____/\____/\____/\____/\____/\____/\____/\____/\
        #                    /\____/\____/\____/\____/\____/\____/\____/\____/\
        else:

            if self.config.get('General', 'debug', 'bool'):
                print('[RTA_DL_DB] Waiting all threads to finish..')

            for i in range(self.config.get('General', 'numberofthreads', 'int')):
                self.eventQueue.put('END')

            for t in self.threads:
                t.join()

            self.end_perf = time.perf_counter()


            # Stopping DTR's working thread
            if self.redisPub:
                self.redisPub.publish(self.config.get('Dtr','inputchannel'), 'STOP')


            if self.config.get('General', 'debug', 'bool'):
                print('[RTA_DL_DB] All threads stopped! Computing statistics and closing..')

            return self.getStatistics()



    # DEPRECATED
    def forceClose(self):
        if self.config.get('General', 'debug', 'bool'):
            print('[RTA_DL_DB] Stopping all threads on close()..')

        # Synchronous (master thread) execution /\____/\____/\____/\____/\____/\
        if not self.pure_multithreading:
            self.dbConnector.close()

        # Multi threading mode /\____/\____/\____/\____/\____/\____/\____/\____/\
        #                    /\____/\____/\____/\____/\____/\____/\____/\____/\
        else:
            for i in range(self.config.get('General', 'numberofthreads', 'int')):
                self.run[i] = False





    def getThreads(self):
        return threading.enumerate()

    def getStatistics(self):
        totalEvents = 0

        for i in self.insertions:
            totalEvents += i
        executionTime = self.end_perf - self.start_perf

        return (totalEvents, executionTime, round(totalEvents/executionTime,2))
