#! /bin/bash

for A in a b c d e f g; do
	echo $A
done

for B in $@; do
	echo $B
done

for C in *; do
	echo $C
done
