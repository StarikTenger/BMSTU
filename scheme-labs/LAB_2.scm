; 1 count
(define (count l x)
  (if (pair? l)
      (+ ( if ( = (car l) x) 1 0 ) (count (cdr l) x))
      0
      )
  )

; 2 delete
(define (delete l pred?)
  (if (pair? l)
      (append (if (pred? (car l)) (list) (list (car l)) ) (delete (cdr l) pred?))
      (list)
      )
  )

; 3 iterate
(define (iterate f x n)
  (if (> n 0)
      (append (list x) (iterate f (f x) (- n 1)))
      (list)
      )
  )

; 4 intersperse
(define (intersperse e xs)
  (if (pair? xs)
      (if (pair? (cdr xs))
          (append (list (car xs)) (list e) (intersperse e (cdr xs)))
          (list (car xs))
          )
      (list)
      )
  )

; 5
; any? 
(define (any? pred? xs)
  (if (pair? xs)
      (or (pred? (car xs)) (any? pred? (cdr xs)) )
      #f
      )
  )
; all?
(define (all? pred? xs)
  (if (pair? xs)
      (and (pred? (car xs)) (all? pred? (cdr xs)) )
      #t
      )
  )

; 6
(define (o-raw args)
  (if (pair? (cdr args))
      (lambda (x) ((o-raw (cdr args)) ((car args) x)))
      (if (pair? args)
          (car args)
          (lambda (x) x)
       )
      )
  )

(define (o . args)
  (o-raw args)
  )








