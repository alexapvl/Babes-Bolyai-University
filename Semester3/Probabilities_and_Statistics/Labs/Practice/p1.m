clc;
clear;
steel = [4.6, 0.7, 4.2, 1.9, 4.8, 6.1, 4.7, 5.5, 5.4];
glass = [2.5, 1.3, 2.0, 1.8, 2.7, 3.2, 3.0, 3.5, 3.4];

alpha = 0.05; % significance level

%h0: variances are the same (=)
%h1: variances differ (!=)

[h, p, ci, stats] = vartest2(steel, glass, "alpha", alpha, "tail", "both");

printf("Point a).\n");
if h == 0
	printf("H0 is NOT rejected, the variances of the populations are the same.\n");
else
	printf("H0 is rejected, the variances are different.\n");
end

