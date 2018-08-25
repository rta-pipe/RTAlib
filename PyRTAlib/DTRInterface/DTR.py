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
from ..DataModels import *

class DTR(metaclass=Singleton):

    def __init__(self, configFilePath=''):

        self.config = Config(configFilePath)

        self.redisConn = redis.Redis(
                                        host=self.config.get('Redis','host'),
                                        port=6379,
                                        db=self.config.get('Redis','dbname'),
                                        password=self.config.get('Redis','password')
                                    )

        self.workingQueue = deque([])

        self.senderWorker = threading.Thread(target=self.processQueue, args=())


    def publish(self, event):
        self.workingQueue.append(event)



    def processQueue(self):

        while True:
            event = self.workingQueue.popleft()

            if isinstance(event, str):
                if self.config.get('General','debug') == 'yes':
                    print("-->[RTA_DL_DB thread: {} ] Found END string in eventList.".format(threadId))
            break

            if event is not None:

                event.getData()

                # Compute channel
                channel = 'visualization.'#+str(observationId)+'.'+dataType

                # Transform data

                # Save it to Redis

                # Publish on channel
