(define (apples n)
  (define r (remainder n 10))
  (if (= (remainder (floor (/ n 10)) 10) 1)
      (list n "яблок")
      (if (equal? r 1)
          (list n "яблоко")
          ( if (and (< r 5) (not (= r 0)))
               (list n "яблока")
               (list n "яблок")))))