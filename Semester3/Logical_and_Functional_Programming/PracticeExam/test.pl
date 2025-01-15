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
