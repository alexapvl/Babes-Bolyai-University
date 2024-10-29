printf("---Normal---\n");
printf("a)\n");

mu = input("Provide MU (real number): ");
sigma = input("Provide sigma (positive): ");

printf("P(X<=0) = %1.6f\n", normcdf(0, mu, sigma));
printf("P(X>=0) = %1.6f\n", 1 - normcdf(0, mu, sigma));

printf("b)\n");
resultB = normcdf(1, mu, sigma) - normcdf(-1, mu, sigma)
printf("P(-1<=X<=1) = %1.6f\n", resultB);
printf("P(X<=-1 or X>=1) = 1 - P(-1<X<1) = 1 - aboveRes = %1.6f\n", 1 - resultB);

printf("c)\n");
alpha = input("Provide alpha from (0,1): ");
printf("X such that P(X<%d) = P(X<=%d) is %1.6f\n", alpha, alpha, norminv(alpha, mu, sigma));
printf("d)\n");
beta = input("Provide beta from (0,1): ");
printf("X such that P(X>%d) = P(X>=%d) is %d\n", beta, beta, norminv(1 - beta, mu, sigma));

clear(); % clear variables
printf("---Student---\n");

printf("a)\n");

df = input("Provide degree of freedom: ");

printf("P(X<=0) = %1.6f\n", tcdf(0, df));
printf("P(X>=0) = %1.6f\n", 1 - tcdf(0, df));

printf("b)\n");
resultB = tcdf(1, df) - tcdf(-1, df)
printf("P(-1<=X<=1) = %1.6f\n", resultB);
printf("P(X<=-1 or X>=1) = 1 - P(-1<X<1) = 1 - aboveRes = %1.6f\n", 1 - resultB);

printf("c)\n");
alpha = input("Provide alpha from (0,1): ");
printf("X such that P(X<%d) = P(X<=%d) is %1.6f\n", alpha, alpha, tinv(alpha, df));
printf("d)\n");
beta = input("Provide beta from (0,1): ");
printf("X such that P(X>%d) = P(X>=%d) is %1.6f\n", beta, beta, tinv(1 - beta, df));

clear();
printf("---Chi-Squared---\n");

printf("a)\n");

df = input("Provide degree of freedom: ");

printf("P(X<=0) = %1.6f\n", chi2cdf(0, df));
printf("P(X>=0) = %1.6f\n", 1 - chi2cdf(0, df));

printf("b)\n");
resultB = chi2cdf(1, df) - chi2cdf(-1, df)
printf("P(-1<=X<=1) = %1.6f\n", resultB);
printf("P(X<=-1 or X>=1) = 1 - P(-1<X<1) = 1 - aboveRes = %1.6f\n", 1 - resultB);

printf("c)\n");
alpha = input("Provide alpha from (0,1): ");
printf("X such that P(X<%d) = P(X<=%d) is %1.6f\n", alpha, alpha, chi2inv(alpha, df));
printf("d)\n");
beta = input("Provide beta from (0,1): ");
printf("X such that P(X>%d) = P(X>=%d) is %1.6f\n", beta, beta, chi2inv(1 - beta, df));

printf("---Fisher---\n");


printf("a)\n");

df1 = input("Provide degree of freedom for numerator: ");
df2 = input("Provide degree of freedom for denominator: ");

printf("P(X<=0) = %1.6f\n", fcdf(0, df1, df2));
printf("P(X>=0) = %1.6f\n", 1 - fcdf(0, df1, df2));

printf("b)\n");
resultB = fcdf(1, df1, df2) - fcdf(-1, df1, df2)
printf("P(-1<=X<=1) = %1.6f\n", resultB);
printf("P(X<=-1 or X>=1) = 1 - P(-1<X<1) = 1 - aboveRes = %1.6f\n", 1 - resultB);

printf("c)\n");
alpha = input("Provide alpha from (0,1): ");
printf("X such that P(X<%d) = P(X<=%d) is %1.6f\n", alpha, alpha, finv(alpha, df1, df2));
printf("d)\n");
beta = input("Provide beta from (0,1): ");
printf("X such that P(X>%d) = P(X>=%d) is %1.6f\n", beta, beta, finv(1 - beta, df1, df2));
