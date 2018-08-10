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

class DBConnector(ABC):

    def __init__(self, url='127.0.0.1', username='', password=''):
        super().__init__()
        self.url = url
        self.username = username
        self.password = password
        self.conn = None;
        self.dbEntities = ['evt2','evt3']
        self.indexes = []

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
