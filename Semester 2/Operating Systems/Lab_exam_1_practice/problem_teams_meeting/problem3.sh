#!/bin/bash

# delete every third occurence of a given word from a file

if [ ! -f $1 ]; then
	echo "$1 is not a file"
fi

count=0

while read line; do # line will be a variable

	for word in $line; do
		if [ "$word" == "$2" ]; then
			count=`expr $count + 1`
			if [ `expr $count % 3` -ne 0 ]; then
				echo $word
			fi
			else 
				echo $word
		fi
	done

done < $1

# echo "The word $2 appears $count times in the file $1"

