% A study is conducted to compare head loss glass pipes, versus steel pipes of 
% the same size. Various liquids at identical starting temperatures are run 
% through segments of each type and the heat loss (in degrees celsius) is measured.
% Steel: [4.6, 0.7, 4.2, 1.9, 4.8, 6.1, 4.7, 5.5, 5.4]
% Glass: [2.5, 1.3, 2.0, 1.8, 2.7, 3.2, 3.0, 3.5, 3.4]

% Clear workspace and command window
clc;
clear all;

% This script performs statistical analysis comparing heat loss between steel and glass pipes
% Set significance level for hypothesis testing
significance = 0.05;

% Define data arrays for steel and glass pipe measurements
steel = [4.6 0.7 4.2 1.9 4.8 6.1 4.7 5.5 5.4];
glass = [2.5 1.3 2.0 1.8 2.7 3.2 3.0 3.5 3.4];

% Part A: Test for equality of variances using F-test
% H0: σ²₁ = σ²₂ (variances are equal)
% H1: σ²₁ ≠ σ²₂ (variances are different)
[h, p, ci, stats] = vartest2(steel, glass, "alpha", significance);
% h: hypothesis test result (1 = reject H0, 0 = fail to reject H0)
% p: p-value of the test
% ci: confidence interval for the ratio of variances
% stats: structure containing test statistics (F-statistic, degrees of freedom)

fprintf("part. A\n")

% Display results of variance test
if h == 0
    fprintf("H0 is not rejected. The variances do not differ.\n")
else
    fprintf("H0 is rejected. The variances differ\n")
end

% Calculate critical values for F-distribution
% q2 is upper critical value, q1 is lower critical value
q2 = finv(1-significance/2, stats.df1, stats.df2);
q1 = finv(significance/2, stats.df2, stats.df1);

% Display p-value and rejection region for F-test
fprintf('P-value is %1.4f\n', p);
fprintf("the rejection region is (-inf, %f) U (%f, +inf)", q1, q2);

% Part B: Test for difference in means using t-test
% H0: μ₁ ≤ μ₂ (steel pipes don't lose more heat than glass)
% H1: μ₁ > μ₂ (steel pipes lose more heat than glass)
fprintf("\npart. B\n");

% Perform one-tailed t-test assuming equal variances (based on Part A)
[h, p, ci, stats] = ttest2(steel, glass, "alpha", significance, "tail", "right");

% Display results of t-test
if h == 0
      fprintf('H0 is not rejected. Steel pipes do NOT lose more heat than glass.\n')
else
      fprintf('H0 is rejected. Steel pipes DO lose more heat than glass pipes.\n')
end

% Display p-value and calculate rejection region
fprintf('P-value of the test statistic is %e.\n', p);

% Calculate critical value for t-distribution
q1 = tinv(1 - significance, stats.df);

% Display rejection region
fprintf('Rejection region R is (%3.4f, +inf)\n', q1);
fprintf('\n\n\n\n');