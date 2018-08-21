# RTAlib



# PyRTAlib

## Installation
* Create a virtualenv
* Install setuptools
* python setup.py install  --> TODO <--

The last command will install the required dependencies: mysql-connector and redis for the production environment,
astropy, matplotlib, ... for the testing environment.

## General usage
* Specify the location of the config file
* Create a RTA_DL_DB object of the right subclass
* Send events to the RTA pipeline with insertEvent()
* Close connection with close()

### Example 1
The RTACONFIGFILE environment variabile is used to specify the configuration file path.
```
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB

os.environ['RTACONFIGFILE'] = './path/to/config/file'
RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
RTA_DL3ASTRI.insertEvent( evtid, eventidfits, observationid, datarepositoryid, ra_deg...)
RTA_DL3ASTRI.close()
```



### Example 2
It is possibile to hard-code the configuration file path directly in the RTA_DL_DB constructor.
```
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB

RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql', './path/to/config/file')
RTA_DL3ASTRI.insertEvent( evtid, eventidfits, observationid, datarepositoryid, ra_deg...)
RTA_DL3ASTRI.close()
```

### Example 3
The close() method is implicitally called as the execution flow exits from the 'with' statement.
```
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB

with RTA_DL3ASTRI_DB('mysql', './path/to/config/file') as RTA_DL3ASTRI:
  RTA_DL3ASTRI.insertEvent( evtid, eventidfits, observationid, datarepositoryid, ra_deg...)
```

## Tests
In order to be able to run the tests:
* rtalibconfigfortest must be configured with the following parameters: --> TODO <--
* a database for testing purpose must be setted up calling the following script: --> TODO <--

### unit test
```
python unit_test.py -v
```

### performance test (single threading)
```
python performance_test.py redis-basic /home/cta/pks1510-089.fits 1000
```

### performance test (multi threading)
```
python performance_test_multithreading.py /home/cta/pks1510-089.fits 1000 2
```

### cprofile
```
python performance_cprofile_test.py /home/cta/pks1510-089.fits
```
