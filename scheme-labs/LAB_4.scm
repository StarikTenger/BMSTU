(load "trace.scm")
(load "unit-test.scm")
(load "assert.scm")



;; === 1 : ASSERTIONS ==================================================

(use-assertions) ; Инициализация вашего каркаса перед использованием

; Определение процедуры, требующей верификации переданного ей значения:

(define (1/x x)
  (assert (not (zero? x))) ; Утверждение: x ДОЛЖЕН БЫТЬ > 0
  (/ 1 x))

; Применение процедуры с утверждением:

(map 1/x '(1 2 3 4 5)) ; ВЕРНЕТ список значений в программу

;(map 1/x '(-2 -1 0 1 2)) ; ВЫВЕДЕТ в консоль сообщение и завершит работу программы


;; === 2 : SERIALIZATION ===============================================

(define (save-data data path)
  (call-with-output-file path
    (lambda (output-port)
      (display data output-port))))

(define (load-data path)
  (let ((input (open-input-file path)))
    (read input)))

(define (calculate-strings-from-input-port input-port symbol-prev)
  (let ((symbol (read-char input-port)))
    (begin
      (+
       (if (and (equal? symbol-prev #\linefeed) (not (equal? symbol #\return)) (not (eof-object? symbol))) 1 0)
       (if (eof-object? symbol) 0 (calculate-strings-from-input-port input-port symbol))
       ))))

(define (count-strings path)
  (calculate-strings-from-input-port (open-input-file path) #\newline))

;; == 3 : MEMORIZATION =================================================
(define (tribonachi-seq-naive x)
  (cond
    ((< x 2) 0)
    ((equal? x 2) 1)
    (else (+ (tribonachi-seq-naive (- x 1)) (tribonachi-seq-naive (- x 2)) (tribonachi-seq-naive (- x 3))))))

(define (tribonachi-seq n)
  (let
      ((tribonachi-vector (make-vector (+ n 1) #f)))
       (letrec ((tribonachi-seq-mem (lambda (x)
                                   (if (vector-ref tribonachi-vector x)
                                       (vector-ref tribonachi-vector x)
                                       (begin
                                         (vector-set! tribonachi-vector x
                                                      (cond
                                                        ((< x 2) 0)
                                                        ((equal? x 2) 1)
                                                        (else (+
                                                               (tribonachi-seq-mem (- x 1))
                                                               (tribonachi-seq-mem (- x 2))
                                                               (tribonachi-seq-mem (- x 3))))))
                                         (tribonachi-seq-mem x))))))
         (tribonachi-seq-mem n))))

;; == 4 : MY IF ========================================================
(define (my-if-func condition a b)
  (and condition (set! b a))
  (force b))

(define-syntax my-if
  (syntax-rules ()
    ((_ condition a b)
     (my-if-func condition (delay a) (delay b)))))
  