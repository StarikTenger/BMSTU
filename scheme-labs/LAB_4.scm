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