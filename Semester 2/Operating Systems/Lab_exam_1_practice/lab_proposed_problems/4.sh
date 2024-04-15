#!/bin/bash





if [ "$1" -gt "$2"  ]
then
  
  echo "$1 > $2"
 
else

  echo "$1 < $2"

fi

exit

if [ ! "$1" != "$2" ] && [ "$2" = "$3" ] 
then

  echo "$1 == $2 == $3"

else

 echo "$1 != $2 | $2 != $3"

fi

exit


if test "$1" = "$2" 
then

  echo "$1 == $2"

else

 echo "$1 != $2"

fi

exit

if grep -q "$1" /etc/passwd
then

  echo "match"

else

  echo "not match"

fi 