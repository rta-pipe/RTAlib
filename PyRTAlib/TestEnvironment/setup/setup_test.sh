if [ $# -eq 0 ]
  then
    echo "No arguments supplied. Please, provide a mysql username that has enough privileges to create a database."
    return
fi

user=$1

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
echo $DIR
# Setup mysql db
mysql -u $user -p < $DIR/setup_test_db.sql


# Create rtalibconfig configuration file
> rtalibconfig
echo "[General]
modelname=rtalib_dl_test_table
mjdref=50000
debug=no
batchsize=1
numberofthreads=1

[Dtr]
active=no
debug=yes
inputchannel=dtr.input
outputchannel=dtr.output

[MySql]
host=127.0.0.1
username=rtalib_test_user
password=#Rtalibtest@2018
dbname=rtalib_test_db

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
" >> rtalibconfig


echo "--> rtalib_test_db database created."
echo "--> rtalib_test_user user created."
echo "--> rtalibconfig configuration file created."
