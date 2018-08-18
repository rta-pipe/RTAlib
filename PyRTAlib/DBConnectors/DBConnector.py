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

from abc import ABC, abstractmethod
from itertools import count

from ..Utils import parseRTALIBConfigFile

class DBConnector(ABC):
    _ids = count(0)

    def __init__(self, configFilePath=''):
        super().__init__()
        self.id = next(self._ids)

        self.configs = parseRTALIBConfigFile(configFilePath)
        self.batchsize = int(self.configs['General']['batchsize'])
        if self.configs['General']['debug'] == 'yes':
            self.debug = True
        elif self.configs['General']['debug'] == 'no':
            self.debug = False

        self.commandsSent = 0
        self.conn = None;


    def printConnectionInfo(self):
        """ For testing purpose.

        """
        print("host: {} \nusername: {} \npassword: {}\ndatabase: {}".format(self.host, self.username, self.password, self.dbname))


    def getConfigs(self):
        return self.configs

    @abstractmethod
    def connect(self, db):
        pass

    @abstractmethod
    def disconnect(self):
        pass

    @abstractmethod
    def testConnection(self):
        pass

    @abstractmethod
    def insertData(self, modelName, *args):
        pass
