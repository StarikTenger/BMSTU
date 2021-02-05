(define (trace val var)
  (display var)
  (display " => ")
  (display val)
  (display "\n")
  val
  )

(define-syntax trace-ex
  (syntax-rules ()
    ((_ exp2 ...)
     (trace exp2 ... (quote exp2 ...)))))