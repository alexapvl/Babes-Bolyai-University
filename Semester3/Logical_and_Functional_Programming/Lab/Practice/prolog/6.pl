% a. Determine the product of a number represented as digits in a list to a given digit.
% Eg.: [1 9 3 5 9 9] * 2 => [3 8 7 1 9 8]

% prod(L: list, X: int, R: list)
prod(L, X, R):-
  reverseList(L, R1),
  prod(R1, X, 0, [], R).

prod([], _, 0, CurrRes, CurrRes).
prod([H|T], X, C, CurrRes, R):-
  P is H * X + C,
  P > 10, !,
  StepRes is P mod 10,
  StepCarry is P // 10,
  prod(T, X, StepCarry, [StepRes|CurrRes], R).

prod([H|T], X, C, CurrRes, R):-
  P is H * X + C,
  prod(T, X, 0, [P|CurrRes], R).

% reverseList(L, R)

reverseList([X], [X]).
reverseList([H|T], R):-
  reverseList(T, R1), !,
  myAppend(R1, H, R).

myAppend([], X, [X]).
myAppend([H|T], X, [H|Res]):-
  myAppend(T, X, Res).

