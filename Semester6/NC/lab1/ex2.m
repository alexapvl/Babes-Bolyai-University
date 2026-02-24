% plot f,f,g:[2, 2pi] -> R where
% f(x) = sin(x)
% g(x) = sin(2x)
% h(x) = sin(3x)
% plot all of them, each in a different subplot

x = linspace(0, 2*pi, 100);
f = sin(x);
g = sin(2*x);
h = sin(3*x);
% subplot(x, y, z)
% x: total number of subplots
% y: total number of columns (the rows are computed based on that automatically)
% z: the index of the subplot (the index starts from 1 and goes from left to right on the rows, top to bottom on the columns)
subplot(3, 1, 1); % 3 rows, 1 column, 1st subplot
plot(x, f);
title('f(x) = sin(x)');
subplot(3, 1, 2); % 3 rows, 1 column, 2nd subplot
plot(x, g);
title('g(x) = sin(2x)');
subplot(3, 1, 3); % 3 rows, 1 column, 3rd subplot
plot(x, h);
title('h(x) = sin(3x)');
