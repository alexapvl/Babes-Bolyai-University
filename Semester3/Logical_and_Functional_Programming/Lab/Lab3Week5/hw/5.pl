% a)
% sub(List, ElemToSubstitute, ListTemp, InitialList, Result)
% ListTemp will be a list from which we take the elements and insert them into the result, but we need a copy of it in order to restitute after the insertion of all elements
%from that list
% flow model: (i, i, i, i, o)

sub([], _, _, _, []).

sub([_|T], X, [], InitialList, Result) :-
    sub(T, X, InitialList, InitialList, Result).

sub([H|T], X, [H1|T1], InitialList, [H1|Result]) :-
    H =:= X,
    sub([H|T], X, T1, InitialList, Result).

sub([H|T], X, ListTemp, InitialList, [H|Result]) :-
    H =\= X,
    sub(T, X, ListTemp, InitialList, Result).
    

% b)
% subh(List, InitialList, Result)
% flow model: (i, i, o)

subh([], _, []).

subh([[H|T]|T1], InitialList, [Result1|Result]) :-
    sub([H|T], H, InitialList, InitialList, Result1), !, 
    subh(T1, InitialList, Result).

subh([H|T], InitialList, [H|Result]) :-
    subh(T, InitialList, Result).

