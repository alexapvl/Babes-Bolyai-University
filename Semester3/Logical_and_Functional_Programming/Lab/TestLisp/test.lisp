(defun is-in-list (elem L)
  (cond 
    ((null L) nil)
    ((= elem (car L)) T)
    (T (is-in-list elem (cdr L)))
  )
)

;; (print (is-in-list 1 '(1 2 3)))
;; (print (is-in-list 4 '(1 2 3)))

(defun insert-in-place (elem L flag)
  (cond 
    ((and (null L) flag) nil)
    ((and (null L) (null flag)) (list elem))
    ((< elem (car L))
      (cond 
        ((null flag) (cons elem (insert-in-place elem L T))); insert element and change flag
        (t (cons (car L) (insert-in-place elem (cdr L) flag))); if the flag is true it means we already inserted the element and we should just insert the next element in L
      )
    )
    (t (cons (car L) (insert-in-place elem (cdr L) flag)))
  )
)

;; (print (insert-in-place 4 '(1 3) nil))

(defun main (L res)
  (cond
    ((null L) res)
    ((is-in-list (car L) res) (main (cdr L) res)) ;do not insert in result if the element already is inside it  
    (t (main (cdr L) (insert-in-place (car L) res nil)))
  )
)

(print (main '(5 1 3 4 2 1 3) nil))
(print (main '(7 7 3 3 5 5 1 1) nil))
(print (main '(5 4 3 2 1) nil))
(print (main '(5 5 4 4 3 3 2 1 2 1) nil))
(print (main '(1 2 3 1 5 3 1 4) nil))