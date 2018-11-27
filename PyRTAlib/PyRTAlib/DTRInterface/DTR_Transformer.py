from abc import ABC, abstractmethod
from os import path
import sys

import configparser

class DTR_Transformer(ABC):
    def __init__(self, configFilePath=''):
        self.config = configparser.ConfigParser()
        self.config.read(configFilePath)


    @abstractmethod
    def getName(self):
        pass

    @abstractmethod
    def getDataType(self):
        pass

    @abstractmethod
    def transformData(self, eventData):
        pass

    @abstractmethod
    def getIndexForRedisZSET(self):
        pass

    @abstractmethod
    def getOutputChannel(self, eventData):
        pass

    @abstractmethod
    def getStoreLocationKey(self, eventData):
        pass
