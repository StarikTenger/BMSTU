(load "trace.scm")
(load "unit-test.scm")

(define (der expr x)
  (cond
    ((number? expr) 0)
    ((equal? expr x) 1)
    ((list? expr)
     (cond
       ; arithmetic
       ; +
       ((equal? (car expr) '+)
        (list '+ (der (cadr expr) x) (der (caddr expr) x)))
       ; - 
       ((equal? (car expr) '-)
        (list '- (der (cadr expr) x) (der (caddr expr) x)))
       ; *
       ((equal? (car expr) '*)
        (list '+
              (list '* (der (cadr expr) x) (caddr expr))
              (list '* (cadr expr) (der (caddr expr) x))
              ))
       ; /
       ((equal? (car expr) '/)
        (list '/ 
              (list '-
                    (list '* (der (cadr expr) x) (caddr expr))
                    (list '* (cadr expr) (der (caddr expr) x))
                    )
              (list 'expt (caddr expr) 2)))
       ; exponent
       ((equal? (car expr) 'expt)
        (list '+
              (list '* (list '* (caddr expr) (list 'expt (cadr expr) (list '- (caddr expr) 1)))
                    (der (cadr expr) x))
              (list '* (list '* (list 'expt (cadr expr) (caddr expr)) (list 'log (cadr expr)))
                    (der (caddr expr) x))))
       ; trigonometric
       ((equal? (car expr) 'sin)
        (list '* (list 'cos (cadr expr)) (der (cadr expr) x)))
       ((equal? (car expr) 'cos)
        (list '* (list '- 0 (list 'sin (cadr expr))) (der (cadr expr) x)))
       ; log
       ((equal? (car expr) 'log)
        (list '/ (der (cadr expr) x) (cadr expr)))
       (else 0)
       ))
    (else 0)))

;; Some simplifying functions
(define (collapse expr)
  (if (list? expr)
      (if (= (length expr) 2)
          (if (and (number? (cadr expr)) (not (equal? (car expr) 'log)))
              (eval expr (interaction-environment))
              (let ((a (collapse (cadr expr))))
                (cond
                  ((and (equal? (car expr) 'log) (equal? a 'e)) 1)
                  (else (list (car expr) (collapse (cadr expr)))))
                ))
          (if (and (number? (cadr expr)) (number? (caddr expr)))
              (eval expr (interaction-environment))
              (let ((a (collapse (cadr expr))) (b (collapse (caddr expr))))
                (cond
                  ((and (equal? (car expr) '*) (or (equal? a 0) (equal? b 0))) 0)
                  ((and (equal? (car expr) '*) (equal? a 1)) b)
                  ((and (equal? (car expr) '*) (equal? b 1)) a)
                  ((and (equal? (car expr) '+) (equal? a 0)) b)
                  ((and (equal? (car expr) '+) (equal? b 0)) a)
                  ((and (equal? (car expr) '-) (equal? a 0)) (list '- b))
                  ((and (equal? (car expr) '-) (equal? b 0)) a)
                  ((and (equal? (car expr) 'expt) (equal? b 1)) a)
                  (else (list (car expr) a b)))
                )))
      expr))


; can expression be multiplied?
(define (can-mult? expr)
  (and
   (list? expr)
   (or
    (equal? (car expr) '+)
    (equal? (car expr) '-)
    (equal? (car expr) '*)
    (equal? (car expr) '/)
    )))

; multiply expression by number
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
           ((equal? op '/)
            (let ((a (cadr expr)) (b (caddr expr)))
              (if (number? a)
                  (trace-ex (list op (* a x) b))
                  expr)))
           ((or (equal? op '+) (equal? op '-))
            (let ((a (cadr expr)) (b (caddr expr)))             
              (list op (mult a x) (mult b x))))
           (else (begin (trace-ex op) (trace-ex expr)))))
       (* expr x)))

; expt expression by number
(define (expand-expt expr x)
  ( if (list? expr)
       (let ((op (car expr)))
         (cond
           ((equal? op 'expt)
            (let ((a (cadr expr)) (b (caddr expr)))
              (if (number? a)
                  (trace-ex (list op (expt a x) b))
                  (if (number? b)
                      (trace-ex (list 'expt a (* b x)))
                      expr))))
           ((or (equal? op '*) (equal? op '/))
            (let ((a (cadr expr)) (b (caddr expr)))             
              (list op (expand-expt a x) (expand-expt b x))))
           (else (begin (trace-ex op) (trace-ex expr)))))
       (expt expr x)))

; expand brackets with multiplication
(define (expand expr)
  (if (list? expr)
      (cond
        ((equal? (car expr) '*)
         (let ((a (cadr expr)) (b (caddr expr)))
           (cond
             ((and (number? a) (list? b) (can-mult? b)) (trace-ex (mult (expand b) a)))
             ((and (number? b) (list? a) (can-mult? a)) (trace-ex (mult (expand a) b)))
             (else expr)
             )))
        ((equal? (car expr) 'expt)
         (let ((a (cadr expr)) (b (caddr expr)))
           (cond
             ((and (number? a) (list? b) (can-mult? b)) (trace-ex (expand-expt (expand b) a)))
             ((and (number? b) (list? a) (can-mult? a)) (trace-ex (expand-expt (expand a) b)))
             (else expr)
             )))
        (else (if (= (length expr) 3)
                  (list (car expr) (expand (cadr expr)) (expand (caddr expr)))
                  (list (car expr) (expand (cadr expr))))))
      expr))


;; minus simplifying
(define (count-minus expr)
  (if (list? expr)
      (if (= (length expr) 2)
          (if (equal? (car expr) '-)
              (+ (count-minus (cadr expr)) 1)
              0)
          (cond
            ((or (equal? (car expr) '*) (equal? (car expr) '/))
             (+ (count-minus (cadr expr)) (count-minus (caddr expr))))
            (else 0)
            ))
      0))

(define (need-minus? expr)
  (= (remainder (count-minus expr) 2) 1))

; erases all posiible minuses from expression
(define (erase-minus expr)
  (if (list? expr)
      (if (= (trace-ex (length expr)) 2)
          (if (equal? (car expr) '-)
              (erase-minus (cadr expr))
              expr)
          (let ((op (car expr)))
            (cond
              ((or (equal? (trace-ex op) '*) (equal? op '/))
               (list op (erase-minus (cadr expr)) (erase-minus (caddr expr))))
              (else expr))))
      expr))

; simplify minus once
(define (simplify-minus expr)
  (if (need-minus? expr)
      (list '- (erase-minus expr))
      (erase-minus expr)))

; simplify minus recursevely
(define (simplify-minus-rec expr)
  (if (list? expr)
      (if (= (length expr) 2)
          (simplify-minus expr)
          (cond
            ((not (or (equal? (trace-ex (car expr)) '*) (equal? (car expr) '/)))
             (list (car expr) (simplify-minus (cadr expr)) (simplify-minus (caddr expr))))
            (else (simplify-minus expr))))
      (simplify-minus expr)))

;;; CONVOLUTION
(define (convolution expr op)
  (if (pair? expr)
      (if (pair? (cdr expr))
      (list op (car expr) (convolution (cdr expr) op))
      (car expr))
      expr))


;; final derivative function
(define (derivative expr x)
  (simplify-minus-rec (expand (collapse (trace-ex (der expr x))))))

;; Unit tests
(define tests (list
               (test (derivative '2 'x) 0) ; 1"
               (test (derivative 'x 'x) 1) ; 2
               (test (derivative '(- 0 x) 'x) -1) ; 3
               (test (derivative '(* 1 x) 'x) 1) ; 4
               (test (derivative '(* -1 x) 'x) -1) ; 5
               (test (derivative '(* -4 x) 'x) -4) ; 6
               (test (derivative '(* 10 x) 'x) 10) ; 7
               (test (derivative '(- (* 2 x) 3) 'x) 2) ; 8
               (test (derivative '(expt x 10) 'x) '(* 10 (expt x 9))) ; 9
               (test (derivative '(* 2 (expt x 5)) 'x) '(* 10 (expt x 4))) ; 10
               (test (derivative '(expt x -2) 'x) '(* -2 (expt x -3))) ; 11
               (test (derivative '(expt 5 x) 'x) '(* (expt 5 x) (log 5))) ; 12
               (test (derivative '(cos x) 'x) '(- (sin x))) ; 13
               (test (derivative '(sin x) 'x) '(cos x)) ; 14
               (test (derivative '(expt e x) 'x) '(expt e x)) ; 15
               (test (derivative '(* 2 (expt e x)) 'x) '(* 2 (expt e x))) ; 16
               (test (derivative '(* 2 (expt e (* 2 x))) 'x) '(* 4 (expt e (* 2 x)))) ; 17
               (test (derivative '(log x) 'x) '(/ 1 x)) ; 18
               (test (derivative '(* 3 (log x)) 'x) '(/ 3 x)) ; 19
               (test (derivative '(+ (expt x 3) (expt x 2)) 'x) '(+ (* 3 (expt x 2)) (* 2 x))) ; 20
               (test (derivative '(- (* 2 (expt x 3)) ( * 2 (expt x 2))) 'x) '(- (* 6 (expt x 2)) (* 4 x))) ; 21
               (test (derivative '(/ 3 x) 'x) '(- (/ 3 (expt x 2)))) ; 22
               (test (derivative '(/ 3 (* 2 (expt x 2))) 'x) '(/ (- 3) (expt x 3))) ; 23
               (test (derivative '(* 2 (* (sin x) (cos x))) 'x) '(+ (* (cos x) (cos x)) (- (* (sin x) (sin x))))) ; 24
               ))

(run-tests tests)