%% non deterministic solution, will find all occurences of E in the list
%% (i,i)
%% (o,i) -> will list all elements in the list
member1(E,[E|_]).
member1(E,[_|L]):-
  member1(E,L).

member2(E,[E|_]):- !. %% with cut, will stop after the first occurence
member2(E,[_|L]):-
  member2(E,L).

%% add an element to a list
%% version 1

%% (i,i,o) - deterministic

add(E,[], [E]).
add(E, [H|T], [H|Rez]):- %% H is a bound variable here and Rez is free, will be formed by the recursive call
  add(E,T,Rez). %% E will be added at the end of T

%% version 2
