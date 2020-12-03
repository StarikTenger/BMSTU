(load "trace.scm")
(load "unit-test.scm")
(load "assert.scm")

;; == 1 : MEMORIZED FACTORIAL ==========================================

(define (gen-factorial)
  (letrec ((map '((1 1)))
           (fact (lambda (x)
                   (if (assq x map)
                       (* x (cadr (assq x map)))
                       (let ((val (* x (fact (- x 1)))))
                         (set! map (cons (list x val) map))
                         val)))))
    fact))

(define memoized-factorial (gen-factorial))

;; == 2 : DELAY ========================================================

;; Pairs

(define-syntax lazy-cons
  (syntax-rules ()
    ((_ a b)
     (cons a (delay b)))))

(define (lazy-car lazy-pair)
  (car lazy-pair))

(define (lazy-cdr lazy-pair)
  (force (cdr lazy-pair)))

;; Lists

(define (lazy-head xs k)
  (if (and (> k 0) (pair? xs))
                  (cons
                   (lazy-car xs)
                   (lazy-head (lazy-cdr xs) (- k 1)))
                  '()))

(define (lazy-ref xs k)
  (if (and (> k 0) (pair? xs))
      (lazy-ref (lazy-cdr xs) (- k 1))
      (lazy-car xs)))

;; Generators

(define (naturals start)
  (lazy-cons start (naturals (+ start 1))))

(define (factorials n value)
  (lazy-cons value (factorials (+ n 1) (* value (+ n 1)))))

(define (lazy-factorial n)
  (lazy-ref (factorials 0 1) n))

