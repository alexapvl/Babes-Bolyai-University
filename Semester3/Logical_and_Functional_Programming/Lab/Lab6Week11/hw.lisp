(defun find-level (tree node level aux)
  (cond
    ((null tree) nil) ; node was not found
    ((equal (car tree) node) level) ; node was found, return the level
    ; node is not a leaf, add it to aux with children - 1 and search for children, also increment level by 1
    ((> (cadr tree) 0) (find-level (cddr tree) node (+ level 1) (cons (car tree) (cons (- (cadr tree) 1) aux)))) 
    ; at this point (cadr tree) = 0, aux is empty, so we have to search for the node in the rest of the tree, level remains the same
    ((null aux) (find-level (cddr tree) node level aux))
    ; here we have something in aux and (cadr tree) = 0, so we remove the node and bring back from aux the parent node as it is
    ; because we decremented the children when we inserted it in aux, the level will decrease by 1 because we climb up in the tree (backtrace)
    (T (find-level (cons (car aux) (cons (cadr aux) (cddr tree))) node (- level 1) (cddr aux)))
  )
)
(format t "The node A is on level: ")
(print (find-level '(A 2 B 0 C 2 D 0 E 0) 'A 0 nil))
(format t "~%~%")
(format t "The node B is on level: ")
(print (find-level '(A 2 B 0 C 2 D 0 E 0) 'B 0 nil))
(format t "~%~%")
(format t "The node C is on level: ")
(print (find-level '(A 2 B 0 C 2 D 0 E 0) 'C 0 nil))
(format t "~%~%")
(format t "The node D is on level: ")
(print (find-level '(A 2 B 0 C 2 D 0 E 0) 'D 0 nil))
(format t "~%~%")
(format t "The node E is on level: ")
(print (find-level '(A 2 B 0 C 2 D 0 E 0) 'E 0 nil))
(format t "~%~%")
(format t "The node F should not be found because it's not in the tree: ")
(print (find-level '(A 2 B 0 C 2 D 0 E 0) 'F 0 nil))
(format t "~%~%")

;; another example
;; (print (find-level '(F 2 G 1 H 0 I 2 J 0 K 1 L 0) 'L 0 nil))
