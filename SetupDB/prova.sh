#!/bin/bash
dbname="rtalib"
n=$(mysql --user="rtalib_user" --password="!A79ca12018@" --database="rtalib" --execute="SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = '$dbname';")
echo $n

tables=$(mysql --user="rtalib_user" --password="!A79ca12018@" --database="rtalib" --execute="SELECT table_name FROM information_schema.tables where table_schema='$dbname';")

isFirst=1
for word in $tables;
do
  if [[ "$isFirst" -eq 0 ]];
    then
      echo $word
  fi
  isFirst=0
done

tn=$(sed 's/[^0-9]//g' <<< $n)


if [[ "$tn" -eq 0 ]];
  then
    printf 'No tables found!\n'
    return;
  else
    for i in `seq 1 $tn`;
      do
        echo $i
    done
fi
