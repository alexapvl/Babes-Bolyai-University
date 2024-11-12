clear
p = input("Provide p: "); % probability of success
N = input("Provide N: "); % number of simulations

X = zeros(1, N); % at each index will be the number of failures at each simulation
for i = 1:N
	X(i) = 0;
	while rand >= p
		X(i) = X(i) + 1
	endwhile
endfor

K = 0:20;
pk = geopdf(k, p);
U_X = unique(X);
n_X = hist(X, length(U_X));
rel_freq = n_X/N;
clf;
plot(U_X, rel_freq, '*', K, pk, 'ro');
legend("simulation", "geometric progresion");

