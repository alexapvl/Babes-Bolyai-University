% merge_sort(L: list, R: list)
% flow model: (i, o)
% performs sorting of the list L using the merging algorithm
merge_sort([], []).
merge_sort([X], [X]).

merge_sort(List, Sorted) :-
  split_list(List, Left, Right),
  merge_sort(Left, SortedLeft),
  merge_sort(Right, SortedRight),
  merge(SortedLeft, SortedRight, Sorted), !.

% split_list(L1: list, L2: list, R: list)
% flow model: (i, i, o)
% splits two lists in two halves
split_list([], [], []).
split_list([X], [X], []).
split_list([X,Y|Tail], [X|Left], [Y|Right]) :-
  split_list(Tail, Left, Right).

% merge(L1: list, L2: list, R: list)
% flow model: (i, i, o)
% performs the merge of two lists by appending at the beginning of the result the smallest element, 
% one at a time until one of the lists becomes empty
merge([], List2, List2).
merge(List1, [], List1).
merge([H1|T1], [H2|T2], [H1|Merged]) :-
  H1 =< H2,
  !,
  merge(T1, [H2|T2], Merged).
merge([H1|T1], [H2|T2], [H2|Merged]) :-
  merge([H1|T1], T2, Merged).

b([], []).
b([[H|T]|Tail], [R1|Result]):-
  merge_sort([H|T], R1), !,
  b(Tail, Result).
b([H|T], [H|Result]):-
  b(T, Result).
