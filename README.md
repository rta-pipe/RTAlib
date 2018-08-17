# RTAlib



## PyRTAlib
### General usage
* Specify the location of the config file
* Create a RTA_DL_DB object of the right subclass
* Send events to the RTA pipeline with insertEvent()
* Close connection with close()

### Usage 1
The RTACONFIGFILE environment variabile is used to specify the configuration file path.
```
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB

os.environ['RTACONFIGFILE'] = './path/to/config/file'
RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql')
RTA_DL3ASTRI.insertEvent( evtid, eventidfits, observationid, datarepositoryid, ra_deg...)
RTA_DL3ASTRI.close()
```



### Usage 2
It is possibile to hard-code the configuration file path directly in the RTA_DL_DB constructor.
```
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB

RTA_DL3ASTRI = RTA_DL3ASTRI_DB('mysql', './path/to/config/file')
RTA_DL3ASTRI.insertEvent( evtid, eventidfits, observationid, datarepositoryid, ra_deg...)
RTA_DL3ASTRI.close()
```

### Usage 3
The close() method is implicitally called as the execution flow exits from the 'with' statement.
```
from PyRTAlib.RTAInterface  import RTA_DL3ASTRI_DB

with RTA_DL3ASTRI_DB('mysql', './path/to/config/file') as RTA_DL3ASTRI:
  RTA_DL3ASTRI.insertEvent( evtid, eventidfits, observationid, datarepositoryid, ra_deg...)
```
