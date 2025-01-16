(print (mapcan #'(lambda (x) (if (> x 2) (list x x) (list x)))
        '(1 2 3 4)))  ; Returns (1 2 3 3 4 4)

(defun G (L)
  (list (car L) (car L))
)

(setq Q 'G)
(setq P 'Q)
(print (eval P))
(print (funcall (eval P) '(A B C)))
