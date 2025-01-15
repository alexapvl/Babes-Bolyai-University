(defun solution (L)
  (cond
    ((null L) nil)
    ((atom L)
      (cond
        ((numberp L) 
          (cond 
            ((= (mod L 3) 0) nil)
            (T L)
          ))
        (T L)
      ))
    (T (mapcar #'solution L))
  )
)

(print (solution '(1(2(C)))))
(print (solution '(1(2 A(3 A)) (6))))
