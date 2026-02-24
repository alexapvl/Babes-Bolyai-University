% clean() -> clear variables in the workspace
% clc() -> clear the command window

% p(x) = x^5-5x^4-16x^3+16x^2-17x+21
% a) given this polynomial, plot it

x = linspace(-4, 7.2, 100); % or we can use -4:0.1:7.2;
px = x.^5 - 5*x.^4 - 16*x.^3 + 16*x.^2 - 17*x + 21;
plot(x, px);
title('p(x) = x^5-5x^4-16x^3+16x^2-17x+21');
xlabel('x');
ylabel('p(x)');
grid on;

% b) find the value of p(-2.6) using polyval function
p=[1, -5, -16, 16, -17, 21];
polyval(p, -2.6) % evaulates the polynomial

% c) find the roots of the polynomial
roots(p) % finds the roots of the polynomial
