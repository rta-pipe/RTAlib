[![Codacy Badge](https://api.codacy.com/project/badge/Grade/02291c119ef146c68d58ce74876871e4)](https://www.codacy.com/app/Leofaber/RTAlib?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=rta-pipe/RTAlib&amp;utm_campaign=Badge_Grade)
[![Maintainability](https://api.codeclimate.com/v1/badges/a99a88d28ad37a79dbf6/maintainability)](https://codeclimate.com/github/codeclimate/codeclimate/maintainability)
[![Test Coverage](https://api.codeclimate.com/v1/badges/a99a88d28ad37a79dbf6/test_coverage)](https://codeclimate.com/github/codeclimate/codeclimate/test_coverage)
# PyRTAlib
The Python version of the RTAlib.

## Dependencies
The following external dependencies are needed:
* Python 3.6+
* MySql server (Ver 14+ Distrib 5.7+, for Linux (x86_64))
* Redis server (4.0.10+)

## Features
The following features are supported:
- inserting data in a MySql/Redis database in a batch (using transactions) or a streaming fashion [A1], using a synchronous single-thread or asynchronous (single or multi-threads) strategies [A2].
- [B] possibility to update other tables at the end of each transaction.
- [C] using the Redis Pub/Sub mechanism to send messages and data into Redis channels to enable asynchronous communication between the RTAlib and other systems.
- [D] specifying the type of data with the implementation of the data model classes.
- [E] a stand alone deamon that uses Redis Pub/Sub mechanism, listens for the RTAlib data messages and forward those data messages to a [https://github.com/IftachSadeh/ctaOperatorGUI] (Graphic User Interface) displaying Data Quality components.

## Who implements the features
- DBConnectors
  - [A1] this module expose an interface that can be used to insert data in a database. Two databases are supported: MySql and Redis. Two types of insertion strategies can be adopted: batch-insert or streaming-insert.
- RTAInterface
  - [A2] the RTA_DL_DB.py base class implements the synchronous or asynchronous (even multi-threading) execution.
  - [B] The code to execute queries after every commit (when a transaction is closed) can be inserted within any class that iherits the RTA_DL_DB.py base class. This is available only in synchronous mode.
  - [C] the RTA_DL_DB.py can send data to a Redis channel specified in the configuration file (Dtr/inputchannel).
- DataModels [D]
  - this module contains all the classes that describe the data types that are stored in the database.

## Installation
* Clone the source:
  ```bash
    cd ~/
    git clone https://github.com/rta-pipe/RTAlib
  ```
* Create a python virtualenv:
  ```bash
    conda create --name rtalib python=3.6
  ```
* Activate the virtualenv:
  ```bash
    source activate rtalib
  ```
* Install dependencies with:
  ```Python
    cd ~/RTAlib/PyRTAlib
    python setup.py install
  ```
* Run the following script:
  ```bash
    cd ~/RTAlib/SetupDB
    . setup_db.sh
  ```
* Complete the fields of configuration files located under RTAlib/Configs

The setup_db.sh script will create 2 environment, one for testing and one for production.
For each environment the script will create:
* a mysql database
* a mysql user
* mysql tables
* a rtalibconfig under RTAlib/Configs



## Configuration options
There will be one rtalibconfig configuration file for each data model.

### General
* modelname: mysql table / redis key for the data model
* mjdref: MJDREFI+MJDREFF
* debug: yes/y/True/'True' or any other value for False
* batchsize: performance tuning parameter: =1 streaming strategy, >1 batch strategy
* numberofthreads: performance tuning parameter: more than one thread may help to sustain a high-rate input

### DTR
The DTR system...
* guiname:
* active: yes/y/True/'True' or any other value for False
* debug: yes/y/True/'True' or any other value for False
* inputchannel:
* outputchannel:

### MySql
host:
username: this field is filled by the setup_db.sh script
password: this field is filled by the setup_db.sh script
dbname: this field is filled by the setup_db.sh script

### Redis
host:
password:
dbname:
indexon: dictionary field (key:value,key:value)

### MySqlPipelineDatabase
active: yes/y/True/'True' or any other value for False
debug:  yes/y/True/'True' or any other value for False
host:
username:
password:
dbname:


## General usage
* Specify the location of the configuration file
* Create a RTA_DL_DB object of the right subclass specifying the database type
* Send events to the RTA pipeline with insertEvent()
* Close connection with close()

### Example

```python
from PyRTAlib.RTAInterface import RTA_DL3ASTRI_DB

# The RTALIBCONFIGFILE environment variabile is used to specify the configuration file path.
os.environ['RTALIBCONFIGFILE'] = './path/to/config/file'  

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

os.environ['RTALIBCONFIGFILE'] = './path/to/config/file'

with RTA_DL3ASTRI_DB('mysql') as RTA_DL3ASTRI:
    RTA_DL3ASTRI.insertEvent( evtid, eventidfits, observationid, datarepositoryid, ra_deg...)
```
the waitAndClose() method is implicitally called as the execution flow exits from the 'with' statement.

## Testing

### Unit testing
The classes under unit testing are:
* Config.py
* MySqlDBConnector.py
* RedisDBConnector.py

In order to run the unit tests you can use the following script:
```bash
  cd TestEnvironment/unit_tests
  . run_unit_tests.sh
```

### Performance test
This test will print the execution time (sec) and event rate (events/sec) of the library writing to the database a
fixed number of events. It is possibile to configure the runs modifying the configuration section within the source code. The output is shown in the console and written on a file in json format.

Arguments:
* database
* number of events
* the number of times a test must be repeated (>1)
* path to the configuration file

```python
python performance_test.py redis-basic 5000 5 ../../../Configs/rtalibconfig_testing
```

### Code coverage
The code coverage is measured with a synchronous single-thread execution.
```bash
cd TestEnvironment/code_coverage
. run_coverage_test.sh
```
### Check installation of Coverage library
The following command:
```python
coverage --version
```
should output:
```
Coverage.py, version 5.0a3 with C extension
Documentation at https://coverage.readthedocs.io/en/coverage-5.0a3
```
If the C extension is not present, you may need to install the python-dev and gcc support files before installing coverage. You can use:
```bash
sudo apt-get install python3-dev gcc
```
or
```bash
sudo yum install python3-devel gcc
```



## DTR
### Starting the DTR deamon
DTR is a standalone python deamon which purpose is to send to the scientific/data quality GUIs the data coming from the RTAlib. The RTAlib stores the DL1/DL2/DL3 events in a database and then, it publish those events in a Redis PubSub channel (the channel's name is specified in the configuration file). The DTR will listen for data on this channel and when an event arrives, the DTR push the event in a queue. An asynchronous thread will consume the queue and each event is transformed (in a data format for GUI visualization) and pushed into another Redis PubSub channel (GUIs channel).

In order to start the DTR, execute the startDTR.py script specifying the path to the configuration file.

```bash
  cd RTAlib/PyRTAlib
  python startDTR.py ./
```



## API
```python
class RTA_DL3ASTRI_DB.RTA_DL3ASTRI_DB(database, configFilePath = '')
```
Constructor for a RTA_DL3ASTRI_DB object.
Arguments:
* *database* (required) is a string that sets the database type.
* *configFilePath* (optional) is a string that specifies the location of the configuration file (it will be overrided by the RTALIBCONFIGFILE environment variabile, if set)
___
```python
class RTA_DL3ASTRI_DB.insertEvent(eventidfits, time, ra_deg, dec_deg, energy, detx, dety, mcid, observationid = 0, datarepositoryid = 0, status = 1)
```
Implements the the *class RTA_DL_DB.insertEvent(°args)* abstract method of the base class.
___
```python
class RTA_DL3ASTRI_DB.waitAndClose()
```
If the numberofthreads parameter is greater than 0, it returns a tuple:
* (totalEvents, executionTime, eventRate)
otherwise it return True or False.
___
