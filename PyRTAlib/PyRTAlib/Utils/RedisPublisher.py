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

from .Generic import Singleton
from .Config import Config

class RedisPublisher(metaclass=Singleton):

    def __init__(self, configFilePath=''):
        self.config = Config(configFilePath)
        self.redisConn = redis.Redis(
                                        host=self.config.get('Redis','host'),
                                        port=6379,
                                        db=self.config.get('Redis','dbname'),
                                        password=self.config.get('Redis','password')
                                    )

    def publish(self, channel, message):
        if self.config.get('General', 'debug', 'bool'):
            print('[Redis publisher] Publishing the message: {} through the channel {}'.format(message,channel))
        # TODO add try catch
        self.redisConn.publish(channel, message)
