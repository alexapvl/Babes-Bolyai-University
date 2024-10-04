% Multiply each element of a list with a constant value.
% Ex: [1, 2, 3] k = 2 => [2, 4, 6]
% mulk(L - input list, K - number, R - result list)
% flow model (input, input, output)

mulk([], _, []).
mulk([H|T], K, R):-
    mulk(T, K, TR),
    NH is H*K,
    R=[NH|TR].


% Add an element at the end of a list
% e=11, L=[1,2,3] => L=[1,2,3,11]

addE([], E, [E]).
addE([H|T], E, [H|TR]):-
    addE(T, E, TR).

% Compute the sum of the elements of a list
% sum(L-list, S - number)
% flow model (i,o) (i,i)

% (i, i) -> checks for true or false condition, compares result with the one provided
% (i, o) -> returns the result in the output parameter

sum([], 0).
sum([H|T], S):-
    sum(T, TS),
    S is H+TS.

% sum([1,2,3], R) => R = 6
% sum([1,2,3], 5) => false

% Compute the product of EVEN elements in a list
% flow model is (i, o)


prode([], 1). % this is the base case
prode([H|T], R):-
    H mod 2 =:= 0,
    prode(T, TP),
    R is TP*H.
prode([H|T], R):-
    H mod 2 =\= 0,
    prode(T, R).
    
