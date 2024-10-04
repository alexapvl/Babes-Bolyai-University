% compute the factorial of a number N

fac(0, 1).
fac(N, R):-
	N > 0,
	N1 is N - 1,
	fac(N1, TR),
	R is N * TR.

% compute the sum of the digits of a number N

sumd(0, 0).
sumd(N, R):-
	D is N mod 10,
	N1 is N div 10,
	sumd(N1, TR),
	R is D + TR.