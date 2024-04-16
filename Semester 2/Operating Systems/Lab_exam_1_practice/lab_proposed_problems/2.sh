#!/bin/bash


#0 - true
#!0 - false

# retrun 0;

#x=" "

if [ "$x" ]
then

  echo "var x does exist"

else

  echo "var x does not exist"

fi

exit


a=6

b=7

while [ "$b" -ne "0" ]
do

  a=$((a + 2))
  echo $a

  b=$((b - 1))

done

echo "===> $a"

exit

if grep -q "$1" /etc/passwd
then

  echo "match"

else

  echo "!match"

fi

exit


if [ "$1" -eq "$2"  ]
then

  echo "$1 == $2"

else

  echo "$1 != $2"

fi

exit


if [[ "$1" ~= ^[0-9].+[A-Z] ]]
then

  echo "match"

else

  echo "no match"

fi

exit

if [ -f "$1" ] 
then
 
  echo "$1 exists" 

else

  echo "$1 does not exist"

fi
exit


if [ -x "$1" ] 
then
 
  echo "$1 is a executabel" 

else

  echo "$1 is not a executable"

fi
exit

if [ -d "$1" ] 
then
 
  echo "$1 is a direcgtory" 

else

  echo "$1 is not a directory"

fi



