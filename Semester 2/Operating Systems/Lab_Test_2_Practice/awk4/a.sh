BEGIN {
	max=-1
	min=100000000
	lin_max=""
	lin_min=""
	no_words=0
	no_lines=0
}

{
	if(NF > max) {
		max = NF
		lin_max = $0
	}
	if(NF < min) {
		min = NF
		lin_min = $0
	}
	no_words += NF
	no_lines++
}

END {
	print lin_max
	print lin_min
	print no_words/no_lines
}
