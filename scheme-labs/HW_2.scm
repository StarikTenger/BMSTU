;;; 1 LISTS ;;;
; range
(define (my-range a b d)
  (if (< a b)
      (append (list a) (my-range (+ a d) b d))
      '()
      )
  )
; flatten
(define (my-flatten-raw xs l)
  ( if (pair? xs)
       (if (pair? (car xs))
           (append (list) (my-flatten-raw (car xs) l) (my-flatten-raw (cdr xs) l))
           (append l (list (car xs)) (if (pair? (cdr xs))
                                         (my-flatten-raw (cdr xs) l)
                                         (list)
                                         )
                   )
           )
       xs
       )
  )
(define (my-flatten xs)
  (my-flatten-raw xs '())
  )

; element
(define (my-element? x xs)
  (and (pair? xs) (or (equal? (car xs) x) (my-element? x (cdr xs))))
  )

; filter
(define (my-filter pred? xs)
  (if (pair? xs)
      (append (if (pred? (car xs)) (list (car xs)) (list) ) (my-filter pred? (cdr xs)))
      (list)
      )
  )

; fold
(define (my-fold-right op xs)
  (if (pair? (cdr xs))
      (op (car xs) (my-fold-right op (cdr xs)))
      (car xs)
      )
  )

(define (my-fold-left-raw op xs val)
  (if (pair? (cdr xs))
      (my-fold-left-raw op (cdr xs) (op val (car xs)))
      (op val (car xs))
      )
  )

(define (my-fold-left op xs)
  (my-fold-left-raw op (cdr xs) (car xs))
  )

;;; 2 SETS ;;;

