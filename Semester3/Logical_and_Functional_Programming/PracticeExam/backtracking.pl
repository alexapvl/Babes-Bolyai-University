mysubset([], []).
mysubset([H|T], [H|Res]) :-
  mysubset(T, Res).
mysubset([_|T], Res):-
  mysubset(T, Res).

test_subset(Res) :- findall(R, mysubset([1,2,3], R), Res).

mycombination(_, 0, Acc, Acc).
mycombination([H|T], K, Acc, Res) :-
    K > 0,
    K1 is K - 1,
    mycombination(T, K1, [H|Acc], Res).
mycombination([_|T], K, Acc, Res) :-
    K > 0,
    mycombination(T, K, Acc, Res).

test_combination(Res) :- findall(R, mycombination([2,5,3,4,10], 2, [], R), Res).

mypermutation([], []).
mypermutation(List, [X|Perm]) :-
  append(Left, [X|Right], List),
  append(Left, Right, Rest),
  mypermutation(Rest, Perm).

test_permutation(Res) :- findall(R, mypermutation([1,3,4,6], R), Res).

myarrangements(List, K, Res) :-
  mycombination(List, K, [], Rez),
  mypermutation(Rez, Res).

test_arrangements(Res) :- findall(R, myarrangements([2,5,3,4,10], 2, R), Res).

myappend([], List, List).
myappend([X|Rest], List, [X|Result]) :- 
    myappend(Rest, List, Result).

prod([], 1).
prod([H|T], Res):-
  prod(T, R1),
  Res is R1 * H.

main(List, K, P, Arrangement):-
  myarrangements(List, K, Arrangement),
  prod(Arrangement, P).