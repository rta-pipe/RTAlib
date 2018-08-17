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

class EVT3_ASTRI():
    def __init__(self, evtid, eventidfits, observationid, datarepositoryid, ra_deg, dec_deg, energy, detx, dety, mcid, status, timerealtt, insert_time):
        self.evtid = evtid
        self.eventidfits = eventidfits
        self.observationid = observationid
        self.datarepositoryid = datarepositoryid
        self.ra_deg = ra_deg
        self.dec_deg = dec_deg
        self.energy = energy
        self.detx = detx
        self.dety = dety
        self.mcid = mcid
        self.status = status
        self.timerealtt = timerealtt
        self.insert_time = insert_time
        pass

    def getInsertQuery(self, table):
        query = 'INSERT INTO '+table
        queryK = '(evtid, eventidfits, observationid, datarepositoryid, ra_deg, dec_deg, energy, detx, dety, mcid, status, timerealtt, insert_time) '
        queryV = 'VALUES('  +str(self.evtid)+', '             \
                            +str(self.eventidfits)+', '       \
                            +str(self.observationid)+', '     \
                            +str(self.datarepositoryid)+', '  \
                            +str(self.ra_deg)+', '            \
                            +str(self.dec_deg)+', '           \
                            +str(self.energy)+', '            \
                            +str(self.detx)+', '              \
                            +str(self.dety)+', '              \
                            +str(self.mcid)+', '              \
                            +str(self.status)+', '            \
                            +str(self.timerealtt)+', '        \
                            +str(self.insert_time)+')'
        return query+queryK+queryV
