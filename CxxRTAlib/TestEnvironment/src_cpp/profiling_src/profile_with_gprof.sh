#!/bin/bash

export LD_LIBRARY_PATH=~/rtalib_dep_libs/libraries_installed/mysql-connector-cpp-install/lib64/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=~/rtalib_dep_libs/libraries_installed/hiredis:$LD_LIBRARY_PATH

../../bin/rtaDlTestDbProf

gprof ../../bin/rtaDlTestDbProf gmon.out
