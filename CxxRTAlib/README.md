# RTAlib

# CxxRTAlib
The C++ version of the RTAlib.

## Dependencies
The following dependencies are needed:
* C++11
* cmake >=2.18
* Boost C++ library
* cfitsio
* MySql Client library
* hiredis
* RTAPrototype2
* googletest

## Installation
* Install cmake library:
```
    wget https://cmake.org/files/v3.12/cmake-3.12.3.tar.gz
    tar -zxvf cmake-3.12.3.tar.gz
    cd cmake-3.12.3
    ./bootstrap --prefix=path/to/installation/folder/cmake-3.12.3
    make
    make install
```

* Install Boost C++ library:
```
  wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.bz2
  tar --bzip2 -xf boost_1_67_0.tar.bz2
  cd boost_1_67_0/tools/build
  . bootstrap.sh
  ./b2 install --prefix=/path/to/installation/folder/boost_1_67_0
```

* Install MySql C++ Connector:
```
  git clone https://github.com/mysql/mysql-connector-cpp.git
  mkdir mysql-connector-cpp-install
  mkdir mysql-connector-cpp-build
  cd mysql-connector-cpp
  git checkout 8.0
  cd ../mysql-connector-cpp-build
  path/to/installation/folder/cmake-3.12.3/bin/cmake -DCMAKE_INSTALL_PREFIX=../mysql-connector-cpp-install -DBOOST_ROOT=/path/to/installation/folder/boost_1_67_0 ../mysql-connector-cpp
  cmake --build . --config Release
  cmake --build . --target install --config Release

```

* Install hiredis library:
```
  git clone https://github.com/redis/hiredis.git
  cd /path/to/hiredis/
  make
```

* Install cfitsio library:
```
    wget http://heasarc.gsfc.nasa.gov/FTP/software/fitsio/c/cfitsio3450.tar.gz
    tar -zxvf cfitsio3450.tar.gz
    cd cfitsio
    ./configure --prefix=/path/to/installation/folder/cfitsio
    make
    make install
```


## General usage
In order to obtain the googletest framework when cloning the project run:

git clone --recursive https://github.com/rta-pipe/RTAlib

In RTAlib/CxxRTAlib/ modify install_local.sh file in order to set correctly the environment variables:
```
    export MYSQL_CXX_CNT=/path/to/mysql-connector-cpp-install/
    export BOOST_PATH=/path/to/boost_1_67_0
    export REDIS=/path/to/hiredis
    export LD_LIBRARY_PATH=/path/to/mysql-connector-cpp-install/lib64:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/path/to/hiredis:$LD_LIBRARY_PATH
```

To execute CxxRTAlib simulation run:
./bin/testRTAlib  mysql ../path/to/rtalibconfig  eventsNumber

## TEST

### unit test
Use cmake to build googletest framework:
```
    cd CxxRTAlib/TESTSUITE/googletest-1.8.1/googletest/
    mkdir build
    cd build
    cmake ..
    make
```
In RTAlib/CxxRTAlib/TESTSUITE/ modify run.sh file in order to set correctly the environment variables:
```
    export MYSQL_CXX_CNT=/path/to/mysql-connector-cpp-install/
    export BOOST_PATH=/path/to/boost_1_67_0
    export REDIS=/path/to/hiredis
    export LD_LIBRARY_PATH=/path/to/mysql-connector-cpp-install/lib64:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/path/to/hiredis:$LD_LIBRARY_PATH
```
To execute unit test run: bin/unitTest

### Test coverage
https://stackoverflow.com/questions/2359344/google-test-code-coverage
