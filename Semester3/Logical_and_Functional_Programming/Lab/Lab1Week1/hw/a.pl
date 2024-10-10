% Base case: The union of an empty set and any set is that set.
union([], L, L).

% If the head of the first list is a member of the second, skip it and recurse.
union([H|T], L, R) :- 
    member(H, L), 
    union(T, L, R).

% If the head of the first list is not a member of the second, include it in the result.
union([H|T], L, [H|R]) :- 
    \+ member(H, L), 
    union(T, L, R).
