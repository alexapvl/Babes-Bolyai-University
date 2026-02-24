% plot a function of 2 variables

% f:[-2,2]x[0.5,4.5] -> R
% f(x,y) = sin(e^x)*cos(ln(y))

x = linspace(-2, 2, 100);
y = linspace(0.5, 4.5, 100);
[X, Y] = meshgrid(x, y); % meshgrid will find the intersection of points of x and y, and create a grid of points for which we can evaluate the function
Z = sin(exp(X)).*cos(log(Y));
surf(X, Y, Z);
title('f(x,y) = sin(e^x)*cos(ln(y))');
xlabel('x');
ylabel('y');
zlabel('f(x,y)');
grid on;