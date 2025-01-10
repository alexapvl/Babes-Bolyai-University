bubble_sort(List, Sorted):-
  bubble_sort_step(List, List1),
  !,
  (List1 = List -> Sorted = List ; bubble_sort(List1, Sorted)).

bubble_sort_step([], []).
bubble_sort_step([X], [X]).
bubble_sort_step([X,Y|Tail], [Y|Rest]) :-
  X > Y,
  bubble_sort_step([X|Tail], Rest).
bubble_sort_step([X,Y|Tail], [X|Rest]) :-
  X =< Y,
  bubble_sort_step([Y|Tail], Rest).

