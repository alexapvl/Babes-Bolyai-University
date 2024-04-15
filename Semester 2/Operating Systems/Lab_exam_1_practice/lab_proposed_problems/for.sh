#!/bin/bash

#for each line in commnad output !!not working yet

while read line
do

  echo "$line"

done < <(ls -la ~)

exit

#for each line in file

while read line
do

  echo "$line"

done < $1

exit

#for each word in file

for w in $(cat $1)
do

echo "$w"

done

#for each word in command output

for w in $("<insert command here>")
do

echo "$w"

done



