#!/bin/sh


if [ $# -eq 0 ]
  then
    echo "Insert 'mysql' or 'redis-basic' argument"
    return
fi

.CxxRTAlib/bin/testRTAlib  $1 ./  10
