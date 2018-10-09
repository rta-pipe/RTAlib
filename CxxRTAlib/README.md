# RTAlib

# CxxRTAlib
The C++ version of the RTAlib.

## Dependencies
The following dependencies are needed:
* C++11
* cmake >=2.18
* MySql Client library
* Boost C++ library
* hiredis

## Installation
* Install Boost C++ library:
```wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.bz2
  tar --bzip2 -xf boost_1_67_0.tar.bz2
  cd boost_1_67_0/tools/build
  . bootstrap.sh
```

* Install MySql C++ Connector:
```git clone https://github.com/mysql/mysql-connector-cpp.git
  mkdir mysql-connector-cpp-install
  cd mysql-connector-cpp
  git checkout 1.1
  cmake -DCMAKE_INSTALL_PREFIX=/path/to/mysql-connector-cpp-install
  -DBOOST_ROOT=/path/to/boost_1_67_0
  ccmake ./    (to check the compilation variables)
  make
  make install
```

* Install hiredis library:
```https://github.com/redis/hiredis.git
  cd /path/to/hiredis/
  make
```

## General usage
In order to obtain the googletest framework when cloning the project run:
```git clone --recursive
```
In RTAlib/CxxRTAlib/ modify install_local.sh file in order to set correctly the environment variables:
``` export MYSQL_CXX_CNT=/path/to/mysql-connector-cpp-install/
    export BOOST_PATH=/path/to/boost_1_67_0
    export REDIS=/path/to/hiredis
    export LD_LIBRARY_PATH=/path/to/mysql-connector-cpp-install/lib64:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/path/to/hiredis:$LD_LIBRARY_PATH
```
To execute CxxRTAlib simulation run:
```\./bin/testRTAlib  mysql ../path/to/rtalibconfig  eventsNumber
```

## TEST

### unit test
In RTAlib/CxxRTAlib/TESTSUITE/ modify run.sh file in order to set correctly the environment variables:
``` export MYSQL_CXX_CNT=/path/to/mysql-connector-cpp-install/
    export BOOST_PATH=/path/to/boost_1_67_0
    export REDIS=/path/to/hiredis
    export LD_LIBRARY_PATH=/path/to/mysql-connector-cpp-install/lib64:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/path/to/hiredis:$LD_LIBRARY_PATH
```
To execute unit test run: bin/unitTest
```
```
