from sys import argv
from PyLibRTA.DBConnectors import RedisDBConnector, MySqlDBConnector

if len(argv) == 1:
    print("Please insert database (redis or mysql)")
    exit()

database = argv[1]

if database == 'redis':
    redisConn = RedisDBConnector.RedisDBConnector('localhost')
    redisConn.connect()
    print(redisConn.testConnection())


elif database == 'mysql':
    mysqlConn = MySqlDBConnector.MySqlDBConnector('localhost', 'zollino', 'Giancarlo@2018#')
    mysqlConn.connect('evt_test')
    mysqlConn.testConnection()


else:
    print("Database {} not found.".format(database))
