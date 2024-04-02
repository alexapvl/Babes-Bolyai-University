BEGIN {
	number_of_words_on_even_lines = 0
	mean_number_of_words_on_odd_lines = 0
	number_of_odd_lines = 0
}

{
	if(NR % 2 == 0) {number_of_words_on_even_lines+=NF;}
	else {
	 mean_number_of_words_on_odd_lines+=NF;
	 number_of_odd_lines++;
	}
}

END{
	print "Number of words on even lines: ", number_of_words_on_even_lines
	print "Mean number of words on odd lines: ", mean_number_of_words_on_odd_lines/number_of_odd_lines
}
