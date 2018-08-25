import os
import sys

from .Generic import Singleton


class Config(metaclass=Singleton):

    def __init__(self, filepath='', load=True):
        if load:
            self.configs = self.parseConfigFile(filepath)
        else:
            self.configs = {}

    def get(self, sectionName = '', attributeName = '', cast=''):
        if sectionName and attributeName:
            val = self.configs[sectionName][attributeName]
        elif sectionName and not attributeName:
            val = self.configs[sectionName]
        else:
            val = self.configs

        if cast=='int':
            return int(val)
        elif cast=='float':
            return float(val)
        else:
            return val


    def set(self, sectionName, attributeName, attributeValue):
        self.configs[sectionName][attributeName] = attributeValue

    def reload(self, filepath=''):
        self.configs = self.parseConfigFile(filepath)

    def parseConfigFile(self, filepath=''):
        if 'RTACONFIGFILE' in os.environ:
            filepath = os.environ['RTACONFIGFILE']
        else:
            if not filepath:
                raise Exception('[Config] Cant configure. Neither the filepath parameter or the RTACONFIGFILE environment variable have been provided.')


        filepath += '/rtalibconfig'

        configs = {}

        try:
            with open(filepath) as f:
                content = f.readlines()
                content = [x.strip() for x in content]

                sectionTempName = ''
                sectionTempValue = ()
                for line in content:
                    if line and (line.find('[') != -1) :
                        sectionTempName = self.parseSectionName(line)
                        configs[sectionTempName] = {}
                    elif line and (line.find('=') != -1) :
                        sectionValue = self.parseLine(line)
                        if sectionValue[1]:
                            configs[sectionTempName][sectionValue[0]] = sectionValue[1]
                    #else:
                        #print('Empty line')
        except FileNotFoundError:
            raise FileNotFoundError('[Config] Cant configure. File not found at {}'.format(filepath))


        return configs


    def parseSectionName(self, line):
        start = line.find('[')
        end = line.find(']', start)
        return line[start+1:end]

    def parseLine(self, line):
        kv = line.split("=")
        return (kv[0],kv[1])
