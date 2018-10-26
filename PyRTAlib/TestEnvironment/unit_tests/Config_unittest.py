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

import unittest
import sys
import os
from os.path import dirname, abspath, join
import time
from random import randint, uniform

rootFolder = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))
sys.path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.Utils import Config


DEBUG = False
config_file_path = '../'

"""
            )       )   (      (
   (     ( /(    ( /(   )\ )   )\ )   (
   )\    )\())   )\()) (()/(  (()/(   )\ )
 (((_)  ((_)\   ((_)\   /(_))  /(_)) (()/(
 )\___    ((_)   _((_) (_))_| (_))    /(_))_
((/ __|  / _ \  | \| | | |_   |_ _|  (_)) __|
 | (__  | (_) | | .` | | __|   | |     | (_ |
  \___|  \___/  |_|\_| |_|    |___|     \___|

"""
class ConfigFile(unittest.TestCase):

    def test_no_path_no_env_var_provided(self):
        if 'RTACONFIGFILE' in os.environ:
            del os.environ['RTACONFIGFILE']
        config = Config('', False)
        self.assertRaises(Exception, config.parseConfigFile, '')

    def test_file_not_found_wrong_path(self):
        config = Config('', False)
        self.assertRaises(FileNotFoundError, config.parseConfigFile, 'akjdiajwnd')

    def test_file_not_found_wrong_env_var_path(self):
        os.environ['RTACONFIGFILE'] = './ajdoiwajdoiwd'
        config = Config('', False)
        self.assertRaises(FileNotFoundError, config.parseConfigFile, '')

    def test_file_found_with_relative_path(self):
        config = Config(config_file_path)
        self.assertEqual(True, bool(config.get()))

    def test_file_found_with_environment_variable(self):
        os.environ['RTACONFIGFILE'] = config_file_path
        config = Config()
        self.assertEqual(True, bool(config.get()))

    def test_priority_to_env_var(self):
        os.environ['RTACONFIGFILE'] = config_file_path
        config = Config(config_file_path+'pluto')
        self.assertEqual(True, bool(config.get()))

    def test_singleton(self):
        config = Config(config_file_path)
        config.set('General', 'batchsize', 666)
        config = Config(config_file_path)
        self.assertEqual(666, config.get('General', 'batchsize'))
        config.reload(config_file_path)

    def test_get(self):
        config = Config(config_file_path)
        config.set('General', 'batchsize', 666)
        self.assertEqual(666, config.get('General', 'batchsize'))

    def test_get_cast(self):
        config = Config(config_file_path)
        config.set('General', 'batchsize', '1')
        self.assertEqual(1, config.get('General', 'batchsize', 'int'))

if __name__ == '__main__':

    unittest.main()
