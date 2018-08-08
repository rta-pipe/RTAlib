from abc import ABC, abstractmethod

class DBConnector(ABC):

    def __init__(self, url='127.0.0.1', username='', password=''):
        super().__init__()
        self.url = url
        self.username = username
        self.password = password

    def printConnectionInfo(self):
        print("url: {} \nusername: {} \npassword: {}".format(self.url,self.username,self.password))
        pass

    @abstractmethod
    def connect(self, db):
        print("Connecting to url={} with user={} to db={}".format(self.url, self.username, db))
        pass

    @abstractmethod
    def disconnect(self):
        pass

    @abstractmethod
    def testConnection(self):
        pass
