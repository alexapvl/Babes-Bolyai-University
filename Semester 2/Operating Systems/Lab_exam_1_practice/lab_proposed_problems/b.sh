#!/bin/bash

#2. Să se scrie un script bash care afișează primele 3 linii și ultimele 3 linii ale fiecărui fișier de tip text 
#din dintr-un director dat ca argument în linia de comandă. Dacă un fișier text are mai puțin de 6 linii, atunci 
#conţinutul acestuia va fi afişat în întregime.
#(comenzi: find, file, grep, wc, head, tail)

if [ ! $# -eq 1 ] ; then
	echo number of args not ok
	exit 1
fi

dir=$1

for FILE in $(find . -type f | sort | sed 's&.*/&&'); do
	if file $FILE | grep -E -q "ASCII text$" ; then
		# here we know for sure that the file is a text file
		echo ${FILE}:
		numberlines=$(cat $FILE | wc -l)
		if [ $numberlines -lt 6 ] ; then
			cat $FILE
		else 
			echo --head--
			cat $FILE | head --lines 3
			echo --tail--
			cat $FILE | tail --lines 3
		fi
		echo "###############################################"
	fi
done

