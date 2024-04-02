BEGIN {
	sum_of_odd_numbers = 0
	sum_of_numbers_on_even_lines = 0
	nr = 0
}

{
	if(NR % 2 == 0) {
		for(i = 1; i <= NF; i++) {sum_of_numbers_on_even_lines += $i}
	}
	for(i = 1; i <= NF; i++) {
		if($i % 2 == 1) {
			sum_of_odd_numbers += $i
			nr++
		}
	}
}

END {
	print "The sum of the numbers on even lines is: ", sum_of_numbers_on_even_lines
	print "The mean sun of the odd numbers is: ", sum_of_odd_numbers/nr
}
