#!/bin/bash

#6. Să se scrie un script bash care primește ca argumente oricâte nume de fișiere. 
#Scriptul va citi de la tastatură un cuvânt, va determina și afișa numărul de apariţii ale cuvântului în fiecare fișier furnizat ca argument.
#(comenzi: read, grep)

if [ $# -eq 0 ] ; then
	echo invalid number of arguments given
	exit 1
fi

echo "Word: "
read word

while [ $# -gt 0 ] ; do
	if [ -f $1 ] ; then
		count=`grep -o "$word" $1 | wc -l`
		echo ${1}, $count appearances
	fi

	shift
done

