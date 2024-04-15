#!/bin/bash

# se citest stringuri pana la string-ul "stop", numaram cate incep cu o vocala

countzero=0

while true; do
	read cuvant
	if [ $cuvant == "stop" ]; then
		break
	fi
	if [[ $cuvant =~ \<[aeiouAEIOU][a-zA-Z]*\> ]]; then
		countzero=`expr $countzero + 1`
	fi
done

echo "There are $countzero words which start with a vowel"
