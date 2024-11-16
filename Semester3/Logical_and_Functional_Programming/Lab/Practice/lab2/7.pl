% a. Determine the position of the maximal element of a linear list.
% Eg.: maxpos([10,14,12,13,14], L) produces L = [2,5].

% maxElem(L: list, CurrMax: int, R: int)
maxElem(L, R):-
  maxElem(L, 0, R).
maxElem([], CurrMax, CurrMax).
maxElem([H|T], CurrMax, R):-
  H > CurrMax, !,
  maxElem(T, H, R).
maxElem([_|T], CurrMax, R):-
  maxElem(T, CurrMax, R).


myAppend([], X, [X]).
myAppend([H|T], X, [H|Res]):-
  myAppend(T, X, Res).
% maxpos(L: list, R: result)
maxPos(L, R):-
  maxElem(L, MaxElem),
  maxPos(L, R, [], MaxElem, 1).

% maxPos(L: list, R: result, MaxPosList: list, MaxElem: int, CurrentPos: int)
maxPos([], MaxPosList, MaxPosList, _, _).
maxPos([H|T], R, MaxPosList, MaxElem, CurrentPos):-
  H =:= MaxElem,
  myAppend(MaxPosList, CurrentPos, MaxPosList1),
  CurrentPos1 is CurrentPos + 1, !,
  maxPos(T, R, MaxPosList1, MaxElem, CurrentPos1).
maxPos([_|T], R, MaxPosList, MaxElem, CurrentPos):-
  CurrentPos1 is CurrentPos + 1,
  maxPos(T, R, MaxPosList, MaxElem, CurrentPos1).
