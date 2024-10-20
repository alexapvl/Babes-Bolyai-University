% a) write a predicate to compute the union of two sets
% isMember -> (i,i)
isMember(X, [X|_]) :- !.

isMember(X, [_|L]) :-
    isMember(X, L).

% Base case: The union of an empty set and any set is that set.
% union -> (i,i,o)
union([], L, L).

% If the head of the first list is a member of the second, skip it and recurse.
union([H|T], L, R) :- 
    isMember(H, L), 
    union(T, L, R).

% If the head of the first list is not a member of the second, include it in the result.
union([H|T], L, [H|R]) :- 
    \+ isMember(H, L), % use \+ for negation
    union(T, L, R).

% b) write a predicate to determine the set of all the pairs of elements in a list

% first we try to make a predicate that finds all the distinct pairs frorm a list

% generate a pair in a non-deterministic way
% pair -> (i,i,o)
pair(A, [B|_], [A,B]).

pair(A, [_|T], P) :-
    pair(A,T,P).

% generate all pairs in a non-deterministic way
% pairs(i,o)
pairs([H|T], P) :-
    pair(H,T,P).

pairs([_|T], P) :-
    pairs(T,P).

% group all pairs using findall to collect all solutions (prolog also has setof and bagof)
% findall(X, goal_on_X, L)

% allPairs -> (i,o)
allPairs(L, LRes) :-
    findall(X, pairs(L, X), LRes).