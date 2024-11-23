; a) Write twice the n-th element of a linear list.
;Example: for (10 20 30 40 50) and n=3 will produce (10 20 30 30 40 50).

; double-n-th-elem subr 3 (list, integer, integer - starts from 1): list
(defun double-n-th-elem (L N &optional (I 1))
  (cond
    ((null L) L) ;checking for an empty list
    ((= N I) (cons (car L) (cons (car L) (double-n-th-elem (cdr L) N (+ I 1))))) ; Duplicate the N-th element
    (t (cons (car L) (double-n-th-elem (cdr L) N (1+ I)))))) ; Continue traversal

; Example usage:
(print "Doubling the third element of the list (10 20 30 40 50):")
(print (double-n-th-elem '(10 20 30 40 50) 3))
(format t "~%"); print a new line in common lisp

;b) Write a function to return an association list with the two lists given as parameters.
; Example: (A B C) (X Y Z) --> ((A.X) (B.Y) (C.Z))

(defun association-list (L1 L2) ; assume that L1 and L2 have the same length
  (cond
    ((null L1) L1)
    (t (cons (cons (car L1) (car L2)) (association-list (cdr L1) (cdr L2))))
  )
)
(print "Association list for the lists (A B C) and (X Y Z) is:")
(print (association-list `(A B C) `(X Y Z)))
(format t "~%")

;c) Write a function to determine the number of all sublists of a given list, on any level.
; A sublist is either the list itself, or any element that is a list, at any level. Example:
; (1 2 (3 (4 5) (6 7)) 8 (9 10)) => 5 lists:
; (list itself, (3 ...), (4 5), (6 7), (9 10)).

(defun count-sublists-on-all-levels (L) ; start counting from
  (cond
    ((null L) 1) ; we count the list itself at the end
    ; if we have an atom, we just continue with the recursion
    ((atom (car L)) (count-sublists-on-all-levels (cdr L)))
    ; in the case in which we have a nested-list, we add the result of
    ; the recursive calls between the remaining list and the nested-list itself
    (t (+ (count-sublists-on-all-levels (cdr L)) (count-sublists-on-all-levels (car L))))
  )
)
(print "The number of sublists on all levels of the list (1 2 (3 (4 5) (6 7)) 8 (9 10)) is:")
(print (count-sublists-on-all-levels `(1 2 (3 (4 5) (6 7)) 8 (9 10))))
(format t "~%")

;d) Write a function to return the number of all numerical atoms in a list at superficial level.
(defun count-numerical-atoms (L)
  (cond
    ((null L) 0)
    ((atom (car L)) (+ (count-numerical-atoms (cdr L)) 1))
    (t (count-numerical-atoms (cdr L)))
  )
)
(print "The number of numerical atoms at a superficial level of the list (1 2 (3 4) 5 (6 7 8) 9) is:")
(print (count-numerical-atoms `(1 2 (3 4) 5 (6 7 8) 9)))
