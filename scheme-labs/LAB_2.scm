; 1 count
(define (count x xs)
  (if (pair? xs)
      (+ ( if ( equal? (car xs) x) 1 0 ) (count x (cdr xs)))
      0
      )
  )

; exapmles
(display "--- 1 ---\n" )
(count 'a '(a b c a)) ; 2
(count 'b '(a c d))   ; 0
(count 'a '())        ; 0
;

; 2 delete
(define (delete pred? xs)
  (if (pair? xs)
      (append
       (if (pred? (car xs))
           (list)
           (list (car xs)) )
       (delete pred? (cdr xs)))
      (list)
      )
  )

; exapmles
(display "--- 2 ---\n" )
(delete even? '(0 1 2 3)) ; (1 3)
(delete even? '(0 2 4 6)) ; ()
(delete even? '(1 3 5 7)) ; (1 3 5 7)
(delete even? '()) ; ()
;

; 3 iterate
(define (iterate f x n)
  (if (> n 0)
      (append (list x) (iterate f (f x) (- n 1)))
      (list)
      )
  )

; exapmles
(display "--- 3 ---\n" )
(iterate (lambda (x) (* 2 x)) 1 6) ; (1 2 4 8 16 32)
(iterate (lambda (x) (* 2 x)) 1 1) ; (1)
(iterate (lambda (x) (* 2 x)) 1 0) ; ()
;

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

; exapmles
(display "--- 4 ---\n" )
(intersperse 'x '(1 2 3 4)) ; (1 x 2 x 3 x 4)
(intersperse 'x '(1 2))     ; (1 x 2)
(intersperse 'x '(1))       ; (1)
(intersperse 'x '())        ; ()
;

; 5
; any? 
(define (any? pred? xs)
  (and
   (pair? xs)
   (or (pred? (car xs)) (any? pred? (cdr xs)) )
   )
  )
; all?
(define (all? pred? xs)
  (or
   (not (pair? xs))
   (and (pred? (car xs)) (all? pred? (cdr xs)) )
   )
  )

; examples
(any? odd? '(1 3 5 7)) ; #t
(any? odd? '(0 1 2 3)) ; #t
(any? odd? '(0 2 4 6)) ; #f
(any? odd? '()) ; #f
    
(all? odd? '(1 3 5 7)) ; #t
(all? odd? '(0 1 2 3)) ; #f
(all? odd? '(0 2 4 6)) ; #f
(all? odd? '()) ; #t ; Это - особенность, реализуйте её

; 6
(define (o-raw args)
  (if (pair? args)
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

; examples
(display "--- 6 ---\n" )
(define (f x) (* x 2))
(define (g x) (* x 3))
(define (h x) (- x))
    
((o f g h) 1) ; -6
((o f g) 1)   ; 6
((o h) 1)     ; -1
((o) 1)       ; 1
