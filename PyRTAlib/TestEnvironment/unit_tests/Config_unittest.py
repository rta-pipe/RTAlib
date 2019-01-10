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

#!/usr/bin/env python

import unittest
from sys import path
from os.path import dirname, abspath, realpath
from os import environ

rootFolder = dirname(dirname(dirname(dirname(realpath(__file__)))))
path.append(rootFolder+'/PyRTAlib/')

from PyRTAlib.Utils import Config


if 'RTALIBCONFIG' not in environ:
    print('\n[Config__unittest.py] No RTALIBCONFIG env var found. Please export it and let it points to the rtalibconfig_testing configuration file.\n')
    exit(1)
config_file_path = environ['RTALIBCONFIG']

DEBUG = False

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
        if 'RTALIBCONFIGFILE' in environ:
            del environ['RTALIBCONFIGFILE']
        config = Config('', False)
        self.assertRaises(Exception, config.parseConfigFile, '')


    def test_file_not_found_wrong_path(self):
        config = Config('', False)
        self.assertRaises(FileNotFoundError, config.parseConfigFile, '../../../Configs/akjdiajwnd')


    def test_file_not_found_wrong_env_var_path(self):
        environ['RTALIBCONFIGFILE'] = '../wrong/path/Configs/rtalibconfig_testing'
        config = Config('', False)
        self.assertRaises(FileNotFoundError, config.parseConfigFile, '')


    def test_file_found_with_relative_path(self):
        config = Config(config_file_path)
        self.assertEqual(True, bool(config.get()))


    def test_file_found_with_environment_variable(self):
        environ['RTALIBCONFIGFILE'] = config_file_path
        config = Config()
        self.assertEqual(True, bool(config.get()))


    def test_priority_to_env_var(self):
        environ['RTALIBCONFIGFILE'] = config_file_path
        config = Config('../../../Configs/wrongconfigfile')
        self.assertEqual(True, bool(config.get()))


    def test_singleton(self):
        config = Config(config_file_path)
        config.set('General', 'batchsize', 666)
        config = Config(config_file_path)
        self.assertEqual(666, config.get('General', 'batchsize'))
        config.reload(config_file_path)


    def test_wrong_attribute(self):
        config = Config(config_file_path)
        self.assertEqual(False, config.get('General', 'Paperino'))


    def test_wrong_section(self):
        config = Config(config_file_path)
        self.assertEqual(False, config.get('Pluto', 'Paperino'))


    def test_wrong_section_no_attribute(self):
        config = Config(config_file_path)
        self.assertEqual(False, config.get('Pluto'))


    def test_get_section(self):
        config = Config(config_file_path)
        self.assertEqual(5, len(config.get('MySql')))


    def test_set_and_get_value(self):
        config = Config(config_file_path)
        config.set('General', 'batchsize', 666)
        self.assertEqual(666, config.get('General', 'batchsize'))


    def test_int_cast(self):
        config = Config(config_file_path)
        config.set('General', 'mjdref', 666.666)
        self.assertEqual(True, isinstance(config.get('General', 'mjdref', 'int'), int))
        self.assertEqual(666, config.get('General', 'mjdref', 'int'))


    def test_float_cast(self):
        config = Config(config_file_path)
        config.set('General', 'mjdref', 666.666)
        self.assertEqual(True, isinstance(config.get('General', 'mjdref', 'float'), float))
        self.assertEqual(666.666, config.get('General', 'mjdref', 'float'))

    def test_bool_true_cast(self):
        config = Config(config_file_path)
        config.set('General', 'debug', 'yes')
        self.assertEqual(True, isinstance(config.get('General', 'debug', 'bool'), bool))
        self.assertEqual(True, config.get('General', 'debug', 'bool'))

    def test_bool_false_cast(self):
        config = Config(config_file_path)
        config.set('General', 'debug', 'alksjdlkasjd')
        self.assertEqual(True, isinstance(config.get('General', 'debug', 'bool'), bool))
        self.assertEqual(False, config.get('General', 'debug', 'bool'))

if __name__ == '__main__':

    unittest.main()
