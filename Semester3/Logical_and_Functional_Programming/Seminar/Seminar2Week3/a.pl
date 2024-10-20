% always make sure variables are bound before using them

countOcc([],_,0).

countOcc([X|T], X, Count):-
  countOcc(T,X,Count1),
  Count is Count1 + 1.

countOcc([H|T], X, Count):- H =\= X, 
  countOcc(T, X, Count).

%countOcc2(List, Elem, Collector, Count)
%(i,i,i,o)

countOcc2([], _, Col, Col).

countOcc2([X|T], X, Col, Res):-
  Col1 is Col + 1, % Col must be bound if we use it here
  countOcc2(T, X, Col1, Res).

countOcc2([H|T], X, Col, Res):-
  H =\= X,
  countOcc2(T, X, Col, Res).

% make a wrapper to hide the bounded variables which should not be changed by the user
countOcc2(L, X, Res):- countOcc2(L, X, 0, Res).

%removeUniques(List, Copy_Initial_List, Result)
%(i,i,o)

removeUniques([], _, []).
removeUniques([H|T], L, [H|Res]):- countOcc2(L, H, 0, Count),
  Count > 1, removeUniques(T, L, Res).

removeUniques([H|T], L, Res):- countOcc2(L, H, 0, Count),
  Count =< 1,
  removeUniques(T, L, Res).

%another wrapper function
removeUniques(L, R):- removeUniques(L, L, R).

addLast([], X, [X]).
addLast([H|T], X, [H|Res]):- addlast(T, X, Res).

removeInc([],[]).
removeInc([X], [X]).
removeInc([X,Y], []):- X =< Y. % condition to impose that a list has only 2 elements
removeInc([H1, H2, H3|T], R):- 
  H1 =< H2,
  H2 =< H3,
  removeInc([H2, H3|T], R).

removeInc([H1, H2, H3|T], R):- 
  H1 =< H2,
  H2 > H3,
  removeInc([H3|T], R).

removeInc([H1, H2|T], [H1|R]):- 
  H1 > H2, 
  removeInc([H2|T], R).




