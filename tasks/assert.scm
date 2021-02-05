(define-syntax call/cc
  (syntax-rules ()
    ((_ x)
     (call-with-current-continuation x))))

;; Break continuation
(define break #f)

(begin
  (call/cc (lambda (continuation) (set! break continuation)))
  #t)

(define enable-assertion #f)
(define (use-assertions)
  (set! enable-assertion #t))

(define (assert-func condition message)
  (if (and enable-assertion (not condition))
      (begin
        (display "FAILED: ")
        (display message)
        (break))))

(define-syntax assert
  (syntax-rules ()
    ((_ x)
     (assert-func x (quote x)))))

