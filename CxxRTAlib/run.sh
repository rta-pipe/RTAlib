#!/bin/sh


if [ $# -eq 0 ]
  then
    echo "Insert 'mysql' or 'redis-basic' argument"
    return
fi

./bin/testRTAlib  $1 ../../PyRTAlib 5000
