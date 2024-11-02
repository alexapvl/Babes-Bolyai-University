% heterogenous list
% given a list which contains numbers and lists of numbers
% write all the necessary predicates that will remove all the odd elemnts from the sublists if the list has a mountain aspect
% mountain predicate
% main predicate

% mountain(List, Flag)
% flow(i, i)
mountain([_], 1) :- !.
mountain([H1, H2|T], 0) :- H1 > H2, !, mountain([H2|T], 1).
mountain([H1, H2|T], 0) :- H1 < H2, !, mountain([H2|T], 0).
mountain([H1, H2|T], 1) :- H1 > H2, mountain([H2|T], 1).

%wrapper to ensure that we dont have increasing at the beginning

mountain([H1, H2, H3|T]) :- H1 < H2, mountain([H1, H2, H3|T], 0).

% remove odd
% flow(i, o)
removeOdd([], []).
removeOdd([H|T], [H|R]) :- H mod 2 =:= 0, !, removeOdd(T, R).
removeOdd([H|T], R) :- removeOdd(T, R).

% removeOdd([H|T], [H|R]) :- H mod 2 =:= 0, !, removeOdd(T, R).
% removeOdd([H|T], R) :- H mod 2 =\= 0, !, removeOdd(T, R).

% main(List, ResultList)
% flow(i, o)

% main([], []).
% main([H|T], [R, Result]) :- 
%     is_list(H),
%     mountain(H),
%     removeOdd(H, R),
%     main(T, Result).

% main([H|T], [H| Result]) :- 
%     is_list(H),
%     \+ mountain(H),
%     removeOdd(H, R),
%     main(T, Result).

% main([H|T], [H|R]) :- not(is_list(H)), main(T, R).

main([], []).
main([[H|T]|R], [Result|R]) :- 
    mountain([H|T]),
    removeOdd([H|T], Result), !.

main([H|T], [H|R]) :- main(T, R).


%paranthesis problem
p(N, Open, Open, []) :- N =:= Open * 2.
p(N, Open, Close, ['('|R]) :- 
  N2 is N div 2,
  O1 is Open + 1, 
  O1 < N2, 
  p(N, O1, C, R).

p(N, Open, Close, [')'|R]) :-
  C1 is Close + 1,
  C1 =< Open,
  p(N, Open, C1, R).

p(N, R) :- p(N, 0, 0, R).

