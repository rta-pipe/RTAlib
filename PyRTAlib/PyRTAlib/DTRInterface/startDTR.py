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

import sys
import os

rootFolder = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))
sys.path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.DTRInterface.DTR import DTR


if __name__ == '__main__':

    if len(sys.argv) < 2:
        print("Please enter the path to the configuration file")
        exit()

    configFilePath = sys.argv[1]

    dtr = DTR(configFilePath)

    # dtr.addTransformer(...)
    # dtr.addTransformer(...)

    dtr.start()
