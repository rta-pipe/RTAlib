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
from ..Utils import Config
from math import isnan

class EVTbase:
    def __init__(self): # pragma: no cover

        """
        NEVER PUT CLASS ATTRIBUTES (they will be inherited by subclasses and added to the dict representation)
        """
        pass


    def getData(self):
        """Return the 'dictionary' representation of the object.
        """
        dictData = vars(self)
        for key,val in dictData.items():
            if val == None:
                dictData[key] = 'NULL'
            elif isnan(val):
                #print("[RTAlib warning] key=",key," = ", val)
                dictData[key] = 'NULL'


        return vars(self)
