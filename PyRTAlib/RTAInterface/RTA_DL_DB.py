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
from ..DBConnectors import RedisDBConnector, MySqlDBConnector
from ..Utils import parseRTALIBConfigFile


class RTA_DL_DB(ABC):

    def __init__(self, database, configFilePath = ''):
        super().__init__()
        self.dbConnector = None
        if database == 'mysql':
            self.dbConnector = MySqlDBConnector(configFilePath)
        elif database == 'redis':
            self.dbConnector = RedisDBConnector(configFilePath)
        else:
            print("[RTA_DL_DB] Database '{}' is not supported. Supported databases: \n- {}\n- {}".format(database,'mysql','redis'))

        self.dbConnector.connect()

        self.configs = parseRTALIBConfigFile(configFilePath, 'General')


    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.close()

    def isConnectionAlive(self):
        return self.dbConnector.testConnection()


    @abstractmethod
    def insertEvent(self, *args):
        pass


    def close(self):
        self.dbConnector.close()
