p = input("P (between 0.05 and 0.95) = ");
for n=1:3:100
	k=0:n;
	pb = binopdf(k,n,p);
	plot(k, pb,'*');
	pause(0.3)
endfor
