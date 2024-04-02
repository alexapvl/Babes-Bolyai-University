BEGIN {
	no_words = 0
	no_lines = 0
}

{
	for(i = 1; i <= NF; i++)
		v[$i]++
	no_words += NF
	no_lines++
}

END {
	print "Average number of words per line is: ", no_words / no_lines
	print "Words which have the frequency lower than the average are:"
	for(word in v)
		if(v[word] < no_words / no_lines)
			printf "%s, frevcenta %d\n", word, v[word]
}
