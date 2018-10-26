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

from ..Utils import Config

class DBConnector(ABC):
    _ids = count(0)

    def __init__(self, configFilePath=''):
        super().__init__()
        self.id = next(self._ids)

        self.config = Config(configFilePath)

        self.batchsize = self.config.get('General', 'batchsize', 'int')

        self.debug = False
        if self.config.get('General','debug', 'bool'): # pragma: no cover
            self.debug = True


        self.commandsSent = 0
        self.conn = None;


    @abstractmethod
    def connect(self, db): # pragma: no cover
        pass

    @abstractmethod
    def disconnect(self): # pragma: no cover
        pass

    @abstractmethod
    def testConnection(self): # pragma: no cover
        pass

    @abstractmethod
    def insertData(self, modelName, *args): # pragma: no cover
        pass

    @abstractmethod
    def close(self): # pragma: no cover
        pass
