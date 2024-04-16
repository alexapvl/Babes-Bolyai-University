#!/bin/bash

# $# <=> argc
# $*
# $@

echo $*

echo $@

echo "argc=$#"

#for (( i=1; i <= $#; i++ ))
#do

#   echo "${i} => ${!i}"

#done

#while [ "$1" ]
#do

#   echo $*
  
#   shift 3
   
#done

for b in "$@"
do 

  echo ">>>$b"

done

#for a
#do

#  echo $a

#done




