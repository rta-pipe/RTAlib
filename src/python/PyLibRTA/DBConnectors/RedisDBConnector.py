#import DBConnector
from .DBConnector import DBConnector
import redis



class RedisDBConnector(DBConnector):
    def __init__(self, url, username='', password=''):
        super().__init__(url, username, password)
        self.conn = 0;
        
    """
    By default, all responses are returned as bytes in Python 3
    """
    def decodeResponse(self, response):
        return response.decode("utf-8")

    def connect(self, db=0):
        super().connect(db)
        self.conn = redis.StrictRedis(host=self.url, port=6379, db=db )
        print("Connected to Redis")
        ### TODO
        # Connecting with username and password
        ###

    def testConnection(self):
        self.conn.set('testconnection', '1')
        val = self.decodeResponse(self.conn.get('testconnection'))
        if val == '1':
            return True
        else:
            return False



    def disconnect(self):
        if self.conn != 0:
            self.conn.disconnect()
        print("Redis disconnected")
