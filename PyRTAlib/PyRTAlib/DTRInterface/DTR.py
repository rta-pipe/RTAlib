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
import redis
import threading
from collections import deque
import json
from ast import literal_eval # to convert string into dictionary

from ..Utils import Config


class DTR(ABC):



    def __init__(self, configFilePath=''):


        self.config = Config(configFilePath)

        self.DEBUG = False
        if self.config.get('Dtr','debug', 'bool'):
            self.DEBUG = True
            print("[DTR] DTR system started!")


        self.redisConn = redis.Redis(
                                        host=self.config.get('Redis','host'),
                                        port=6379,
                                        db=self.config.get('Redis','dbname'),
                                        password=self.config.get('Redis','password')
                                    )

        self.workingQueue = deque([])

        self.senderWorker = threading.Thread(target=self.processQueue, args=())
        self.senderWorker.start()


    @staticmethod
    def pub(channel, message):
        pass

    @staticmethod
    def sub(channel):
        pass

    def start(self):

        inputChannel = self.config.get('Dtr','inputchannel')

        pubsub = self.redisConn.pubsub()
        pubsub.subscribe(inputChannel)

        if self.DEBUG:
            print('[DTR] DTR subscribed to channel {} , waiting for messages...'.format(inputChannel))

        for message in pubsub.listen():

            if message['type'] != 'subscribe' and message['type'] != 'unsubscribe':

                if self.DEBUG:
                    print('[DTR] New message arrived!')# Message: {}".format(message))

                if message['data'].decode('utf-8') == 'STOP':
                    if self.DEBUG:
                        print('[DTR] Found STOP message! Waiting for the thread to stop..')
                    self.workingQueue.append("{'STOP': True}")
                    pubsub.unsubscribe()
                    self.senderWorker.join()

                elif message['type'] != 'subscribe' and message['type'] != 'unsubscribe':
                    payload = message['data'].decode('utf-8')
                    self.workingQueue.append(payload)

    def processQueue(self):

        if self.DEBUG:
            print("-->[DTR thread] Worker thread started!")

        while True:
            if len(self.workingQueue) == 0:
                continue

            eventData = literal_eval(self.workingQueue.popleft()) # eventData is a dictionary

            if self.DEBUG:
                print("\n-->[DTR thread] Pop one! Data: {}".format(eventData))


            if eventData is not None:

                if 'STOP' in eventData:
                    if self.DEBUG:
                        print("-->[DTR thread] Found STOP string in eventList.")
                    break

                if self.DEBUG:
                    print("-->[DTR thread] Transforming data..")

                # Transform data
                transformedData = self.transformData(eventData)

                if self.DEBUG:
                    print("-->[DTR thread] Saving transformed data to Redis")

                # Save it to Redis
                self.redisConn.lpush(self.outputchannel, json.dumps(transformedData))

                if self.DEBUG:
                    print("-->[DTR thread] Publishing transformed data on channel {}".format(self.outputchannel))

                # Publish on channel
                message = {'type': 'lc', 'loc': self.outputchannel, 'last_data': transformedData}
                self.redisConn.publish(self.outputchannel, message)

            else:
                print("-->[DTR thread] Something is wrong..  Type of eventData: {}".format(type(eventData)))

        if self.DEBUG:
            print("-->[DTR thread] Quitting...")
