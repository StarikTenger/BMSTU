(load "trace.scm")
(load "unit-test.scm")

(define (der expr x)
  (cond
    ((number? expr) 0)
    ((equal? expr x) 1)
    ((list? expr)
     (cond
       ((equal? (car expr) '+)
        (list '+ (der (cadr expr) x) (der (caddr expr) x)))
       ((equal? (car expr) '-)
        (list '- (der (cadr expr) x) (der (caddr expr) x)))
       ((equal? (car expr) '*)
        (list '+
              (list '* (der (cadr expr) x) (caddr expr))
              (list '* (cadr expr) (der (caddr expr) x))
              ))
       ((equal? (car expr) 'expt)
        (list '+
              (list '* (list '* (caddr expr) (list 'expt (cadr expr) (list '- (caddr expr) 1)))
                    (der (cadr expr) x))
              (list '* (list '* (list 'expt (cadr expr) (caddr expr)) (list 'log (cadr expr)))
                    (der (caddr expr) x))))
       (else 0)
       ))
    (else 0)))

(define (collapse expr)
  (if (list? expr)
      (if (= (length expr) 2)
          (if (number? (cadr expr))
              (eval expr (interaction-environment))
              (list (car expr) (collapse (cadr expr))))
          (if (and (number? (cadr expr)) (number? (caddr expr)))
              (eval expr (interaction-environment))
              (let ((a (collapse (cadr expr))) (b (collapse (caddr expr))))
                (cond
                  ((and (equal? (car expr) '*) (or (equal? a 0) (equal? b 0))) 0)
                  ((and (equal? (car expr) '*) (equal? a 1)) b)
                  ((and (equal? (car expr) '*) (equal? b 1)) a)
                  ((and (equal? (car expr) '+) (equal? a 0)) b)
                  ((and (equal? (car expr) '+) (equal? b 0)) a)
                  ((and (equal? (car expr) '-) (equal? a 0)) b)
                  ((and (equal? (car expr) '-) (equal? b 0)) a)
                  (else (list (car expr) a b)))
                )))
      expr))

(define (can-mult? expr)
  (and
   (list? expr)
   (or
    (equal? (car expr) '+)
    (equal? (car expr) '-)
    (equal? (car expr) '*)
    (equal? (car expr) '/)
    )))

(define (mult expr x)
  ( if (list? expr)
       (let ((op (car expr)))
         (cond
           ((equal? op '*)
            (let ((a (cadr expr)) (b (caddr expr)))
              (if (number? a)
                  (trace-ex (list op (* a x) b))
                  (if (number? b)
                      (trace-ex (mult (list op b a) x))
                      expr))))
           ((or (equal? op '+) (equal? op '-))
            (let ((a (cadr expr)) (b (caddr expr)))             
              (list op (mult a x) (mult b x))))
           (else (begin (trace-ex op) (trace-ex expr)))))
       (* expr x)))

(define (expand expr)
  (if (and (list? expr )(equal? (car expr) '*))
      (let ((a (cadr expr)) (b (caddr expr)))
        (cond
          ((and (number? a) (list? b) (can-mult? b)) (trace-ex (mult (expand b) a)))
          ((and (number? b) (list? a) (can-mult? a)) (trace-ex (mult (expand a) b)))
          (else expr)
          ))
      expr))

(define (derivative expr x)
  (expand (collapse (der expr x))))

;; Unit tests
(define tests (list
               (test (derivative '2 'x) 0)
               (test (derivative 'x 'x) 1)
               (test (derivative '(- 0 x) 'x) -1)
               (test (derivative '(* 1 x) 'x) 1)
               (test (derivative '(* -1 x) 'x) -1)
               (test (derivative '(* -4 x) 'x) -4)
               (test (derivative '(* 10 x) 'x) 10)
               (test (derivative '(- (* 2 x) 3) 'x) 2)
               (test (derivative '(expt x 10) 'x) '(* 10 (expt x 9)))
               (test (derivative '(* 2 (expt x 5)) 'x) '(* 10 (expt x 4)))
               ))

(run-tests tests)