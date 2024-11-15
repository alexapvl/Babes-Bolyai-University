% For a list a1... an with integer and distinct numbers, define a predicate to determine all subsets with
% sum of elements divisible with n.

getElem([E|T], E , T).
getElem([_|T], E, R):- getElem(T, E, R).

%subSetDivN(list, N, S, Res)
subSetDivN(_, N, S, []) :-
  S mod N =:= 0.
subSetDivN([H|T], N, S, [H|Res]):-
  S1 is S + H,
  subSetDivN(T, N, S1, Res).
subSetDivN([H|T], N, S, [H|Res]):-
  S mod N =\= 0,
  subSetDivN(T, N, S, Res).

solve(L, N, [E|Res]):- getElem(L, E, Rest), subSetDivN(Rest, N, E, Res).

