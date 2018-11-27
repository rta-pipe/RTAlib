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

from sys import argv
from DTR import DTR

from DTR_DQHisto_Transformer import DTR_DQHisto_Transformer

if __name__ == '__main__':

    if len(argv) < 2:
        print("Please enter the path to the DTR configuration file")
        exit()

    configFilePath = argv[1]

    dtr = DTR(configFilePath)

    dtr.addTransformer(DTR_DQHisto_Transformer())
    #dtr.addTransformer(..)

    dtr.start()
