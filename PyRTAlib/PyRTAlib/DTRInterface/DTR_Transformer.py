from abc import ABC, abstractmethod
from os import path
import sys

rootFolder = path.dirname(path.dirname(path.realpath(__file__)))
sys.path.append(rootFolder+'/PyRTAlib/')
from PyRTAlib.Utils         import Config

class DTR_Transformer(ABC):
    def __init__(self, configFilePath=''):

        self.config = Config(configFilePath)


    @abstractmethod
    def getName(self):
        pass

    @abstractmethod
    def getDataType(self):
        pass

    @abstractmethod
    def getIndexForData(self):
        pass

    #@abstractmethod
    #def requiredKeys(self):
    #    pass

    @abstractmethod
    def getOutputChannel(self, eventData):
        pass

    @abstractmethod
    def transformData(self, eventData):
        pass

    @abstractmethod
    def getStoreLocationKey(self, eventData):
        pass

    #def validateModel(self, eventData, keys):
    #    for k in keys:
    #        if k not in eventData:
    #            return False
    #    return True
