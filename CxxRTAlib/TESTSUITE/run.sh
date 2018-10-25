#!/bin/sh

export GTEST_DIR=./googletest-1.8.1/googletest
export MYSQL_CXX_CNT=/home/zollino/CxxRTAlib_External_Dependencies/mysql-connector-cpp-install/
export BOOST_PATH=/home/zollino/CxxRTAlib_External_Dependencies/boost_1_67_0
export REDIS=/home/zollino/CxxRTAlib_External_Dependencies/hiredis
export LD_LIBRARY_PATH=/home/zollino/CxxRTAlib_External_Dependencies/mysql-connector-cpp-install/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/zollino/CxxRTAlib_External_Dependencies/hiredis:$LD_LIBRARY_PATH

make clean
make
