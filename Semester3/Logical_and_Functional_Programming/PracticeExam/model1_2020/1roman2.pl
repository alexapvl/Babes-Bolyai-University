f([], 0).
f([H|T], S):-
  f(T, S1),
  S1<H, !,
  S is H.
f([_|T], S):-
  f(T, S1),
  S is S1.

mymin([], Rez).
mymin([H|T], Rez):-
  Rez <= H,
  mymin(T, Rez).
mymin([H|T], Rez):-
  mymin(T, H).