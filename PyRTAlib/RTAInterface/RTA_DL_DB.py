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

from ..DBConnectors import RedisDBConnector, MySqlDBConnector, RedisDBConnectorBASIC
from ..Utils import Config


class RTA_DL_DB(ABC):

    def __init__(self, database, configFilePath = '', **kwargs):
        super().__init__()

        if database != 'mysql' and database != 'redis' and database != 'redis-basic':
            print("[RTA_DL_DB] Database '{}' is not supported. Supported databases: \n- {}\n- {}".format(database,'mysql','redis'))
            exit()

        self.config = Config(configFilePath) # singleton config object

        # Synchronous (master thread) execution /\____/\____/\____/\____/\____/\
        if self.config.get('General', 'numberofthreads', 'int') == 1:
            self.dbConnector = self.getConnector(database, configFilePath)
            self.dbConnector.connect()


        # Multi threading mode /\____/\____/\____/\____/\____/\____/\____/\____/\
        else:

            """
            The queue module implements multi-producer, multi-consumer queues. It is
            especially useful in threaded programming when information must be
            exchanged safely between multiple threads.
            """
            self.eventQueue = queue.Queue(maxsize=-1) # queue size is infinite.


            # batchsize = 1 -> threadsnumber = 1 ?
            #if self.config.get('General', 'batchsize', 'int') == 1):
            #    self.config.set('General', 'numberofthreads', 1)

            """
                Variables to stop the threads
            """
            self.run = []
            for i in range(self.config.get('General','numberofthreads', 'int')):
                self.run.append(True)

            """
                Running the threads. Each thread has its own db connector.
            """
            # self.threads = []
            for i in range(self.config.get('General','numberofthreads', 'int')):

                if self.config.get('General','debug') == 'yes':
                    print("[RTA_DL_DB] Starting new thread!")

                dbConnector = self.getConnector(database, configFilePath)
                t = threading.Thread(target=self.consumeQueue, args=(i, dbConnector))
                # self.threads.append(t)
                t.start()


    def __enter__(self):
        return self


    def __exit__(self, exc_type, exc_value, traceback):
        self.close()


    @abstractmethod
    def insertEvent(self, *args):
        pass


    def _insertEvent(self, event):
        # Single thread
        if self.config.get('General', 'numberofthreads', 'int') == 1:
            self.dbConnector.insertData(self.config.get('General','evt3modelname'), event.getData())
        # Multi threading mode
        else:
            self.eventQueue.put_nowait(event)

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
        if self.config.get('General','debug') == 'yes':
            print('-->[RTA_DL_DB thread: {} ] Starting..'.format(threadId))

        dbConnector.connect()

        while self.run[threadId]:
            event = self.readNewData()

            if event is not None:
                if not dbConnector.insertData(self.config.get('General','evt3modelname'), event.getData()):
                    print("-->[RTA_DL_DB thread: {} ] DBconnector insert data error. ".format(threadId))
                    break;

                elif self.config.get('General','debug') == 'yes':
                    print("-->[RTA_DL_DB thread: {} ] Data inserted: {} ".format(threadId, event.getData()))

        if self.config.get('General','debug') == 'yes':
            print("-->[RTA_DL_DB thread: {} ] Job finished..")
        dbConnector.close()


    def close(self):

        # TODO  Should close when all threads finish their job.

        if self.config.get('General','debug') == 'yes':
            print('[RTA_DL_DB] Stopping all threads on close()..')

        if self.config.get('General', 'numberofthreads', 'int') > 1:
            for i in range(self.config.get('General', 'numberofthreads', 'int')):
                self.run[i] = False
        else:
            self.dbConnector.close()

    def getThreads(self):
        return threading.enumerate()
