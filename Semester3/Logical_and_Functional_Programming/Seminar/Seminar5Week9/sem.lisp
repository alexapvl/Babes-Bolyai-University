;; (defun merge-lists (l1 l2)
;;   (cond
;;     ((null l1) l2)
;;     ((null l2) l1)
;;     ((> (car l1) (car l2)) (cons (car l2) (merge-lists l1 (cdr l2)))))
;;     ((< (car l1) (car l2)) (cons (car l1) (merge-lists (cdr l1) l2)))
;;     (T (cons (car l1) (merge-lists (cdr l1) (cdr l2))))
;;   )
;; (print (merge-lists '(1 2 3 4) '(3 4 5 6)))

(defun remove-element (element list)
  (cond ((null list) nil)
        ((listp (car list)) (cons (remove-element element (car list)) (remove-element element (cdr list))))
        ((equal (car list) element) (remove-element element (cdr list)))
        (t (cons (car list) (remove-element element (cdr list))))))

(print (remove-element 3 '(1 2 3 (4 3 5) 6 3)))


(defun getMin (l current)
  (cond 
    ((numberp (car l))
      (cond
        ((< (car l) current))
      )
    )
    ()
  )
)

(defun minPos (l min pos)
  (cond 
    ((null l) nil)
    ((equal min (car l)) (cons pos (minPos (cdr l) min (+ pos 1))))
    (T (minPos (cdr l) min (+ pos 1)))
  )
)

(defun main (l)
  (cond 
    ((not (null (getMin l nil))) (minPos l (getMin l nil) 1))
  )
)

(print (main '(2 b c -4 -6)))

; add an element to a linear list every nth position
(defun addN (l pos e n)
  (cond
    ((and (null l) (= pos 0)) (list e)) ; or (cons e nil) -> same effect
    ((null l) nil)
    ((= pos n) (cons (car l) (cons e (addN (cdr l) e 1 n))))
    (T (cons (car l) (addN (cdr l) e (+ pos 1) n)))
  )
)
(print (addN '(1 2 3 4 5 6 7 8 9) 100 1 3))


; problem 2 from l2
(defun kLevelElems (tree current level aux)
  (cond 
    ((null tree) nil)
    ((= (cadr tree) 0)
      (cond 
        ((= level current) (cons (car tree) (kLevelElems )))

    ) 
  )
)

;; take solutions from prof site!!!!!!!!!!!!!!
