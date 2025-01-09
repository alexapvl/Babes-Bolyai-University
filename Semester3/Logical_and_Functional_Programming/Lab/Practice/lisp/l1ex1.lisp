(defun pointA (L N)
  (cond 
    ((null L) nil)
    ((= N 1) (car L))
    (t (pointA (cdr L) (- N 1)))
  )
)

(print (pointA '(1 2 3) 1))
(print (pointA '(1 2 3) 2))
(print (pointA '(1 2 3) 3))
(print (pointA '(1 2 3) 4))
(format t "~%################")

(defun pointB (E L)
  (cond 
    ((null L) nil) ; E was not found in L
    ((atom (car L))
      (cond 
        ((= (car L) E) T)
        (T (pointB E (cdr L)))
      )
    )
    (T (or (pointB E (car L)) (pointB E (cdr L))))
  )
)

(print (pointB 1 '(2 3 4 (2 3 ((1))))))
