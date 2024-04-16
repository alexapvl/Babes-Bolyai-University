#!/bin/bash 

# 8. Să se scrie un script bash care are ca argumente triplete formate dintr-un nume de fisier, un cuvant si un numar k.
# Pentru fiecare astfel de triplet, se vor afişa toate liniile fişierului care contin cuvăntul respectiv exact de k ori.
# (comenzi: shift, wc, sed, grep)

if [ ! `expr $# % 3` -eq 0 ] ; then
	echo invalid number of arguments, should be multipe of 3
	exit 1
fi

count=0
while [ $# -gt 0 ] ; do
	if [ -f $1 ] ; then # check for filename to be a valid file
		echo ${1}:
		if [[ $3 =~ ^[1-9][0-9]*$ ]] ; then # check for number provided to be a number
			while read line ; do
					for WORD in $line ; do
						if [ "$WORD" == "$2" ] ; then
							count=`expr $count + 1`
						fi
					done
					if [ $count -eq $3 ] ; then
						echo $line
					fi
					count=`expr $count - $count`
			done < $1
		else
			echo $3 not a number, skipping tripple 
		fi
	else
		echo $1 not a valid file, skipping tripple
	fi
	echo "###############################################"
	shift 3
done


