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

"""
ConfigFileParser Module
The following module exports functions to parse config files that are used in the RTAlib python library.

The parseRTALIBConfigFile(filepath='') parse the RTALIB config file. If the filepath parameter is not provided the
function, it uses the RTACONFIGFILE environment variable.

RTALIB config file format:

[<section1 header>]
<key1>=<value1> | <empty>
<key2>=<value2> | <empty>

[<section2 header>]
<key1>=<value1> | <empty>
<key2>=<value2> | <empty>
...

"""

import os

def parseRTALIBConfigFile(filepath=''):

    if not filepath:
        if 'RTACONFIGFILE' in os.environ:
            filepath = os.environ['RTACONFIGFILE']
        else:
            print('[ParserError] Neither the filepath parameter or the RTACONFIGFILE environment variable have been provided.')
            return False

    filepath += '/rtalibconfig'

    configs = {}

    with open(filepath) as f:
        content = f.readlines()
        content = [x.strip() for x in content]

        sectionTempName = ''
        sectionTempValue = ()
        for line in content:
            if line and (line.find('[') != -1) :
                sectionTempName = parseSectionName(line)
                configs[sectionTempName] = {}
            elif line and (line.find('=') != -1) :
                sectionValue = parseLine(line)
                if sectionValue[1]:
                    configs[sectionTempName][sectionValue[0]] = sectionValue[1]
            #else:
                #print('Empty line')

    return configs


def parseSectionName(line):
    start = line.find('[')
    end = line.find(']', start)
    return line[start+1:end]

def parseLine(line):
    kv = line.split("=")
    return (kv[0],kv[1])
