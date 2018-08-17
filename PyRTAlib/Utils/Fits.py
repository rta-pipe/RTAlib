# ==========================================================================
#
# Copyright (C) 2018 Leonardo  Baroncelli
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

from astropy.io import fits
#from operator import itemgetter

def read_data_from_fits(fits_file_path):

    # Read the fit file
    hdulist = fits.open(fits_file_path)
    hdulist.info()
    print(hdulist[1].columns)
    tbdata = hdulist[1].data
    print("no of elements: " +  str(len(tbdata)))
    return tbdata
    """
    time = str(float(event[1])-timestart)
    eventidfits = str(event[0])
    timestart = float(event[1])
    ra = str(event[2])
    dec = str(event[3])
    energy = str(event[4])
    detx = str(event[5])
    dety = str(event[6])
    mc_id = str(event[7])
    """

def time_mjd_to_tt(mjd):
    return (float(mjd) - 53005.0) * 86400.0

def time_tt_to_mjd(tt):
    return (float(tt) / 86400.0) + 53005.0
