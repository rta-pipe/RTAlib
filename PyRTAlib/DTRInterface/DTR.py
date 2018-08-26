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

from ..Utils import Config
from ..Utils.Generic import Singleton

class DTR(metaclass=Singleton):

    def __init__(self, configFilePath=''):

        self.config = Config(configFilePath)

        if self.config.get('Dtr','debug') == 'yes':
            print("-->[DTR] DTR system started!")


        self.redisConn = redis.Redis(
                                        host=self.config.get('Redis','host'),
                                        port=6379,
                                        db=self.config.get('Redis','dbname'),
                                        password=self.config.get('Redis','password')
                                    )

        self.workingQueue = deque([])

        self.senderWorker = threading.Thread(target=self.processQueue, args=())
        self.senderWorker.start()


    def publish(self, event):
        self.workingQueue.append(event)



    def processQueue(self):

        if self.config.get('General','debug') == 'yes':
            print("-->[DTR thread] Worker thread started!")

        while True:
            if len(self.workingQueue) == 0:
                continue

            event = self.workingQueue.popleft()

            if isinstance(event, str):
                if self.config.get('General','debug') == 'yes':
                    print("-->[DTR thread] Found END string in eventList.")
                break

            if event is not None:

                data = event.getData()
                print(data)

                # Compute channel and key
                keychannel = 'visualization.'+str(data['observationid'])+'.lc'
                print(keychannel)

                # Transform data
                lc_data = {'energy': data['energy'], 'time': data['timerealtt'], 'isUpperLimit': True if data['energy']<= 2.9 else False }
                print(lc_data)

                # Save it to Redis
                self.redisConn.lpush(keychannel, lc_data)

                # Publish on channel
                message = {'type': 'lc', 'loc': keychannel, 'last_data': lc_data}
                print(message)
                self.redisConn.publish(keychannel, message)

    def waitAndClose(self):
        self.workingQueue.append('END')
        self.senderWorker.join()
        if self.config.get('General','debug') == 'yes':
            print('[DTR] Worker thread stopped! Closing..')
