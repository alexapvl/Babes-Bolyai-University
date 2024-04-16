#!/bin/bash

# 5. Să se scrie un script bash care pentru fiecare argument din linia de comandă:
# - dacă e fișier, va afişa numele, numărul de caractere și de linii (în această ordine)
# - dacă e director, va afişa numele și câte fișiere conține (inclusiv în subdirectoare).
# (comenzi: find, grep, test, wc)

if [ $# -eq 0 ] ; then
	echo no arguments provided
	exit 1
fi

while [ $# -gt 0 ] ; do
	if [ -d $1 ] ; then
		numberfiles=`find $1 -type f | wc -l`
		echo directory $1 has $numberfiles files inside of it
	fi

	if [ -f $1 ] ; then
		numberchars=`cat $1 | wc -c`
		numberlines=`cat $1 | wc -l`
		echo $1 $numberchars $numberlines
	fi

	shift
done

