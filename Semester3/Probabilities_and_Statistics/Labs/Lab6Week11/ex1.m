% H0 : statistical parameter =  given value <- null hypothesis -> this is what is already known
% H1 : statistical parameter (the same as above) <, > or different from our given value <- alternative hypothesis -> this is the new information that can be put to the test
% usually stat param. = mean of population, variance of population, difference of means of 2 populations, ratio of 2 population variances
x=[7,7,4,5,9,9,...
   4,12,8,1,8,7,...
   3,13,2,1,17,7,...
   12,5,6,2,1,13,...
   14,10,2,4,9,11,...
   3,5,12,6,10,7];

n = length(x);
alpha = input("Please give the significance level: "); % 0.05; significance_level = 1 - confidance_level (that's why it is should be so small)

% a)
% H0: m = 8.5 <- null hypothesis
% H1: m < 8.5 <- alternative hypothesis
% this is a left-tail(<) Z-test for the population mean when signma is known

sigma = 5;
m0 = 0.5; % threshold value for comparison - observed mean

[h, p, ci, zobs] = ztest(x, m0, sigma, "alpha", alpha, "tail", "left"); % we don't care about ci

% prepare the Rejection Region
z = norminv(alpha, 0, 1);
RR = [-inf, z];
if h == 1 % if h = 1 reject H0 and if h = 0 do not reject H0
	printf("The value of h is %d. We reject the null hypothesis, \n", h);
	printf("So, the data suggests that the standard is not met. \n");
else
	printf("The value of h is %d. We do not reject the null hypothesis, \n", h);
	printf("So, the data suggests that the standard is met. \n");
endif
	
printf("The rejection region is (%4.3f, %4.3f). \n", RR);
printf("The obesrved value of the test statistic is %4.3f. \n", zobs);
printf("The p-value of the test is %4.3f. \n", p);

% for subpoint b) use ttest for tobs = stats.tstat (structure, check help ttest)
% for exercise 2) a) you use vartest2 to decide if the variances are equal or not
% 2) b) the test here depends on a) variances equal => one version(2nd minus of point 3)  / variances not equal => another(3rd minus of point 3)

