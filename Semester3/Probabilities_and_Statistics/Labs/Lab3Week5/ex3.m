n = input("How many trials? (more than 30): ")
p = input("Prob of success for a trail (smaller than 0.05): ")

lambda = p*n;

k = 0:n
pb = binopdf(k,n,p)
pb1 = poisscdf(k, lambda);

plot(k, pb, '*');
hold on;
plot(k, pb1, '--');

