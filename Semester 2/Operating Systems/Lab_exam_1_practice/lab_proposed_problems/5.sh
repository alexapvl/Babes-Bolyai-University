#!/bin/bash

echo ">>> $#"

#xxa=101

#echo "$0 == $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12}"

# $*
  
# $@

while [ "$1" ]
do

  echo $*
  
  echo $@

  echo $#
   
  shift 3

done

exit

for c in $(echo $*)
do

  echo $c

done

exit

for b in $@
do

  echo $b

done



for (( i = 1; i <= $#; i++))
do

   echo "${i} ==> ${!i}"

done

exit

for a
do

  echo $a

done
