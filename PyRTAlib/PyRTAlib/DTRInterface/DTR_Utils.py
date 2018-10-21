from astropy.coordinates import SkyCoord

def convert_fk5_to_gal(ra,dec):

    ra = float(ra)
    dec = float(dec)

    if ra < -180 or ra > 180:
         return  -2,-2

    if dec < -90 or dec > 90:
         return  -2,-2

    #convert coordinate from galactic to celestial (fk5)
    fk5_coord = SkyCoord(ra,dec, unit='deg', frame='fk5')

    gal_coord = fk5_coord.galactic

    l = "{0:.4f}".format(gal_coord.l.degree)
    b = "{0:.4f}".format(gal_coord.b.degree)

    return l,b

def convert_gal_to_fk5(l,b):

    l = float(l)
    b = float(b)

    if l < 0 or l > 360:
         return  -2,-2

    if b < -90 or b > 90:
         return  -2,-2

    #convert coordinate from galactic to celestial (fk5)
    gal_coord = SkyCoord(l,b, unit='deg', frame='galactic')

    fk5_coord = gal_coord.fk5

    ra = "{0:.4f}".format(fk5_coord.ra.degree)
    dec = "{0:.4f}".format(fk5_coord.dec.degree)

    return ra,dec
