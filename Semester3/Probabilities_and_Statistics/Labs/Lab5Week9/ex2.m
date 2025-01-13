x=[7,7,4,5,9,9,...
   4,12,8,1,8,7,...
   3,13,2,1,17,7,...
   12,5,6,2,1,13,...
   14,10,2,4,9,11,...
   3,5,12,6,10,7]; % x is always a VECTOR, not a MATRIX
xbar=mean(x);
n=length(x);
oneMinusAlpha = input("Confidence level: "); % confidence level: 0.95, 0.98, 0.99, 0.995
alpha = 1 - oneMinusAlpha; % significance level
% a)
sigma=5;
m1 = xbar - sigma/sqrt(n) * norminv(1-alpha/2, 0, 1);  % N(0,1)
m2 = xbar - sigma/sqrt(n) * norminv(alpha/2, 0, 1);

printf("The confidence interval for the population mean when sigma is known is (%4.3f, %4.3f).\n", m1, m2);
% b)
s=std(x); % standard deviation of x -> use it when the population standard deviation is NOT known
m1b = xbar - s/sqrt(n)*tinv(1 - alpha/2, n-1);
m2b = xbar - s/sqrt(n)*tinv(alpha/2, n-1);
printf("The confidence interval for the population mean when sigma is unknown is (%4.3f, %4.3f).\n", m1b, m2b);


% c)
v = var(x); % variance of x -> THIS IS s^2 (s from line 18), so 'v' can also be computed as v = s^2
v1 = (n-1)*v/chi2inv(1-alpha/2, n-1);
v2 = (n-1)*v/chi2inv(alpha/2, n-1);
printf("The confidence interval for the variance is (%4.3f, %4.3f).\n", v1, v2);
s1 = sqrt(v1);
s2 = sqrt(v2);
printf("The confidence interval for standard deviation is (%4.3f, %4.3f).\n", s1, s2);

% problem 2 - homework
clear; % clears all variables

premium = [22.4, 21.7, 24.5, 23.4, 21.6, 23.3, 22.4, 21.6, 24.8, 20.0];
regular = [17.7, 14.8, 19.6, 19.6, 12.1, 14.8, 15.4, 12.6, 14.0, 12.2];

lenPremium = length(premium);
lenRegular = length(regular);

meanPremium = mean(premium);
meanRegular = mean(regular);

oneMinusAlpha = input("Confidence level for problem 2: "); % confidence level: 0.95, 0.98, 0.99, 0.995
alpha= 1 - oneMinusAlpha; % significance level

quantile = tinv(1-alpha/2, lenPremium + lenRegular - 2);

sp = sqrt(((lenPremium - 1) * meanPremium^2 +(lenRegular - 1) * meanRegular^2) / (lenPremium + lenRegular - 2));

r1 = meanPremium - meanRegular - quantile * sp * sqrt(1/lenPremium + 1/lenRegular);
r2 = meanPremium - meanRegular + quantile * sp * sqrt(1/lenPremium + 1/lenRegular);

printf("a) The confidence interval for the difference of the true means is: (%4.3f, %4.3f).\n", r1, r2);

c = (meanPremium^2 / lenPremium) / ((meanPremium^2 / lenPremium) + (meanRegular^2 / lenRegular));
n = (lenPremium - 1) * (lenRegular - 1) / (c^2 * (lenRegular - 1) + (1 - c)^2 * (lenPremium - 1));
quantileB = tinv(1-alpha/2, n); 

r1b = meanPremium - meanRegular - quantileB * sqrt(meanPremium^2/lenPremium + meanRegular^2/lenRegular);
r2b = meanPremium - meanRegular + quantileB * sqrt(meanPremium^2/lenPremium + meanRegular^2/lenRegular);

printf("b) The confidence interval for the differece of the true means when sigma1 != sigma2 is: (%4.3f, %4.3f).\n", r1b, r2b);

f1c = finv(1-alpha/2, lenPremium - 1, lenRegular - 1);
f2c = finv(alpha/2, lenPremium - 1, lenRegular - 1);

r1c = (1/f1c)*(meanPremium^2 / meanRegular^2);
r2c = (1/f2c)*(meanPremium^2 / meanRegular^2);

printf("c) The confidence interval for the ratio of the variances is: (%4.3f, %4.3f).\n", r1c, r2c);


