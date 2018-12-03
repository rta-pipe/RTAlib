[![Codacy Badge](https://api.codacy.com/project/badge/Grade/02291c119ef146c68d58ce74876871e4)](https://www.codacy.com/app/Leofaber/RTAlib?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=rta-pipe/RTAlib&amp;utm_campaign=Badge_Grade)
[![Maintainability](https://api.codeclimate.com/v1/badges/a99a88d28ad37a79dbf6/maintainability)](https://codeclimate.com/github/codeclimate/codeclimate/maintainability)
[![Test Coverage](https://api.codeclimate.com/v1/badges/a99a88d28ad37a79dbf6/test_coverage)](https://codeclimate.com/github/codeclimate/codeclimate/test_coverage)
# CxxRTAlib
The C++ version of the RTAlib.

## External dependencies
* MySql server (Ver 14.14 Distrib 5.7+ for Linux (x86_64))
* MySql client
* Redis server (4.0.10+)

## Dependencies
The following dependencies are needed:
* C++11
* cmake
* Boost
* cfitsio
* MySql C++ Connector
* hiredis
* RTAPrototype2
* googletest

## Installation

* Home directory
```bash
  export HOME=~/
```

* Create folders for sources
```bash
  mkdir -p ~/rtalib_dep_libs/libraries_sources
  mkdir -p ~/rtalib_dep_libs/libraries_building_area
  mkdir -p ~/rtalib_dep_libs/libraries_installed
```

* Install cmake library:
```bash
  mkdir -p ~/rtalib_dep_libs/libraries_building_area/cmake-3.12.3
  mkdir -p ~/rtalib_dep_libs/libraries_installed/cmake-3.12.3
  cd ~/rtalib_dep_libs/libraries_sources
  wget https://cmake.org/files/v3.12/cmake-3.12.3.tar.gz
  cd ~/rtalib_dep_libs/libraries_building_area
  tar -zxvf ~/rtalib_dep_libs/libraries_sources/cmake-3.12.3.tar.gz
  cd cmake-3.12.3
  ./bootstrap --prefix=~/rtalib_dep_libs/libraries_installed/cmake-3.12.3
  make
  make install
  export CMAKE=~/rtalib_dep_libs/libraries_installed/cmake-3.12.3/bin/
  cd ~/
```

* Install Boost:
```bash
  mkdir -p ~/rtalib_dep_libs/libraries_building_area/boost_1_67_0
  mkdir -p ~/rtalib_dep_libs/libraries_installed/boost_1_67_0
  cd ~/rtalib_dep_libs/libraries_sources
  wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.bz2
  cd ~/rtalib_dep_libs/libraries_building_area
  tar --bzip2 -xf ~/rtalib_dep_libs/libraries_sources/boost_1_67_0.tar.bz2
  cd ~/rtalib_dep_libs/libraries_building_area/boost_1_67_0/
  ./bootstrap.sh --prefix=$HOME/rtalib_dep_libs/libraries_installed/boost_1_67_0
  ./b2 install
  cd ~/
```

* Install MySql C++ Connector:
```bash
  mkdir -p ~/rtalib_dep_libs/libraries_building_area/mysql-connector-cpp-build
  mkdir -p ~/rtalib_dep_libs/libraries_installed/mysql-connector-cpp-install
  cd ~/rtalib_dep_libs/libraries_sources
  git clone https://github.com/mysql/mysql-connector-cpp.git
  cd mysql-connector-cpp
  git checkout 8.0
  cd ~/rtalib_dep_libs/libraries_building_area/mysql-connector-cpp-build
  cmake -DCMAKE_INSTALL_PREFIX=~/rtalib_dep_libs/libraries_installed/mysql-connector-cpp-install/ -DBOOST_ROOT=~/rtalib_dep_libs/libraries_installed/boost_1_67_0 -DCMAKE_BUILD_TYPE=Release ~/rtalib_dep_libs/libraries_sources/mysql-connector-cpp/
  cmake --build . --target install --config Release
  cd ~/
```

* Install hiredis library:
```bash
  cd ~/rtalib_dep_libs/libraries_installed
  git clone https://github.com/redis/hiredis.git
  cd hiredis
  make
  cp libhiredis.so libhiredis.so.0.14
  cd ~/
```

* Install cfitsio library:
```bash
  mkdir -p ~/rtalib_dep_libs/libraries_installed/cfitsio_install
  cd ~/rtalib_dep_libs/libraries_sources
  wget http://heasarc.gsfc.nasa.gov/FTP/software/fitsio/c/cfitsio3450.tar.gz
  tar -zxvf cfitsio3450.tar.gz
  cd cfitsio
  ./configure --prefix=$HOME/rtalib_dep_libs/libraries_installed/cfitsio_install
  make
  make shared
  make install
  cd ~/
```

* Install RTA-Prototype
```bash
  cd ~/rtalib_dep_libs/libraries_installed
  git clone https://github.com/ASTRO-BO/RTAPrototype2-Setup
  cd RTAPrototype2-Setup
  git submodule update --init
  rm -r RTAPrototype2
  LIBRARY_PATH=$HOME/rtalib_dep_libs/libraries_installed/cfitsio_install/lib:$LIBRARY_PATH
  LD_LIBRARY_PATH=$HOME/rtalib_dep_libs/libraries_installed/cfitsio_install/lib:$LD_LIBRARY_PATH
  CPLUS_INCLUDE_PATH=$HOME/rtalib_dep_libs/libraries_installed/cfitsio_install/include:$CPLUS_INCLUDE_PATH
  C_INCLUDE_PATH=$HOME/rtalib_dep_libs/libraries_installed/cfitsio_install/include:$C_INCLUDE_PATH
  CFITSIO_LIBDIR=$HOME/rtalib_dep_libs/libraries_installed/cfitsio_install/lib:$CFITSIO_LIBDIR
  CFITSIO_INCDIR=$HOME/rtalib_dep_libs/libraries_installed/cfitsio_install/lib:$CFITSIO_INCDIR
  ./install
  cd ~/
```

* Install GoogleTest
```bash
  mkdir -p ~/rtalib_dep_libs/libraries_building_area/google-test-1.8.1
  mkdir -p ~/rtalib_dep_libs/libraries_installed/google-test-1.8.1-install
  cd ~/rtalib_dep_libs/libraries_sources
  git clone https://github.com/google/googletest.git
  cd googletest/
  git checkout release-1.8.1
  cd ~/rtalib_dep_libs/libraries_building_area/google-test-1.8.1
  cmake -DCMAKE_INSTALL_PREFIX=~/rtalib_dep_libs/libraries_installed/google-test-1.8.1-install ~/rtalib_dep_libs/libraries_sources/googletest
  make
  make install
```

## Compilation
```bash
  cd ~/RTAlib/CxxRTAlib
  export REDIS=~/rtalib_dep_libs/libraries_installed/hiredis
  export MYSQL_CXX_CNT=~/rtalib_dep_libs/libraries_installed/mysql-connector-cpp-install
  export BOOST_PATH=~/rtalib_dep_libs/libraries_installed/boost_1_67_0
  export CTA_ALGORITHMS=~/rtalib_dep_libs/libraries_installed/RTAPrototype2-Setup/CTAAlgorithms/
  export CTA_UTILS=~/rtalib_dep_libs/libraries_installed/RTAPrototype2-Setup/CTAUtils/
  export GTEST_DIR=~/rtalib_dep_libs/libraries_installed/google-test-1.8.1-install
  make
```
The compilation step will produce a static library and the executables for running the tests.

## Running the tests
In order to run the executables, define the following environment variables:
```bash
  export RTALIBDIR=/path/to/root/RTAlib
  export LD_LIBRARY_PATH=~/rtalib_dep_libs/libraries_installed/mysql-connector-cpp-install/lib64/:$LD_LIBRARY_PATH
  export LD_LIBRARY_PATH=~/rtalib_dep_libs/libraries_installed/hiredis:$LD_LIBRARY_PATH
```
### Unit testing
```bash
  bash ~/RTAlib/CxxRTAlib/TestEnvironment/bin/unitTest
```

### Performance test
```bash
  bash ~/RTAlib/CxxRTAlib/TestEnvironment/bin/perfTest <params>
```

### Stand alone usage
To execute CxxRTAlib simulation run:
```bash
  bash ~/RTAlib/CxxRTAlib/TestEnvironment/bin/rtaLibSim  mysql ../path/to/rtalibconfig  eventsNumber
```

## Test coverage
https://stackoverflow.com/questions/2359344/google-test-code-coverage
