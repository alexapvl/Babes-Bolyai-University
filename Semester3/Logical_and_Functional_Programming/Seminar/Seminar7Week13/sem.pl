% nrocc(Elem, List, Count)
% nrocc(i, i, i, o)
nrocc(_, [], Count, Count):- !.
% nrocc(_, List, Count, Res):- List=[], Res is Count.
nrocc(X, [X|T], C, Res):-
  !,
  C1 is C + 1,
  nrocc(X, T, C1, Res).
nrocc(X, [E|T], C, Res):-
  nrocc(X, T, C, Res).

nrocc(X, L, Res):- nrocc(X, L, 0, Res).


% remove(List, CopyOfList, Result)
% remove(i, i, o)
remove([], _, []).
remove([H|T], Copy, Res):-
  nrocc(H, Copy, Count), 
  Count > 1,
  remove(T, Copy, Res).

remove([H|T], Copy, [H|Res]):-
  remove(T, Copy, Res).

remove(L, R):-
  remove(L, L, R).

% comb(List, K, Result)
comb(_, 0, []).
comb([H|T], K, [H|R]):- K > 0, K1 is K - 1, comb(T, K1, R).
comb([_|T], K, R):- K > 0, comb(T, K, R).


comb2(_, 0, []).

getElem([H|_], H, []).
getElem([H|T], Elem, [H|Res]):- getElem(T, Elem, Res).

comb2(_, 0 Col, Col).
comb2(L, K, [H|Col], Res):- getElem(L, X, L1), 
K > 0,
X < H, 
K1 is K - 1,
comb2(L1, K1, [H|Col], Res):-
  K > 0,
  getElem(L, _, L1),
  comb2(L1, K , [H|Col], Res).

comb2(L, K, Res):- K1 is K - 1, getElem(L, E, L1), comb2(L1, K1, [E])