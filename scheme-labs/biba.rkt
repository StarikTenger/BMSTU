(define (fact n)
  (define (tail i n acc)
    (if (= i n)
        acc
        (tail (+ i 1) n (* acc i))))
  (tail 1 ( + n 1) 1))