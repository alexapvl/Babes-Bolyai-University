#!/bin/bash

# the script reads until the string "stop" is provided, we count how many words begin with a vowel

count=0

while true; do
	read word
	if [ $word == "stop" ]; then
		break
	fi
	if [[ $word =~ \<[aeiouAEIOU].*\> ]] ; then
		count=`expr $count + 1`
	fi
done

echo "There are $count words which start with a vowel"
