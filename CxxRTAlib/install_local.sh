#!/bin/sh


if [ -z "$1" ]
  then
    echo "No argument supplied -> DEBUG MODE = OFF"
  else
    echo "One argument supplied -> DEBUG MODE = ON"
    export DEBUG=1
fi

export CTA_UTILS=/home/zollino/RTAPrototype2-Setup/CTAUtils/
export CTA_ALGORITHMS=/home/zollino/RTAPrototype2-Setup/CTAAlgorithms/
export MYSQL_CXX_CNT=/home/zollino/mysql-connector-cpp-install/
export BOOST_PATH=/home/zollino/boost_1_67_0
export REDIS=/home/zollino/hiredis
export LD_LIBRARY_PATH=/home/zollino/mysql-connector-cpp-install/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/zollino/hiredis:$LD_LIBRARY_PATH


make clean
make
