% a) X ~ BinomialModel(3, 0.5)
% (  0   1   2   3  )
% ( 1/8 3/8 3/8 1/8 )

% b) see prev ex. - just plot it

% c) P(x=0) - def of PDF in x=0
printf("c)\n");
P1 = binopdf(0,3,0.5);
% P(x!=1) = 1 - P(X=1)
P2 = 1 - binopdf(1,3,0.5);
printf("P(X=0) = %1.6f\n", P1);
printf("P(X!=1) = %1.6f\n", P2);

% d) P(X<=2) - def of CDF in x = 2
P3 = binocdf(2,3,0.5);

% P(X<2) = P(X<=1)
P4 = binocdf(1,3,0.5);

printf("d)\n");
printf("P(X<=2) = %1.6f\n", P3);
printf("P(X<2) = P(X<=1) = %1.6f\n", P4);

% e) P(X>=1) = 1 - P(X<1) = 1 - P(X<=0)
P5 = 1 - binopdf(0,3,0.5);
% P(X>1) = 1 - P(X<=1)
P6 = 1 - binocdf(1, 3, 0.5);

printf("e)\n");
printf("P(X>=1) = 1 - P(X<1) = 1 - P(X<=0) = %1.6f\n", P5);
printf("P(X>1) = 1 - P(X<=1) = %1.6f\n", P6);

