(defun postorderType1 (tree aux)
  (cond 
    ((null tree) nil)
    ((> (cadr tree) 0) (postorderType1 (cddr tree) (cons (car tree) (cons (- (cadr tree) 1) aux))))
    ((= (cadr tree) 0) 
      (cond 
        ((null aux) (cons (car tree) (postorderType1 (cddr tree) aux)))
        (t (cons (car tree) (postorderType1 (cons (car aux) (cons (cadr aux) (cddr tree))) (cddr aux))))
      )
    )
  )
)

(print (postorderType1 '(A 2 B 0 C 2 D 0 E 1 F 0) nil))

;; (print (cons 'A '(B)))