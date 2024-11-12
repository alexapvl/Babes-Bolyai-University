% ask user for probability p = 0.4
p = input("Provide p: ");
% ask user for number of trails n = 20
n = input("Provide n: ")
% ask user for number of simulations N = 100
N = input("Provide N: ");

% simulate a matrix, one column corresponds to one simulation
U = rand(n, N); % n -> number of rows; N -> number of columns
X = sum(U < p);

K = 0:n;
pk = binopdf(K, n, p);
U_X = unique(X);
n_X  = hist(X, length(U_X));
rel_freq = n_X/N;
clf; % clear figure
plot(U_X, rel_freq, '*', K, pk, 'ro');
legend('sim', 'bino');
