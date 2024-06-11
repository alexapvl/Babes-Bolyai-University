#!/bin/bash

if [ $# -eq 0 ] ; then
	echo "Usage: ./solution.sh dir1 dir2 dir3 ..."
	exit 1
fi

while [ $# -gt 0 ] ; do
	for FILE in $(find $1 | grep -Eo "\./[a-zA-Z0-9]") ; do
		# get the name of the file which is at the end of the path
		nameOfFile=$(echo $FILE | grep -Eo "/[^/]*$" | sed -E 's@(/)(.*)@\2@')

		# write in the report.txt file(first part of problem statement)
		if [ -L $FILE ] ; then 
			# this will get the path to which the symbolic link points to
			destination=$(file $FILE | grep -Eo "\`.*" | sed -E 's@(.)(.*)(.$)@\2@')
			echo $FILE IS A LINK TO $destination >> report.txt
		fi

		# create folders and add copies for all files(second part of problem statement)
		if [ -f $FILE ] && [ ! -L $FILE ] ; then # if we have a file and it is not a symbolic link, we make a copy of it and put it in the specific directory
			extension=$(echo $FILE | grep -Eo "\.[^/]*$" | sed -E 's@(\.)(.*)@\2@')
			if [ "$extension" == "" ] ; then
				# if the extension is empty put it into a separate directory
				if [ ! -d no_extension ] ; then
					mkdir no_extension
				fi
				if [ -f no_extension/$nameOfFile ] ; then # if we already have a file with the same name, we concatenate the contents
					touch temp
					head -n -1 no_extension/$nameOfFile >> temp
					cat $FILE >> temp
					echo $(date "+<<-- Created on %d/%m/%Y %H:%M by Apavaloaiei Alexandru -->>") >> temp
					cat temp > no_extension/$nameOfFile # overwrite the already existing file with the full contents(including concatenations) 
					rm temp # remove the temporary file
				else
					cp $FILE no_extension/$nameOfFILE
					echo $(date "+<<-- Created on %d/%m/%Y %H:%M by Apavaloaiei Alexandru -->>") >> no_extension/$nameOfFile
				fi
			else # do the same thing but for files which have an extension
				if [ ! -d $extension ] ; then
					mkdir $extension
				fi
				if [ -f $extension/$nameOfFile ] ; then
					touch temp
					head -n -1 $extension/$nameOfFile >> temp
					cat $FILE >> temp
					echo $(date "+<<-- Created on %d/%m/%Y %H:%M by Apavaloaiei Alexandru -->>") >> temp
					cat temp > $extension/$nameOfFile
					rm temp
				else
					cp $FILE $extension/$nameOfFile
					echo $(date "+<<-- Created on %d/%m/%Y %H:%M by Apavaloaiei Alexandru -->>") >> $extension/$nameOfFile
				fi
			fi
		fi
	done
	shift
done
