
import unittest
from PyRTAlib.DTRInterface.DTR  import DTR
from PyRTAlib.Utils         import Config

"""
 (            (
 )\ )   *   ) )\ )
(()/( ` )  /((()/(
 /(_)) ( )(_))/(_))
(_))_ (_(_())(_))
 |   \|_   _|| _ \
 | |) | | |  |   /
 |___/  |_|  |_|_\
"""
class DTRTEST(unittest.TestCase):

    def test_dtr(self):
        dtr = DTR('./')
        dtr.publish('astri',2,'lc',4)


if __name__ == '__main__':
    unittest.main()
