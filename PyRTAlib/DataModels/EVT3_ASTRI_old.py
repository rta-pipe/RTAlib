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
import time
from ..Utils import time_mjd_to_tt


class EVT3_ASTRI_old():
    def __init__(self, eventidfits, timemjd, ra_deg, dec_deg, energy, detx, dety, mcid, mjdref, observationid, datarepositoryid, status):

        ## I convert those data to float() in order to convert them to json later in RedisDBConnector
        ## beacause of --> TypeError: Object of type 'uint32' (<class 'numpy.uint32'>) is not JSON serializable
        self.eventidfits = float(eventidfits)
        self.ra_deg = float(ra_deg)
        self.dec_deg = float(dec_deg)
        self.energy = float(energy)
        self.detx = float(detx)
        self.dety = float(dety)
        self.mcid = float(mcid)

        self.timerealtt = time_mjd_to_tt(mjdref) + float(timemjd)
        self.insert_time = time.time()

        self.observationid = int(observationid)
        self.datarepositoryid = int(datarepositoryid)
        self.status = int(status)


    def getData(self):
        """Return the 'dictionary' representation of the object.
        """
        return vars(self)
