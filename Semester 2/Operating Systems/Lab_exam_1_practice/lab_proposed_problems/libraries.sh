# Write a script shell that receives command line arguments as follows:
# The first argument is a file containg c libraries, one library on each line.
# The rest of the arguments are directories.
# The script will serach in the hierarchy of each directory and identify all c
# source code files. Then the script will make sure that all c source files
# found in the directories are using all the libraries noted in the file given
# as a first argument, if a source file already uses all the libraries the
# script will ignore the file, if not the script will insert the include line
# for all the missing libraries at the beginning of the c source file and
# insert a note at the end of the file, the note should look as follows:
# // File modified on <day>/<month>/<year>-<hour>:<minute> by <insert your name here>
# where day month year hour minute are takne from the system current time.
# No input data validation is necessary.
# If you want to use temporary files to solve the problem make sure they do
# not alter the already existing files on the server.

libraries=$1

while [ $# -gt 1 ] ; do # while we still have directories
	# go through each file in the current directory ($2)
	for currentFile in `find $2 -type f | grep -E "\.c$"` ; do
		# if we do not find the line, we put it into a temporary file
		touch temp.txt # create the temporary file
		modification=0
		for LINE in `cat $libraries` ; do
				ok=0 # 0 = false, we did not find the library
				for currentLine in `cat $currentFile` ; do
						# at this point we want to compare the lines
						if [ "$LINE" == "$currentLine" ] ; then
							ok=1 # we found the library in the current file
						fi
				done
				if [ $ok -eq 0 ] ; then
					echo "#include $LINE" >> temp.txt # if we don't find the library in the current file, we put it into a temporary file	
					modification=1
				fi
		done
		if [ $modification -eq 1 ] ; then
			echo `cat $currentFile` >> temp.txt # in order to put the inserted lines before the already existing information in the file
			echo File modified on `date "+%d/%m/%Y-%H:%M"` >> temp.txt
			cat temp.txt > $currentFile
			rm temp.txt
		fi
	done
	shift
done

if [ -f temp.txt ] ; do
	rm temp.txt
fi

