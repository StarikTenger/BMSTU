;;; DAY OF WEEK ;;;
; works with prepared data
(define (day-of-week-raw d m y)
  (  remainder (+ d (quotient (* 31 m) 12) (+ y (quotient y 4)) (quotient (- 0 y) 100) (quotient y 400)) 7)
  )

; correction
(define (corr m)
  (quotient (- 14 m) 12)
  )

; day-of-week itself
(define (day-of-week d m y)
  ( day-of-week-raw d (+ m (* (corr m)  12) -2) (- y (corr m)) )
  )

;;; ROOTS ;;;
(define (D a b c) ; discriminant
  (- (* b b) (* 4 (* a c)))
  )

(define (roots a b c)
  (if (= a 0)
      ( list (- 0 ( / c b)) )
      ( if (< (D a b c) 0)
           (list)
           ( if (= (D a b c) 0)
                (list
                 ( / ( + (- 0 b) (sqrt (D a b c)) ) (* 2 a))
                 )
                (list
                 ( / ( + (- 0 b) (sqrt (D a b c)) ) (* 2 a))
                 ( / ( - (- 0 b) (sqrt (D a b c)) ) (* 2 a))
                 )
                )
           )
      )
  )

;;; GCD, LCM, PRIME ;;;
(define (my-gcd a b)
  (if (= b 0)
      a
      (my-gcd b (remainder a b))
      )
  )

(define (my-lcm a b)
  (* (/ a (my-gcd a b)) b)
  )

(define (prime-r? n x)
  (if(= x 1)
     #t
     (if (= (remainder n x) 0)
         #f
         (prime-r? n (- x 1))
         )
     )
  )

(define (prime? n)
  (prime-r? n ( - n 1))
  )