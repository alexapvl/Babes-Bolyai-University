% a. Define a predicate to determine the longest sequences of consecutive even numbers (if exist more maximal
% sequences one of them).
% b. For a heterogeneous list, formed from integer numbers and list of numbers, define a predicate to replace
% every sublist with the longest sequences of even numbers from that sublist.
% Eg.: [1, [2, 1, 4, 6, 7], 5, [1, 2, 3, 4], 2, [1, 4, 6, 8, 3], 2, [1, 5], 3] =>
% [1, [4, 6], 5, [2], 2, [4, 6, 8], 2, [], 3]

% a.
% longestEvenSequence(L: list, Rez: list, LenRez: int, CurrList: list, CurrLen: int)
% flow model: (i, o, i, i, i)
longestEvenSequence(List, Res):-
    longestEvenSequence(List, [], 0, [], 0, Res).
longestEvenSequence([], _, _, Acc, _, Acc).
longestEvenSequence([H|T], AccCur, LenCur, Acc, LenAcc, Res) :-
    0 =:= H mod 2,
    myAppend(AccCur, H, NewAccCur),
    LenCur1 is LenCur + 1,
    (LenCur1 > LenAcc -> longestEvenSequence(T, NewAccCur, LenCur1, NewAccCur, LenCur1, Res), ! ; longestEvenSequence(T, NewAccCur, LenCur1, Acc, LenAcc, Res)).
longestEvenSequence([H|T], _, _, Acc, LenAcc, Res) :-
    1 =:= H mod 2,
    longestEvenSequence(T, [], 0, Acc, LenAcc, Res).

myAppend([], X, [X]).
myAppend([H|T], X, [H|Res]):-
  myAppend(T, X, Res).

% b.

% solve(L: list, Res: list)
solve([], []).
solve([[H|T]|T1], [Res1|Res]):-
  longestEvenSequence([H|T], Res1), !, % ! is there in order to ensure that it will not branch out on the last case with [H|T]
  solve(T1, Res).
solve([H|T], [H|Res]):-
  solve(T, Res).
