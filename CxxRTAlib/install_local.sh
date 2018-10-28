#!/bin/sh


if [ -z "$1" ]
  then
    echo "No argument supplied -> DEBUG MODE = OFF"
  else
    echo "One argument supplied -> DEBUG MODE = ON"
    export DEBUG=1
fi

export CTA_UTILS=/home/zollino/development/working/RTAlib/CxxRTAlib/CxxRTAlib/RTAPrototype2-Setup/CTAUtils/
export CTA_ALGORITHMS=/home/zollino/development/working/RTAlib/CxxRTAlib/CxxRTAlib/RTAPrototype2-Setup/CTAAlgorithms/
export MYSQL_CXX_CNT=/home/zollino/CxxRTAlib_External_Dependencies/mysql-connector-cpp-install/
export BOOST_PATH=/home/zollino/CxxRTAlib_External_Dependencies/boost_1_67_0
export REDIS=/home/zollino/CxxRTAlib_External_Dependencies/hiredis
export LD_LIBRARY_PATH=/home/zollino/CxxRTAlib_External_Dependencies/mysql-connector-cpp-install/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/zollino/CxxRTAlib_External_Dependencies/hiredis:$LD_LIBRARY_PATH


make clean
make
