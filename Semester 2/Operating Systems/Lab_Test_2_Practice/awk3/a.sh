BEGIN {
	sum_odd_nr_fields = 0;
	nr = 0;
}

{
	if(NF % 2 == 0) 
		print "Sum of the first and last number is: ", $1 + $NF
	else {
		sum_odd_nr_fields += $(int(NF/2)+1)
		nr++
	}
}

END {
	print "Mean odd number of numbers: ", sum_odd_nr_fields/nr
}
