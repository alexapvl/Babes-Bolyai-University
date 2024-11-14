% Two integers, n and m are given. Write a predicate to determine all possible sequences of numbers
% from 1 to n, such that between any two numbers from consecutive positions, the absolute difference
% to be >= m

% candidatesListAux(X: int, N: int, Rez: list)
% flow model: (i, i, o) - (int, int, list)
% returns a list of the form [1, ..., N]
candidatesListAux(N, N, [N]):-!.
candidatesListAux(X, N, [X|Rez]):-
  X =< N,
  X1 is X + 1,
  candidatesListAux(X1, N, Rez).

% candidatesList(N: int, Rez: list)
% flow model: (i, o) - (int, list)
% returns a list of the form [1, ..., N] - wrapper of aux function
candidatesList(N, Rez):-
  candidatesListAux(1, N, Rez).

% candidateAux(E: int, L: list)
% flow model: (o, i) - (int, list)
% assigns to E the head of the list L given as input parameter
candidateAux(E, [E|_]).
candidateAux(E, [_|T]):-
  candidateAux(E, T).

% candidate(E: int, N: int)
% flow model: (o, i) - (int, int)
% returns all elements from 1 to N using the candidatesList function to generate the list and the auxiliary function to assign the elements
candidate(E, N):-
  candidatesList(N, L),
  candidateAux(E, L).

% not in list
% flow model: (i, i) - (int, list)
% returns true if the element X is not in the list L
notInList(_, []).
notInList(X, [H|T]):-
  X \= H,
  notInList(X, T).

% absoluteDifference(X1: int, X2: int, M: int)
% flow model: (i, i, i) - (int, int, int)
% returns true if the absolute difference between X1 and X2 is greater or equal to M
constraints(X1, X2, M):-
  X1 >= X2,
  K is X1 - X2,
  K >= M.
constraints(X1, X2, M):-
  X1 < X2,
  K is X2 - X1,
  K >= M.

f(N, M, R):-
  candidate(E, N),
  fAux(N, M, R, [E]).

% fAux(N: int, M: int, R: list, E: int, L: list)
% flow model: (i, i, o, i, i) - (int, int, list, int, list)
% returns all possible sequences of numbers from 1 to N, such that between any two numbers from consecutive positions, the absolute difference to be >= M
fAux(_, _, R, R).
fAux(N, M, R, [H|T]):-
  candidate(E, N),
  notInList(E, [H|T]),
  constraints(E, H, M),
  fAux(N, M, R, [E, H|T]).

func(N, M, R):-
  findall(RPartial, f(N, M, RPartial), R).


