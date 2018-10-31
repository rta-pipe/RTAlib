#!/bin/sh

if [ "$1" = "--help" ] ; then
  printf "\nThe following script will create databases, users and tables needed by the RTAlib"
  printf "\n"
  return;
fi

if [ -z "$1" ]
  then
    host=localhost
  else
    host=$1
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

printf '\nPlease enter the name of the database you wish to create > '
read dbname

printf '\nPlease enter the name of a Mysql user that has enough privileges to create a database > '
read user

testuser=rtalib_test_user_$dbname
testuserpsw="!A$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c 5)2018@"
rtalibuser=rtalib_user_$dbname
rtalibuserpsw="!A$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c 5)2018@"

#__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\

find $SCRIPT_DIR -name '*tmp.sql' -delete

# Executing each sql script in mysql/
for mysql_script in $SCRIPT_DIR/mysql/*.sql; do

  mysql_script_tmp="${mysql_script%.*}_tmp.sql";

  sed -e 's/__DBNAME__/'$dbname'/g' $mysql_script > $mysql_script_tmp;

  sed -i -e 's/__TESTUSER__/'$testuser'/g' $mysql_script_tmp;
  sed -i -e 's/__TESTUSERPSW__/'$testuserpsw'/g' $mysql_script_tmp;

  sed -i -e 's/__RTALIBUSER__/'$rtalibuser'/g' $mysql_script_tmp;
  sed -i -e 's/__RTALIBUSERPSW__/'$rtalibuserpsw'/g' $mysql_script_tmp;

  sed -i -e 's/__HOST__/'$host'/g' $mysql_script_tmp;

  printf '\n --> Executing: '$mysql_script_tmp
  printf '\n'

  mysql --user=$user -p -v < $mysql_script_tmp;

  if [ $? -ne 0 ]; then
    printf "\n --> Something went wrong..quitting..\n\n"
    return;
  fi
done

find $SCRIPT_DIR -name '*tmp.sql' -delete





#__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\
# Creating the configuration file for the test environment

mkdir -p $SCRIPT_DIR/../Configs/

testdb="$dbname"_testing

> $SCRIPT_DIR/../Configs/rtalibconfig_testing
echo "[General]
modelname=rtalib_dl_test_table
mjdref=50000
debug=no
batchsize=1
numberofthreads=1

[Dtr]
active=no
debug=no
inputchannel=dtr.input
outputchannel=dtr.output

[MySql]
host=127.0.0.1
username=$testuser
password=$testuserpsw
dbname=$testdb

[Redis]
host=127.0.0.1
password=
dbname=1
indexon=rtalib_dl_test_table:timerealtt,rtalib_test_table:a

[MySqlPipelineDatabase]
active=no
debug=no
host=
username=
password=
dbname=
" >> $SCRIPT_DIR/../Configs/rtalibconfig_testing



#__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\__/\
# Creating the configuration file for the production environment -> one for each datamodel

tables=$(mysql --user="$rtalibuser" --password="$rtalibuserpsw" --database="$dbname" --execute="SELECT table_name FROM information_schema.tables where table_schema='$dbname';")

isFirst=1
for tableName in $tables;
  do
    if [[ "$isFirst" -eq 0 ]];
      then
        > $SCRIPT_DIR/../Configs/rtalibconfig_$tableName
        echo "[General]
modelname=$tableName
mjdref=
debug=
batchsize=
numberofthreads=

[Dtr]
active=no
debug=no
inputchannel=
outputchannel=

[MySql]
host=127.0.0.1
username=$rtalibuser
password=$rtalibuserpsw
dbname=$dbname

[Redis]
host=127.0.0.1
password=
dbname=1
indexon=rtalib_dl_test_table:timerealtt,rtalib_test_table:a

[MySqlPipelineDatabase]
active=no
debug=no
host=
username=
password=
dbname=
" >> $SCRIPT_DIR/../Configs/rtalibconfig_$tableName

    printf '\n  * Created rtalibconfig_'$tableName' file in RTAlib/Configs/'

    fi
    isFirst=0

done

printf '\n  * Created rtalibconfig_testing file in RTAlib/Configs/'


for mysql_script in mysql/*.sql; do
  printf '\n  * Executed: '$mysql_script
done
printf '\n'

printf '\n\n --> Please, fill in the configuration file fields!\n\n'
