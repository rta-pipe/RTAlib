#!/bin/bash

command -v python  >/dev/null 2>&1 || { echo >&2 "I require 'python' but it's not installed. Please install it or activate a virtual environment. Aborting."; return; }

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

cat << EOF

 __/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__
|               RTAlib unit testing                |
|__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__|

Hi! The following script will execute the RTAlib unit tests.

EOF


if [[ -z "${RTALIBCONFIG}" ]]; then
    printf "  --> No RTALIBCONFIG env var found.\n      Please execute 'export RTALIBCONFIG=/path/to/rtalibconfig_testing' , or\n";
    printf "      please enter the absolute path to your 'rtalibconfig_testing' configuration file > "
    read rtalibconfig_path
    export RTALIBCONFIG=$rtalibconfig_path

  else
    echo "RTALIBCONFIG is set to '$RTALIBCONFIG'";
fi



cd $SCRIPT_DIR

for f in $SCRIPT_DIR/*_unittest.py; do
  printf "\n\n";
  printf "      ===> Unit testing for:  $f \n\n";
  python "$f" -v;
  printf "\n\n";
  read -p "   -> Press enter to continue"
done
