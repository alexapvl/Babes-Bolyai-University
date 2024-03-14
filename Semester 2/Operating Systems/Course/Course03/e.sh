#!/bin/bash

D=$1
S=$2

if ! test -d $D; then
	echo $D is not a directory
	exit 1
fi

for F in 'find $D -type f 2> /dev/null'; do
	N=`ls -l $F | awk '{print $5}'`
	if test $N -gt $S; then
		echo $F
	fi
done

#find $D -type f 2> /dev/null | while read F; do
#	N=`ls -l $F | awk '{print $5}'`
#	if test $N -gt $S; then
#		echo $F
#	fi
#done | sort | grep -E -v "a"

