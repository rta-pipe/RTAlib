from abc import ABC, abstractmethod

class DLDB(ABC):

    def __init__(self, url, username, password):
        self.url = url
        self.username = username
        self.password = password
        super().__init__()

    @abstractmethod
    def connect(self):
        print "basic functionality of the base-class connect method"
        pass

    @abstractmethod
    def disconnect(self):
        pass
