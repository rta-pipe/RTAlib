#!/bin/bash

command -v coverage  >/dev/null 2>&1 || { echo >&2 "I require 'coverage' but it's not installed. Please install it or activate a virtual environment. Aborting."; return; }

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

mkdir -p $DIR/cov_results
COV_RESULTS=$DIR/cov_results
UNIT_TEST_DIR=$DIR/../unit_tests


# ==========> Config.py <==============
export COVERAGE_FILE=$COV_RESULTS/.coverage.results_Config
coverage run $UNIT_TEST_DIR/Config_unittest.py -v
ret=$?
if [ $ret -ne 0 ]; then
     echo "==> Error on Config_unittest ret=$ret"
     export EXIT_CODE=$ret
     return;
fi
#coverage report


# ==========> MySqlDBConnector.py <==============
export COVERAGE_FILE=$COV_RESULTS/.coverage.results_MySqlDBConnector
coverage run $UNIT_TEST_DIR/MySqlDBConnector_unittest.py -v
ret=$?
if [ $ret -ne 0 ]; then
     echo "==> Error on MySqlDBConnector_unittest ret=$ret"
     export EXIT_CODE=$ret
     return;
fi
#coverage report


# ==========> RedisDBConnector.py <==============
export COVERAGE_FILE=$COV_RESULTS/.coverage.results_RedisDBConnector
coverage run $UNIT_TEST_DIR/RedisDBConnector_unittest.py -v
ret=$?
if [ $ret -ne 0 ]; then
     echo "==> Error on RedisDBConnector_unittest ret=$ret"
     export EXIT_CODE=$ret
     return;
fi
#coverage report









export EXIT_CODE=0

printf "\n\nTEST COVERAGE RESULTS\n $exit_code_config $exit_code_mysql $exit_code_redis"

export COVERAGE_FILE=$DIR/.coverage.combined
coverage combine $COV_RESULTS/.coverage.results_Config $COV_RESULTS/.coverage.results_MySqlDBConnector $COV_RESULTS/.coverage.results_RedisDBConnector
coverage report

coverage xml
coverage html
coverage annotate -d coverage_annotated_sources
