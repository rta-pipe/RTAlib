#!/bin/bash

command -v python  >/dev/null 2>&1 || { echo >&2 "I require 'python' but it's not installed. Please install it or activate a virtual environment. Aborting."; return; }

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

cd $SCRIPT_DIR

for f in $SCRIPT_DIR/*_unittest.py; do
  printf "\n\n";
  printf "      ===> Unit testing for:  $f \n\n";
  python "$f" -v;
  printf "\n\n";
  read -p "   -> Press enter to continue"
done

cd
