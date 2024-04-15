#!/bin/bash

# par 1 - f.txt ; par 2 ... litere
# pentru fiecare litera, creeaza un fisier cu toate cuvintele din f.txt care incep cu litera respectiva

if [ ! -f $1 ] ; then
	echo $1 trebuie sa fie fisier
	exit 1
fi

fisier=$1
shift

for litera in $@; do
	if [[ $litera =~ ^[a-z]$ ]] ; then
		grep -E -i -o "\<${litera}[a-z]*\>" $fisier > "${litera}.txt"
	else 
		echo $litera nu este o litera mica
	fi
done

