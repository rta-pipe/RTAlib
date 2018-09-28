# RTAlib

## Dependencies
In order to execute succesfully the library code, the following softwares are needed:
* Python 3.6+
* MySql (14.14+)
* Redis (4.0.10+)

# PyRTAlib
The Python version of the RTAlib.

## Features
The following features are supported:
- inserting data in a MySql/Redis database in a batch (using transactions) or a streaming fashion [A1], using a synchronous single-thread or asynchronous (single or multi-threads) strategies [A2].
- [B] possibility to update other tables at the end of each transaction.
- [C] using the Redis Pub/Sub mechanism to send messages and data into Redis channels to enable asynchronous communication between the RTAlib and other systems.
- [D] specifying the type of data with the implementation of the data model classes.
- [E] a stand alone deamon that uses Redis Pub/Sub mechanism, listens for the RTAlib data messages and forward those data message to a [https://github.com/IftachSadeh/ctaOperatorGUI](Graphic User Interface) displaying Data Quality components.

## Who implements the features
- DBConnectors
  - [A1] this module expose an interface that can be used to insert data in a database. Two databases are supported: MySql and Redis. Two types of insertion strategies can be adopted: batch-insert or streaming-insert.
- RTAInterface
  - [A2] the RTA_DL_DB.py base class implements the synchronous or asynchronous (even multi-threading) execution.
  - [B] the RTA_DL3ASTRI_DB.py can be used to execute queries after every commit (when a transaction is closed). This is available only in synchronous mode.
  - [C] the RTA_DL_DB.py can send data to the Redis channel specified in the configuration file.
- DataModels [D]
  - this module contains all the classes that describe the data types that are stored in the database.

## Installation
* Create a virtualenv
  * conda create --name rtalib python=3.6
* Activate the virtualenv
  * source activate rtalib
* python setup.py install
* Rename the configuration file from rtalibconfig_default to rtalibconfig.
* Fill in the configuration file's fields.



## Configuration options
```
[General]
evt3modelname=     # the name of the mysql table or of the redis zset/hashset
mjdref=            # MJDREFI+MJDREFF
debug=             # yes/y/True/'True' or any other value for False
batchsize=         # performance tuning parameter: the input streaming is writed to db with a batch strategy (if batchsize > 1)
numberofthreads=   # performance tuning parameter: more than one thread may help to sustain a high-rate input streaming

[Dtr]
active=            # yes/y/True/'True' or any other value for False
debug=

[MySql]
host=
username=
password=
dbname=

[Redis]
host=
password=
dbname=

[MySqlPipelineDatabase]
active=            # yes/y/True/'True' or any other value for False
debug=             # yes/y/True/'True' or any other value for False
host=
username=
password=
dbname=
```

## General usage
* Specify the location of the configuration file
* Create a RTA_DL_DB object of the right subclass specifying the database type
* Send events to the RTA pipeline with insertEvent()
* Close connection with close()

### Example

```python
from PyRTAlib.RTAInterface import RTA_DL3ASTRI_DB

# The RTACONFIGFILE environment variabile is used to specify the configuration file path.
os.environ['RTACONFIGFILE'] = './path/to/config/file'  

# In the class constructor is specified the database in which the events will be writed.
RTA_DL3ASTRI = RTA_DL3ASTRI_DB('redis-basic')

# The insertEvent(..) method writes the events in the database.
for i in range(10000):
  RTA_DL3ASTRI.insertEvent(..)

# If multithreading has been setted up, the waitAndClose() methods blocks the execution
# until all threads finish their jobs
RTA_DL3ASTRI.waitAndClose()
```

### Example 2
The library can be also used with the following, pythonic syntax:
```python
from PyRTAlib.RTAInterface import RTA_DL3ASTRI_DB

os.environ['RTACONFIGFILE'] = './path/to/config/file'

with RTA_DL3ASTRI_DB('mysql') as RTA_DL3ASTRI:
    RTA_DL3ASTRI.insertEvent( evtid, eventidfits, observationid, datarepositoryid, ra_deg...)
```
the waitAndClose() method is implicitally called as the execution flow exits from the 'with' statement.



## API
```python
class RTA_DL3ASTRI_DB.RTA_DL3ASTRI_DB(database, configFilePath = '', pure_multithreading = False)
```
Constructor for a RTA_DL3ASTRI_DB object.
Arguments:
* *database* (required) is a string that sets the database type.
* *configFilePath* (optional) is a string that specifies the location of the configuration file (it will be overrided by the RTACONFIGFILE environment variabile, if set)
* *pure_multithreading* (optional) is a boolean that specifies the *pure multithreading* mode. When the number of threads is configured equal to one, a False value means that the execution flow is synchronous i.e. no asynchronous threads are created.
___
```python
class RTA_DL3ASTRI_DB.insertEvent(eventidfits, time, ra_deg, dec_deg, energy, detx, dety, mcid, observationid = 0, datarepositoryid = 0, status = 1)
```
Implements the the *class RTA_DL_DB.insertEvent(°args)* abstract method of the base class.
___
```python
class RTA_DL3ASTRI_DB.waitAndClose()
```
TODO
If the multithreading mode has been set, it returns:
* (totalEvents, executionTime, eventRate)
___
```python
class RTA_DL3ASTRI_DB.forceClose()
```
TODO
___

## Setup the test environment (*NOT UPDATED*)
In order to be able to run the tests:
* rtalibconfig_test must be configured with the following parameters: --> TODO <--

* For now, create the database with:
  * mysql -uroot -p --execute="CREATE DATABASE 'evt_test';"
* import the tables from an existing backup with:
  * mysql -uroot -p evt_test < evt_test.sql
  
* a MySQL database for testing purpose must be setted up calling the following instructions:
  * CREATE DATABASE 'evt_test'
  * CREATE TABLE IF NOT EXISTS test_table ( id INT AUTO_INCREMENT PRIMARY KEY, a INTEGER, b INTEGER, c INTEGER, d INTEGER );
  * TABELLA PERFORMANCE --> TODO <-- (dump tabella originale?)
  * CREATE USER 'rta_test'@'localhost' IDENTIFIED BY 'Rtatest@2018#';
  * GRANT ALL PRIVILEGES ON evt_test.* to 'rta_test'@'localhost' identified by 'Rtatest@2018#'
* a Redis database for testing purpose must be setted up calling the following instructions:
  * redis-cli
  * auth <redis-password>
  * set indexstring:testmodel a
--> TODO <--

### unit test
Script:
* unit_test.py

Arguments:
* -v for verbose mode

```shell
python unit_test.py -v
```

### performance test (single and multithreading)
This test will print the execution time (sec) and event rate (events/sec) of the library writing to the database a
fixed number of events. It is possibile to configure the runs modifying the configuration section within the source code.

Scripts:
* performance_test.py
* performance_test_multithreading.py

Arguments:
* database
* file fits path
* number of events

```
python performance_test.py redis-basic /home/cta/pks1510-089.fits 5000
python performance_test_multithreading.py mysql /home/cta/pks1510-089.fits 5000

```


### cprofile
```
python performance_cprofile_test.py /home/cta/pks1510-089.fits
```
