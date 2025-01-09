; Write a function that computes the sum of even numbers 
; and the decrease the sum of odd numbers,
; at any level of a list.

(defun process-number (X)
  (cond 
    ((= (mod X 2) 0) X) ; returns X if X is even
    (t (* -1 X)); -X otherwise
  )
)

(defun addEvenMinusOdd (L)
  (cond 
    ((null L) 0)
    ((numberp (car L)) (+ (process-number (car L)) (addEvenMinusOdd (cdr L))))
    (t (apply #'+ (mapcar #'addEvenMinusOdd L)))
  )
)

(print (addEvenMinusOdd '(1 (3))))
(print (addEvenMinusOdd '(1 2 (3 4) (5 (6 7 (8))))))