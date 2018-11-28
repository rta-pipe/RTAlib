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

import redis
import threading
from collections import deque
import json
from ast import literal_eval
import configparser

class DTR():
    def __init__(self, configFilePath=''):
        print("[DTR] DTR system started!")

        self.config = configparser.ConfigParser()
        self.config.read(configFilePath)

        self.DEBUG = self.config.getboolean('General','debug')

        self.transformers = []

        self.redisConn = redis.Redis(
                                        host=self.config['Redis']['host'],
                                        port=self.config['Redis']['port'],
                                        db=self.config['Redis']['dbname'],
                                        password=self.config['Redis']['password']
                                    )


        self.workingQueue = deque([])

        self.senderWorker = threading.Thread(target=self.processQueue, args=())
        self.senderWorker.start()


    def addTransformer(self, transformer):
        if self.DEBUG:
            print('[DTR] Added new transformer: ', transformer.getName())
        self.transformers.append(transformer)

    def getAllowedTransformers(self, eventData):
        validated = ( t for t in self.transformers if t.getDataType()==eventData['dataType'] )
        return list(validated)

    def start(self):
        msgNum = 0

        inputChannel = self.config.get('General','inputchannel')

        pubsub = self.redisConn.pubsub()
        pubsub.subscribe(inputChannel)

        print('[DTR] Subscribed to channel {} , waiting for messages...'.format(inputChannel))

        for message in pubsub.listen():

            if message['type'] != 'subscribe' and message['type'] != 'unsubscribe':

                msgNum += 1

                if self.DEBUG:
                    print('[DTR] New message arrived! ({})'.format(msgNum))# Message: {}".format(message))

                if message['data'].decode('utf-8') == 'STOP':
                    if self.DEBUG:
                        print('[DTR] Found STOP message! Waiting for the thread to stop..')
                    self.workingQueue.append("{'STOP': True}")
                    pubsub.unsubscribe()
                    self.senderWorker.join()

                else:
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

                transformers = self.getAllowedTransformers(eventData)

                if self.DEBUG:
                    print("-->[DTR thread] Allowed transformers ({}) for eventData: ".format(len(list(transformers))))
                    for t in transformers:
                        print("--> {}".format(t.getName()))


                # Transform data
                for dataTransformer in transformers:

                    transformerName = dataTransformer.getName()
                    dataType = dataTransformer.getDataType()
                    indexData = dataTransformer.getSortingIndex()
                    transformedData = dataTransformer.transformData(eventData)
                    outputChannel = dataTransformer.getOutputChannel(eventData)
                    storeLocationKey = dataTransformer.getStoreLocationKey(eventData)


                    print("\n-->[DTR thread] Data transformer ",transformerName,"\n" \
                                        "Saving transformed data to Redis at ",storeLocationKey," in database nr: ",self.config.get('Redis','dbname'),"\n" \
                                        "Data type: ", dataType, "\n" \
                                        "Output channel: ", outputChannel, "\n" \
                                        "storeLocationKey: ", storeLocationKey, "\n" \
                                        "transformedData: ", transformedData)

                    # Save it to Redis
                    #self.redisConn.lpush(storeLocationKey, json.dumps(transformedData))  # --> changed to ZSET
                    self.redisConn.zadd(storeLocationKey, json.dumps(transformedData), transformedData[indexData])

                    if self.DEBUG:
                        print("-->[DTR thread] Publishing transformed data on channel {}".format(outputChannel))

                    # Publish on channel
                    message = {'type': dataType, 'loc': storeLocationKey, 'last_data': transformedData}
                    self.redisConn.publish(outputChannel, json.dumps(message))

            else:
                print("-->[DTR thread] Something is wrong..  Type of eventData: {}".format(type(eventData)))

        if self.DEBUG:
            print("-->[DTR thread] Quitting...")
