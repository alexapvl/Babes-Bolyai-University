#!/bin/bash

# parameter 1 - f.txt ; parameter 2, 3, ... letters 
# for each letter provided, create a file with all words from f.txt which begin with that specific letter

if [ ! -f $1 ] ; then
	echo $1 needs to be a file 
	exit 1
fi

file=$1
shift

for letter in $@; do
	if [[ $letter =~ ^[a-z]$ ]] ; then
		grep -E -i -o "\<${letter}[a-z]*\>" $file > "${letter}.txt"
	else 
		echo $letter is not a lower case letter 
	fi
done

