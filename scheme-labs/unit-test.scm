(define (run-test-r val var exp-val)
  (display var)
  (display " ")
  (if (equal? val exp-val)
      (begin
        (display "ok\n"))
      (begin
        (display "FAIL\n")
        (display "  Expected: ")
        (display exp-val)
        (display "\n  Returned: ")
        (display val)
        (display "\n")))
  (equal? val exp-val)
  )

(define-syntax run-test
  (syntax-rules ()
    ((_ exp1 exp2 ...)
     (run-test-r exp1 (quote exp1) exp2 ...))))

(define-syntax test
  (syntax-rules ()
    ((_ exp1 exp2 ...)
     (list (quote exp1) exp2 ...))))

(define (run-tests tests)
  (or (not (pair? tests))
      (and
       (run-test-r
        (eval (car (car tests)) (interaction-environment))
        (car (car tests))
        (cadr (car tests))
        )
       (run-tests (cdr tests)))
      ))