from PyLibRTA.DBConnectors import RedisDBConnector, MySqlDBConnector
from PyLibRTA.Utils import ConfigFileParser

print("\n[ ] Redis connection test..")
redisConn = RedisDBConnector('localhost')
redisConn.connect()
assert redisConn.testConnection() == True
print("[X] Redis connection test passed")

print("\n[ ] MySql connection test..")
mysqlConn = MySqlDBConnector('localhost', 'zollino', 'Giancarlo@2018#')
assert mysqlConn.connect('evt_test') == True
assert mysqlConn.testConnection() == True
print("[X] MySql connection test passed")


print("\n[ ] Parser test..")
assert ConfigFileParser.parseRTALIBConfigFile() == False
data = ConfigFileParser.parseRTALIBConfigFile('/home/cta/RTAlib')
print(data)
import os
os.environ["RTACONFIGFILE"] = "/home/cta/RTAlib"
data = ConfigFileParser.parseRTALIBConfigFile()
print(data)
print("[X] Parser test passed")
