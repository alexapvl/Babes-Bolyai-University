#!/bin/bash

#3. Să se scrie un script bash care calculează numărul mediu de linii ale fișierelor de tip text dintr-un director dat ca argument.
#(comenzi: find, file, grep, wc)

filecount=0
linecount=0

if [ ! $# -eq 1 ] ; then 
	echo invalid arguments
	exit 1
fi

for FILE in $(find . -type f | sed 's@.*/@@') ; do
	if file $FILE | grep -Eq "ASCII text$" ; then
		filecount=`expr $filecount + 1`
		currentlines=`cat $FILE | wc -l` 
		linecount=`expr $currentlines + 1`
	fi
done

expr $linecount / $filecount

