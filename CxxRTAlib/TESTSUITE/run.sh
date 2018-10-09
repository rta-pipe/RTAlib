#!/bin/sh

export GTEST_DIR=/home/zollino/development/working/RTAlib/CxxRTAlib/TESTSUITE/googletest-1.8.1/googletest
export MYSQL_CXX_CNT=/home/zollino/mysql-connector-cpp-install/
export BOOST_PATH=/home/zollino/boost_1_67_0
export REDIS=/home/zollino/hiredis
export LD_LIBRARY_PATH=/home/zollino/mysql-connector-cpp-install/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/zollino/hiredis:$LD_LIBRARY_PATH
export DEBUG=1

make clean
make
