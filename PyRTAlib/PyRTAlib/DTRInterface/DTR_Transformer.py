from abc import ABC, abstractmethod
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
    def getSortingIndex(self, inputData=None):
        pass

    @abstractmethod
    def getOutputChannel(self, inputData=None):
        pass

    @abstractmethod
    def getStoreLocationKey(self, inputData=None):
        pass

    @abstractmethod
    def transformData(self, inputData=None):
        pass
