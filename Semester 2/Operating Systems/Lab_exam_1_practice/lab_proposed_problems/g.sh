#!/bin/bash

# 7. Să se scrie un script bash care primește ca argumente oricâte nume de fișiere. 
# Scriptul va citi de la tastatură două cuvinte și va înlocui TOATE aparițiile primului cuvânt cu cel de-al doilea în fiecare fișier furnizat ca argument.
# (comenzi: read, sed)

if [ $# -eq 0 ] ; then
	echo invalid number of arguments
	exit 1
fi

echo first word:
read word1
echo secont word:
read word2

while [ $# -gt 0 ] ; do
	if [ -f $1 ] ; then
		#sed -i 's/'$word1'/'$word2'/g' $1
		cat $1 | sed 's/'$word1'/'$word2'/g'
	else
		echo $1 not a file
	fi
	shift
done

