clc;
clear;
standard = [46, 37, 39, 48, 47, 44, 35, 31, 44, 37]; % first population
new = [35, 33, 31, 35, 34, 30, 27, 32, 31, 31]; % second population

alpha = 0.05; % significance level

% a) at a 5% significance level, do the population variences seem to difer?
% variance => we need to use vartest2 and the result will be in 'h'
% h=0 => fail to reject the null hypothesis and h=1 => reject the null hypothesis

% h0: the population variances are the same (==)
% h1: the population variances are different (!=)
[ha, pa, cia, statsa] = vartest2(standard, new, "alpha", alpha, "tail", "both");

printf("Point a)\n");
if ha == 0
	printf("H0 is NOT rejected, the variances of the populations are the same.\n");
else
	printf("H0 is rejected, the variances of the populations differ.\n");
end

% b) Find a 95% confidence interval for the difference iof the average assembling times.
% confidence_level = 0.95 => significance_level = 1 - confidence_level = 0.05
% the significance level is the same as in subpoint a)
% average => we need to use ttest2 and the result will be in 'ci'(stands for confidence interval)

printf("\nPoint b)\n");
[hb, pb, cib, statsb] = ttest2(standard, new, "alpha", alpha, "tail", "both");
printf("The confidence interval is: (%4.3f, %4.3f).\n", cib(1), cib(2));

