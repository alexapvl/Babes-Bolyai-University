% maxElem(L: list, R: int)
% flow model: (i ,o)
maxElem(L, R):-
  maxElem(L, -1, R). % initialise with -1 assuming that all elements from L are positive integers

% maxElem(L: list, CurrMax: int, Rez: int)
% flow model: (i, i, o)
maxElem([], CurrMax, CurrMax).
maxElem([H|T], CurrMax, Rez):-
  H > CurrMax, !, % cut operator because we don't want to explore other branches if we change the max elem
  maxElem(T, H, Rez).
maxElem([_|T], CurrMax, Rez):-
  maxElem(T, CurrMax, Rez).

% myAppend(L: list, X: int, R: list)
% flow model: (i, i, o)
myAppend([], X, [X]).
myAppend([H|T], X, [H|Res]):-
  myAppend(T, X, Res).

% solve(L: list, R: list)
% flow model: (i, o)
% ex: solve([10,9,10,8,7],R).
solve(L, R):-
  maxElem(L, MaxElem),
  solve(L, MaxElem, 1, [], R).

% solve(L: list, MaxElem: int, CurrPos: List, Res: List)
% flow model: (i, i, i, i, o)
solve([], _, _, CurrPos, CurrPos).
solve([H|T], MaxElem, CurrIndex, CurrPos, Res):-
  H =:= MaxElem, !, % cut operator beacause we don't want to explore other branches if we found an index of the max elem
  CurrIndex1 is CurrIndex + 1,
  myAppend(CurrPos, CurrIndex, CurrPos1),
  solve(T, MaxElem, CurrIndex1, CurrPos1, Res).

solve([_|T], MaxElem, CurrIndex, CurrPos, Res):-
  CurrIndex1 is CurrIndex + 1,
  solve(T, MaxElem, CurrIndex1, CurrPos, Res).
