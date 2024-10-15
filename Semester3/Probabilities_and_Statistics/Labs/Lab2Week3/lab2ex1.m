n = input("Give number of trials n="); % n - positive intiger
p = input("Give probability of success p="); % p - belongs to [0,1]
x = 0:n;
px = binopdf(x,n,p);
clf; % clear figure

plot(x, px, '*');
hold on;

xx = 0:0.01:n; % simulate continuity
cx = binocdf(xx, n, p);
plot(xx, cx, 'b');
legend('pdf', 'cdf');

