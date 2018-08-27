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

import unittest
from random import randint, uniform
from time import sleep
import redis
import threading

from PyRTAlib            import DTRtimeVSenergy
from PyRTAlib.Utils      import Config
from PyRTAlib.DataModels import EVT3_ASTRI

"""
 (            (
 )\ )   *   ) )\ )
(()/( ` )  /((()/(
 /(_)) ( )(_))/(_))
(_))_ (_(_())(_))
 |   \|_   _|| _ \
 | |) | | |  |   /
 |___/  |_|  |_|_\
"""


def publisher(redisConn, config):
    channel = config.get('Dtr', 'inputchannel')
    print('[publisher] Publisher started..')
    sleep(2)
    print('[publisher] Publisher publishing in {}..'.format(channel))
    evt3 = EVT3_ASTRI(*EVT3_ASTRI.getRandomEvent(), 1, 0, 0, 1)
    redisConn.publish(channel, evt3.getData())
    sleep(5)
    redisConn.publish(channel, 'STOP')

def subscriber(redisConn, config):
    channel = config.get('Dtr', 'outputchannel')+'.timevsenergy'
    print('[subscriber] Subscriber started..')
    # Subscribe for events
    pubsub = redisConn.pubsub()
    pubsub.subscribe([channel])
    count = 0
    for item in pubsub.listen():

        if count == 0:
            print('[subscriber] Receveid message:',item)
            #unittest.assertEqual(item['type'], 'subscribe')
            #unittest.assertEqual(item['pattern'], None)
            #unittest.assertEqual(item['channel'].decode('utf-8'), 'visualization.0.lc')
            #unittest.assertEqual(item['data'], 1)

        if count == 1:
            print('[subscriber] Receveid message:',item)
            #unittest.assertEqual(item['type'], 'message')
            #unittest.assertEqual(item['pattern'], None)
            #unittest.assertEqual(item['channel'].decode('utf-8'), 'visualization.0.lc')
            pubsub.unsubscribe()

        count += 1


class DTRTEST(unittest.TestCase):

    def test_dtr(self):


        config = Config('./')
        config.set('Dtr', 'debug', 'yes')
        config.set('Dtr', 'inputchannel', 'dtr.input.testchannel')
        config.set('Dtr', 'outputchannel', 'dtr.output.testchannel')


        dtr = DTRtimeVSenergy('./')

        conn = redis.Redis(
                host=config.get('Redis','host'),
                port=6379,
                db=config.get('Redis','dbname'),
                password=config.get('Redis','password')
            )



        # Start publisher
        pub = threading.Thread(target=publisher, args=(conn, config))
        pub.start()

        # Start subscriber
        sub = threading.Thread(target=subscriber, args=(conn, config))
        sub.start()

        dtr.start()

        pub.join()
        sub.join()




if __name__ == '__main__':
    unittest.main()
