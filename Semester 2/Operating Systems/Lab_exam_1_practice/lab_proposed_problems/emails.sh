#!/bin/bash

# Write a script shell that receives command line arguments as follows:
# The first argument is a file containg text.
# The second argument is an email address.
# The rest of the arguments are also email addresses.
# For each email address recived as an agrument, except the second argument
# the script will generate a file named:
# message-<email address here>
# Each file created will contain the following:

# From: <insert email received as the second argument here>
# To: <insert email address here>
# <insert the text from the file received as the first argument here>

# Delivered on <day>/<month>/<year>

# where day month year takne from the system current time.

# After creating the files the script will group all the files in directoies,
# each direcotory will be named after the domains of the email addresses given
# as arguments (the domain of an email address is the that part of the email
# that comes after @ ex: yahoo.com gmail.com scs.ubbcluj.ro)

# The following command can be used to solve this problem:
# while
# for
# mv
# date
# cut

if [ $# -lt 3 ] ; then
	echo not enough arguments
fi

if [ ! -f $1 ] ; then 
	echo $1 not a file
fi

# save the values because we will shift
file=$1 
fromAddress=$2 

while [ $# -ge 3 ] ; do
	# $3 will be the email address we will use		
 	echo "" > message-$3 # create a file for the current email address
	echo From: $fromAddress >> message-$3
	echo To: $3 >> message-$3
	cat $file >> message-$3
	echo Delivered on $(date "+%Y-%m-%d") >> message-$3
			
	# verify if the folder with the domain exists
	domain=`echo $3 | grep -Eo "@.*$" | sed -E 's/(@)(.*)/\2/'` # take only the domain from the email
	if [ ! -d $domain ] ; then
			mkdir $domain
	fi
	mv message-$3 $domain

	shift
done

