#!/bin/bash

# 4. Să se scrie un script bash care afișează numele fișierelor dintr-un director dat ca argument care conțin numere cu peste 5 cifre.
# (comenzi: find, grep, sed)

if [ ! $# -eq 1 ] ; then
	echo invalid number of argments
	exit 1
fi

dir=$1

for FILE in $(find . -maxdepth 1 -type f | sed 's@.*/@@') ; do
	if echo $FILE | grep -E -q "[1-9]{1}[0-9]{4,}" ; then
		echo $FILE
	fi
done

