%% cannot have (o, i) because N needs to be bound
%% failing means unability to be proven true!

%% if a list is empty, it cannot be split in head and tail
%% the head is always an element and the tail is a list(even if it is empty)
%% [a,b,c] = [a|[b,c]] = [a,b | [c]] = [a,b,c | []]


%% Predicate fact1 fails (correctly) when N<0.

fact1(0,1).

fact1(N,F) :-
  N>0,
  N1 is N-1,
  fact1(N1,F1),
  F is N*F1.

%% will go into infinite reccursion if N<0
fact2(0,1) :- 
  !. %% works like break in a loop, stops finding other solutions i.e. "if you get success here, don't go further away", 
     %%"don't explore further alternatives"
     %% !. is called (in prolog): "cut"

fact2(N,F) :-
  N1 is N-1,
  fact2(N1,F1),
  F is N*F1.

fact3(N,F) :-
  N>0, N1 is N-1,
  fact3(N1,F1),
  F is N*F1.

fact3(0,1).
