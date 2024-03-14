#!/bin/bash

for A in $@; do
	if echo $a | grep -E -q "^[0-9]*[02468]$" && \
				( test -f $A || test -d $A); then
				echo Behold the amazind even file name $A
	elif test -f $A; then
		echo $A is a file
	elif test -d $A; then
		echo $A is a directory
	elif echo $A | grep -E -q "^[0-9]+$"; then
		echo $A is a number
	else
		echo "$A what?"
	fi
done
