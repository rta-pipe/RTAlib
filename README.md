# RTAlib

## Dependencies
In order to execute succesfully the library code, the following softwares are needed:
* MySql (14.14+)
* Redis (4.0.10+)

# PyRTAlib
The Python version of the RTAlib.

## Description
TODO

## Installation
* Create a virtualenv
* Install setuptools
* python setup.py install  --> TODO <--
* Create script to initialize db and test environment --> TODO <--
* Fill in the configuration file's fields.

The last command will install the required dependencies: mysql-connector and redis for the production environment,
astropy, matplotlib, ... for the testing environment. (TODO production - development separation)

## Configuration options
```
[General]
evt3modelname=     # the name of the mysql table or of the redis zset/hashset
mjdref=            # MJDREFI+MJDREFF
debug=             # yes/no
batchsize=         # performance tuning parameter: the input streaming is writed to db with a batch strategy (if batchsize > 1)
numberofthreads=   # performance tuning parameter: more than one thread may help to sustain a high-rate input streaming

[MySql]
host=
username=
password=
dbname=

[Redis]
host=
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

```python
class RTA_DL3ASTRI_DB.insertEvent(eventidfits, time, ra_deg, dec_deg, energy, detx, dety, mcid, observationid = 0, datarepositoryid = 0, status = 1)
```
Implements the the *class RTA_DL_DB.insertEvent(°args)* abstract method of the base class.

```python
class RTA_DL3ASTRI_DB.waitAndClose()
```
TODO
If the multithreading mode has been set, it returns:
* (totalEvents, executionTime, eventRate)


```python
class RTA_DL3ASTRI_DB.forceClose()
```
TODO



## Tests
In order to be able to run the tests:
* rtalibconfigfortest must be configured with the following parameters: --> TODO <--
* a database for testing purpose must be setted up calling the following script: --> TODO <--

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
