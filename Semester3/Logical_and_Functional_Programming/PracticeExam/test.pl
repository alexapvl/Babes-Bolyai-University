f(0,0):-!.
f(I, Y):-
  J is I-1,
  f(J, V),
  V>1, !,
  K is I-2,
  Y is K.
f(I, Y):-
  J is I-1,
  f(J, V),
  Y is V+1.

g(1,1):-!.
g(K, X):-K1 is K-1, g(K1, Y), Y>1, !, K2 is K1-1, X is K2.
g(K, X):-K1 is K-1, g(K1, Y), Y>0.5, !, X is Y.
g(K, X):-K1 is K-1, g(K1, Y), X is Y-1.

myg(1,1):-!.
myg(K,X):-
  K1 is K-1,
  myg(K1, Y),
  (Y =<0.5, X is Y-1 ; (Y > 1, K2 is K1 - 1, X is K2 ; X is Y)).


prod([], 1).
prod([H|T], Res):-
  prod(T, R1),
  Res is R1 * H.