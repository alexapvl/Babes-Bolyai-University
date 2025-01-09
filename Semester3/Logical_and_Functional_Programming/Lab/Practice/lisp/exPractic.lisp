; sa se stearga dintr-o lista neliniara toate sublistere liniare care au numar par de elemente

(defun count-elements (L)
  (cond 
    ((null L) 0)
    (t (+ 1 (count-elements (cdr L))))
  )
)

;; (print (count-elements '(1 2 3)))

(defun is-linear-list (L) ;; 0 means false and 1 means true
  (cond
    ((null L) 1)
    ((atom (car L)) (is-linear-list (cdr L)))
    (t 0)
  )
)

;; (print (is-linear-list '(1 2 3)))
;; (print (is-linear-list '(5 6 (7 8))))


(defun solution (L)
  (cond 
    ((null L) nil)
    ((atom (car L)) (cons (car L) (solution (cdr L))))
    ((= (is-linear-list (car L)) 1) ; verify if the list is linear
      (cond 
        ((= (mod (count-elements (car L)) 2) 0) (solution (cdr L))); verify if the number of elements is even
        (t (cons (solution (car L)) (solution (cdr L))))
      )
    )
    (t (cons (solution (car L)) (solution (cdr L))))
  )
)

(print (solution '((2 3 4) (6 (7 8) ((7 9) 8)) (6 8) 9)))
;; (print (solution '((2 3 4) (5 6 (7 8)))))