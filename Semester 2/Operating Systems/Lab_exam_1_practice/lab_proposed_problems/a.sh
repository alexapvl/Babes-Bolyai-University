#!/bin/bash

# 1. Să se scrie un script bash care primeşte ca argument un număr natural N şi generează N fişiere de tip text, astfel:
#- numele fişierelor vor fi de forma: file_X.txt, unde X = {1, 2, ..., N}
#- fiecare fişier generat va conţine doar liniile de la X la X + 5 ale fişierului /etc/passwd.
#(comenzi: touch, sed)

echo "" > report.txt

while [ $# -gt 0 ] ; do
	if [ ! -d $1 ] ; then
		echo "$1 not a directory, skipping this argument"
	else
		echo $1 `find $1 -type f -perm +111 | grep -Ec ".*"` >> report.txt		
	fi
	shift
done

