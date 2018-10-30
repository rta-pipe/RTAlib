import os
import sys

from .Generic import Singleton


class Config(metaclass=Singleton):

    def __init__(self, filepath=''):
        self.configs = self.parseConfigFile(filepath)

    def get(self, sectionName = '', attributeName = '', cast=''):

        if sectionName and attributeName:

            if sectionName in self.configs:

                if attributeName in self.configs[sectionName]:

                    val = self.configs[sectionName][attributeName]
                else:
                    return False
            else:
                return False


        elif sectionName and not attributeName:

            if sectionName in self.configs:

                return self.configs[sectionName]

            else:
                return False
        else:
            return self.configs


        if cast=='int':
            return int(val)

        elif cast=='float':
            return float(val)

        elif cast=='bool':

            if val == 'True' or val == True or val == 'yes' or val == 'y':
                return True
            else:
                return False

        elif cast=='dict':

            data = {}
            elements = val.split(',')

            for e in elements:
                kv = e.split(':')
                key = kv[0]
                value = kv[1]
                data[key]=value

            return data
        else:
            return val


    def set(self, sectionName, attributeName, attributeValue):
        self.configs[sectionName][attributeName] = attributeValue
        return self

    def reload(self, filepath=''):
        self.configs = self.parseConfigFile(filepath)

    def parseConfigFile(self, filepath=''):
        if 'RTACONFIGFILE' in os.environ:
            filepath = os.environ['RTACONFIGFILE']
        else:
            if not filepath:
                raise Exception('[Config] Cant configure. Neither the filepath parameter or the RTACONFIGFILE environment variable have been provided.')


        filepath += '/rtalibconfig_testing'

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
