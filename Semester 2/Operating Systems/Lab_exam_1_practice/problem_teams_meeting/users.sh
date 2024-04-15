#!/bin/bash

# Script ./users.sh user1 n1 user2 n2 user3 n3 ...
# Afisam un mesaj daca user1 s-a conectat de n1 ori in sistem in zilele de vineri din aprilie

if [ `expr $# % 2` -eq 1 ]; then
	echo "Ne trebuie numar par de argumente"
	exit 1
fi

echo ""> sol.txt

while [ $# -gt 0 ]; do
		if grep -E -q "^$1:" /etc/passwd ; then
			if echo $2 | grep -E -q "^[1-9][0-9]*$" ; then
				nr=`last | grep -E -q "Fri[ ]* Apr" | grep -E -c "^$1"`
				if [ $nr -ge $2 ] ; then
					echo $1 s-a logat de minim $2 ori in sistem vinerea in aprilie >> sol.txt
				else 
					echo $1 s-a logat de prea putine ori >> sol.txt
				fi
			else
				echo $2 nu este numar, se ignora perechea de argumente >> sol.txt
			fi
		else
			echo $1 nu este user valid in sistem, se ignora >> sol.txt
		fi
	shift 2
done

cat sol.txt | sort
rm sol.txt
