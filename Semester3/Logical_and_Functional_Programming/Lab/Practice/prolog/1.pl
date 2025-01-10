% a. Sort a list with removing the double values. E.g.: [4 2 6 2 3 4] --> [2 3 4 6]
% b. For a heterogeneous list, formed from integer numbers and list of numbers, write a predicate to sort every
% sublist with removing the doubles.
% Eg.: [1, 2, [4, 1, 4], 3, 6, [7, 10, 1, 3, 9], 5, [1, 1, 1], 7] =>
% [1, 2, [1, 4], 3, 6, [1, 3, 7, 9, 10], 5, [1], 7].

% a.
% bubble_sort(L: list, R: list)
% flow model: (i, o)
bubble_sort(List, Sorted):-
  bubble_sort_step(List, List1),
  !,
  (List1 = List -> 
  Sorted = List ; bubble_sort(List1, Sorted)).

% bubble_sort_step(L: list, R: list)
% flow model: (i, o)
bubble_sort_step([], []).
bubble_sort_step([X], [X]).

bubble_sort_step([X,Y|Tail], [Y|Rest]) :-
  X > Y,
  bubble_sort_step([X|Tail], Rest).

bubble_sort_step([X,Y|Tail], [X|Rest]) :-
  X =< Y,
  bubble_sort_step([Y|Tail], Rest).

% remove_doubles(L: list, R: list)
% flow model: (i, o)
% removes the doubles of a sorted list
remove_doubles([], []).
remove_doubles([X], [X]).

remove_doubles([X, Y|Tail], [X|Rest]):-
  X =\= Y,
  remove_doubles([Y|Tail], Rest), !.

remove_doubles([X, Y|Tail], Rest):-
  X =:= Y,
  remove_doubles([Y|Tail], Rest), !.

a(List, Result):-
  bubble_sort(List, List1),
  remove_doubles(List1, Result).

% b.
b([], []).
b([[H|T]|Tail], [R1|Result]):-
  a([H|T], R1), !,
  b(Tail, Result).
b([H|T], [H|Result]):-
  b(T, Result).

