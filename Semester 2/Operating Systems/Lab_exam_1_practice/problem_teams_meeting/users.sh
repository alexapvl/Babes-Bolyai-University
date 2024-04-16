#!/bin/bash

# Script ./users.sh user1 n1 user2 n2 user3 n3 ...
# Output a message if user1 has connected n1 or more time in the system in the Frydays of April

if [ `expr $# % 2` -eq 1 ]; then
	echo "We need an even number of agruments"
	exit 1
fi

echo ""> sol.txt

while [ $# -gt 0 ]; do
		if grep -E -q "^$1:" /etc/passwd ; then
			if echo $2 | grep -E -q "^[1-9][0-9]*$" ; then
				nr=`last | grep -E -q "Fri[ ]* Apr" | grep -E -c "^$1"`
				if [ $nr -ge $2 ] ; then
					echo $1 has logged in a minimum of $2 in the system on Frydays in April >> sol.txt
				else 
					echo $1 did not log in enough times >> sol.txt
				fi
			else
				echo $2 not a number, pair of arguments ignored >> sol.txt
			fi
		else
			echo $1 not a valid user in the system, pair of arguments ignored >> sol.txt
		fi
	shift 2 # shift until no more pairs available
done

cat sol.txt | sort # display in sorted order the solution from sol.txt
rm sol.txt # remove the file after we use it

