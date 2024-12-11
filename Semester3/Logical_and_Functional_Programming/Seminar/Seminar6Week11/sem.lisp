; (1 2 3) => (3 6 9)
(defun triple (x) (* 3 x))
(mapcar #'triple '(1 2 3))
; (list (triple 1) (triple 2) (triple 3)) 
; (list 3 6 9)
; (3 6 9)

; (1 (a (2 b)) 3 4 2)

(defun triple_l(X)
  (cond
    ((numberp X) (* 3 X))
    ((atom X) X)
    (t (mapcar #'triple_l(X)))
  )
)

; product of numerical elems in a nonlinear list
; ex: (1 a b 2 (1 5 2) 1 1)
(defun prod (l)
  (cond
    ((numberp l) l)
    ((atom l) 1)
    (t (apply '* (mapcar #'prod l)))
  )
)

; compute the number of nodes from the even levels of an n-ary tree representation as
; (root (subtree1) (subtree2) ... (subtree n)). Root level is 1.
;ex: (A (B (D (G) (H)) (E (I)) (C (F (J (L)) (K))))) => 7
;     0  1  0  1   1    0  1    1  0  1  0    1      => 7
(defun count (tree level)
  (cond 
    ((and (atom tree) (= (mod level 2) 1)) 0)
    ((atom tree) 1)
    (t (apply '+ (mapcar #' (lambda (ti) (count ti (+ 1 level))) tree))) ; use lambda function bcs we have 2 params
  )
)

; lambda obs: each element of tree will be passed in lambda function and then the count will be called with it

; given a nonlinear list, compute the number of sublists (including the initial list) where the first numeric atom is 5 (at any level)
; ex: (A 5 ( B C D) 2 1 (G (5 H) & D) 11 14)
;=> (5 2 1 5 7 11 14), (5)
(defun transform (l)
  (cond 
    ((null x) ())
    ((numberp l) (l))
    ((atom l) nil)
    (t (apply #'append (mapcar #'transform l))) ; (append (list (transform l1) (transform l2) ... (transform ln))))) <=> (mapcan #'transform l)
  )
)

