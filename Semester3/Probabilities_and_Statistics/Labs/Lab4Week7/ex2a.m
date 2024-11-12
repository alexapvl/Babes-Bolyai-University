% ask user for prob. p -> ex p = 0.3
% ask user for the number of simulations N -> ex N = 100
% this exercise just SIMULATES a bernoulli variable, not actually generates it
p = input("Provide p: ");
N = input("Provide N: ");

U = rand(1, N); % this generates a random number for each simulation
X = (U < p);

%  count how many zeros and ones we have in X
U_X = [0, 1];
n_X = hist(X, length(U_X)); % histogram(barchart)

rel_freq = n_X/N;
