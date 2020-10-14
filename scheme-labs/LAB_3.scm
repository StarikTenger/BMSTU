(load "trace.scm")
(load "unit-test.scm")

; task 1
(define (zip . xss)
  (if (or (null? xss)
          (null? (trace-ex (car xss)))) ; Здесь...
      '()
      (cons (map car xss)
            (apply zip (map cdr (trace-ex xss)))))) ; ... и здесь
(zip '(1 2 3) '(one two three))

; task 3
(define (ref-get xs i)
  (let (
        (l (cond
             ((list? xs) xs)
             ((string? xs) (string->list xs))
             ((vector? xs) (vector->list xs))
             )))
    (define len (length l))
    (if (or (not (integer? i)) (< i 0) (>= i len))
        #f
        (list-ref l i))))

(define (list-insert xs i val acc)
  (if (= i 0)
      (append (reverse acc) (list val) xs)
      (list-insert (cdr xs) (- i 1) val (cons (car xs) acc))))

(define (ref-insert xs i val)
  (let
      ((listed (cond
                 ((list? xs) xs)
                 ((string? xs) (string->list xs))
                 ((vector? xs) (vector->list xs)))))
    (begin
      (define len (length listed))
      (if (or (not (integer? i))
              (< i 0)
              (> i len)
              (and (string? xs) (not (char? val))))
          
          #f
          (let
              ((l1 (list-insert
                    listed
                    i val (list))))
            (cond
              ((list? xs) xs)
              ((string? xs) (list->string l1))
              ((vector? xs) (list->vector l1))
              ))
          )
      )))

(define (ref xs i . args)
  (if (null? args)
      (ref-get xs i)
      (ref-insert xs i (car args))))

; task 4

(define (diff-of-squares exp)
  (define a (cadadr exp))
  (define b (car (cdaddr exp)))
  (list '* (list '- a b) (list '+ a b)))

(define (diff-of-cubes exp)
  (define a (cadadr exp))
  (define b (car (cdaddr exp)))
  (list '* (list '- a b)
        (list '+ (list 'expt a 2) (list 'expt b 2) (list '* a b))))

(define (sum-of-cubes exp)
  (define a (cadadr exp))
  (define b (car (cdaddr exp)))
  (list '* (list '+ a b)
        (list '- (list '+ (list 'expt a 2) (list 'expt b 2)) (list '* a b))))

(define (factorize exp)
  (define sign (car exp))
  (define power (car (cddadr exp)))
  (cond
    ((and (equal? power 2) (equal? sign '-))  (diff-of-squares exp))
    ((and (equal? power 3) (equal? sign '-)) (diff-of-cubes exp))
    ((and (equal? power 3) (equal? sign '+)) (sum-of-cubes exp))))

; unit-tests



(define (make-tests samples)
  (if (null? samples)
      (list)
      (cons (list
             (factorize (car samples))
             (eval (car samples) (interaction-environment)))
            (make-tests (cdr samples)))))

(run-tests
 (make-tests
  (list
   '(- (expt 1 2) (expt 5 2))
   '(- (expt 4 2) (expt 6 2))
   '(- (expt 8 2) (expt 1 2))
   '(+ (expt 8 3) (expt 10 3))
   '(+ (expt 5 3) (expt -234 3))
   '(+ (expt 43 3) (expt -4 3))
   '(- (expt 124214 3) (expt 15 3))
   '(- (expt 234 3) (expt -13 3))
   '(- (expt 54 3) (expt 56 3))
   )))