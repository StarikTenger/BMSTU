(load "trace.scm")
(load "unit-test.scm")
(load "assert.scm")
(load "LAB_4.scm")

; == АЧИВКИ ==================================================+
; 1. Else (сдано вместе с лабораторной)                       |
; 2. Вложенный if (сдано вместе с лабораторной)               |
; 3. Циклы while (с предусловием) и until (с постусловием)    |
; 4. Break/continue                                           |
; 5. switch-case                                              |
; 6. lambda-функции, ковенный вызов (реализован командой do)  |
; 7. Хвостовая рекурсия (реализуется через оператор continue) |
; 8. Глобальные переменные                                    |
; ============================================================+

;; Some operations
(define (op= a b)
  (if (= a b) -1 0))
(define (op< a b)
  (if (< a b) -1 0))
(define (op> a b)
  (if (> a b) -1 0))
(define (true? a)
  (not (= a 0)))
(define (format-logic a)
  (if a -1 0))
(define (op-not a)
  (format-logic (not (true? a))))
(define (op-and a b)
  (format-logic (and (true? a) (true? b))))
(define (op-or a b)
  (format-logic (or (true? a) (true? b))))

(define (interpret program stack)
  (letrec (
           (return-stack '())
           (words '())
           (varaibles '())
           (skip (lambda (i balance beg end)
                   (if (or
                        (>= i (vector-length program))
                        (= balance 0))
                       (- i 1)
                       (begin
                         (if (equal? (vector-ref program i) end) (set! balance (- balance 1)))
                         (if (equal? (vector-ref program i) beg) (set! balance (+ balance 1)))
                         (skip (+ 1 i) balance beg end)))))
           (skip-if (lambda (i balance)
                      (if (or
                           (>= i (vector-length program))
                           (= balance 0))
                          (- i 1)
                          (begin
                            (if (equal? (vector-ref program i) 'endif) (set! balance (- balance 1)))
                            (if (and (= balance 1) (equal? (vector-ref program i) 'else)) (set! balance (- balance 1)))
                            (if (equal? (vector-ref program i) 'if) (set! balance (+ balance 1)))
                            (skip-if (+ 1 i) balance)))))
           (skip-switch (lambda (i case)
                          (if (>= i (vector-length program))
                              (- i 1)
                              (if (or (equal? (vector-ref program i) 'endswitch)
                                      (and (equal? (vector-ref program i) 'case)
                                           (equal? (vector-ref program (+ i 1)) case)))
                                  (if (equal? (vector-ref program i) 'endswitch)
                                      i
                                      (+ 1 i))
                                  (if (equal? (vector-ref program i) 'switch)
                                      ( + 1 (skip (+ 1 i) 1 'switch 'endswitch))
                                      (skip-switch (+ 1 i) case))))))
           (interpret-symbol (lambda (i)                                             
                               (if (< i (vector-length program))
                                   (let ((symbol (vector-ref program i)))
                                     ;(cout << i << " " << stack << " => " << symbol << endl)
                                     (cond
                                       ; Number
                                       ((number? symbol) (set! stack (cons symbol stack)))
                                       
                                       ; Arithmetic
                                       ((equal? symbol '+) (set! stack (cons (+ (cadr stack) (car stack)) (cddr stack))))
                                       ((equal? symbol '-) (set! stack (cons (- (cadr stack) (car stack)) (cddr stack))))
                                       ((equal? symbol '*) (set! stack (cons (* (cadr stack) (car stack)) (cddr stack))))
                                       ((equal? symbol '/) (set! stack (cons (/ (cadr stack) (car stack)) (cddr stack))))
                                       ((equal? symbol 'mod) (set! stack (cons (remainder (cadr stack) (car stack)) (cddr stack))))
                                       ((equal? symbol 'neg) (set! stack (cons (- (car stack)) (cdr stack))))
                                       
                                       ; Compare
                                       ((equal? symbol '=) (set! stack (cons (op= (cadr stack) (car stack)) (cddr stack))))
                                       ((equal? symbol '>) (set! stack (cons (op> (cadr stack) (car stack)) (cddr stack))))
                                       ((equal? symbol '<) (set! stack (cons (op< (cadr stack) (car stack)) (cddr stack))))
                                       
                                       ; Logic
                                       ((equal? symbol 'not) (set! stack (cons (op-not (car stack)) (cdr stack))))
                                       ((equal? symbol 'and) (set! stack (cons (op-and (cadr stack) (car stack)) (cddr stack))))
                                       ((equal? symbol 'or) (set! stack (cons (op-or (cadr stack) (car stack)) (cddr stack))))
                                       
                                       ; Stack operations
                                       ((equal? symbol 'drop) (set! stack (cdr stack)))
                                       ((equal? symbol 'swap) (set! stack (cons (cadr stack) (cons (car stack) (cddr stack)))))
                                       ((equal? symbol 'dup) (set! stack (cons (car stack) stack)))
                                       ((equal? symbol 'over) (set! stack (cons (cadr stack) stack)))
                                       ((equal? symbol 'rot) (set! stack (cons (caddr stack) (cons (cadr stack) (cons (car stack) (cdddr stack))))))
                                       ((equal? symbol 'depth) (set! stack (cons (length stack) stack)))
                                       
                                       ; If
                                       ((equal? symbol 'if)
                                        (begin
                                          (if (not (true? (car stack))) (set! i (skip-if (+ 1 i) 1)))
                                          (set! stack (cdr stack))))
                                       ((equal? symbol 'else) (set! i (skip-if (+ 1 i) 1)))
                                       
                                       ; Procedure
                                       ((equal? symbol 'define)
                                        (begin
                                          (set! i (+ 1 i))
                                          (set! words (cons (list (vector-ref program i) i) words))
                                          (set! i (skip (+ 1 i) 1 'define 'end))))
                                       ((assq symbol words)
                                        (begin
                                          (set! return-stack (cons i return-stack))
                                          (set! i (cadr (assq symbol words)))))
                                       ((or (equal? symbol 'end) (equal? symbol 'exit) (equal? symbol 'endlambda))
                                        (begin
                                          (set! i (car return-stack))
                                          (set! return-stack (cdr return-stack))
                                          (if (equal? symbol 'endlambda)
                                              (begin (set! i (car return-stack))
                                                     (set! return-stack (cdr return-stack))))))
                                       
                                       ; While
                                       ((equal? symbol 'while) (begin
                                                                 (if (not (true? (car stack)))
                                                                     (begin
                                                                       (set! i (+ 0 (skip (+ 1 i) 1 'while 'endwhile)))
                                                                       (set! stack (cdr stack)))
                                                                     (begin
                                                                       (set! return-stack (cons i return-stack))))))
                                       ((or (equal? symbol 'endwhile))
                                        (begin
                                          (set! i (- (car return-stack) 1))
                                          (set! return-stack (cdr return-stack))))

                                       ; Until
                                       ((equal? symbol 'until) (begin
                                                                 (if (not (true? (car stack)))
                                                                     (begin
                                                                       (set! return-stack
                                                                             (cons
                                                                              (+ 1 (skip (+ 1 i) 1 'until 'enduntil))
                                                                              return-stack)))
                                                                     (begin
                                                                       (set! return-stack (cons i return-stack))))))
                                       ((or (equal? symbol 'enduntil))
                                        (begin
                                          (if (equal? i (- (car return-stack) 1))
                                              (set! stack (cdr stack)))
                                          (set! i (- (car return-stack) 1))                                          
                                          (set! return-stack (cdr return-stack))))
                                       
                                       ; Continue
                                       ((or (equal? symbol 'continue))
                                        (begin
                                          (set! i (- (car return-stack) 1))
                                          (if (equal? (vector-ref program i) 'enduntil)
                                              (set! stack (cdr stack)))
                                          (if (not (equal? (vector-ref program i) 'lambda))
                                              (set! return-stack (cdr return-stack)))))

                                       ; Break
                                       ((or (equal? symbol 'break))
                                        (begin
                                          (set! stack (cons 0 (cdr stack)))
                                          (set! i (- (car return-stack) 1))
                                          (set! return-stack (cdr return-stack))))

                                       ; Switch
                                       ((equal? symbol 'switch)
                                        (begin
                                          (set! i (skip-switch (+ i 1) (car stack)))
                                          (set! stack (cdr stack))
                                          ))
                                       ((equal? symbol 'endcase)
                                        (set! i (skip (+ i 1) 1 'switch 'endswitch)))

                                       ; Varaibles
                                       ((equal? symbol 'var)
                                        (begin
                                          (set! i (+ 1 i))
                                          (set! varaibles (cons (list (vector-ref program i) (car stack)) varaibles))
                                          (set! stack (cdr stack))))
                                       ((assq symbol varaibles)
                                        (begin
                                          (set! stack (cons (cadr (assq symbol varaibles)) stack))))

                                       ; Lambda
                                       ((equal? symbol 'lambda)
                                        (begin
                                          (set! stack (cons (list (+ i 0)) stack))
                                          (set! i (skip (+ 1 i) 1 'lambda 'endlambda))))
                                       ((equal? symbol 'do)
                                        (begin
                                          (set! return-stack (cons i return-stack))
                                          (set! i (caar stack))
                                          (set! return-stack (cons (+ i 1) return-stack))
                                          (set! stack (cdr stack))
                                          ))
                                       )
                                     (interpret-symbol (+ 1 i)))))))
    (begin
      (interpret-symbol 0)
      stack)))


;; Unit-testing
(define tests (list
               ; Простейшие тесты
               (test (interpret #(2 3 * 4 5 * +) '()) '(26))
               (test (interpret #(neg) '(1)) '(-1))
               (test (interpret #(10 2 /) '()) '(5))
               (test (interpret #(10 2 >) '()) '(-1))
               (test (interpret #(10 2 <) '()) '(0))
               (test (interpret #(10 5 + 15 =) '()) '(-1))
               (test (interpret #(1 1 and) '()) '(-1))
               (test (interpret #(1 0 and) '()) '(0))
               (test (interpret #(1 0 or) '()) '(-1))
               (test (interpret #(1 2 3 drop) '()) '(2 1))
               (test (interpret #(1 2 3 4 5 swap) '()) '(4 5 3 2 1))
               (test (interpret #(1 2 3 4 5 dup) '()) '(5 5 4 3 2 1))
               (test (interpret #(1 2 3 4 5 over) '()) '(4 5 4 3 2 1))
               (test (interpret #(1 2 3 4 5 rot) '()) '(3 4 5 2 1))
               (test (interpret #(depth 0 0 0 depth 0 0 0 depth) '()) '(8 0 0 0 4 0 0 0 0))
               (test (interpret #(1 dup if dup dup if 5 endif endif 3) '()) '(3 5 1 1))
               (test (interpret #(0 dup if dup dup if 5 endif endif 3) '()) '(3 0))
               (test (interpret #(define test 1 2 3 end 0 test 0) '()) '(0 3 2 1 0))
               ; Несколько тестов из условия
               (test (interpret #(   define abs 
                                      dup 0 < 
                                      if neg endif 
                                      end 
                                      abs    ) ; программа
                                '(-9)) '(9))
               (test (interpret #(   define abs 
                                      dup 0 < 
                                      if neg endif 
                                      end 
                                      9 abs 
                                      -9 abs      ) (quote ())) '(9 9))
               (test (interpret #(   define =0? dup 0 = end 
                                      define <0? dup 0 < end 
                                      define signum 
                                      =0? if exit endif 
                                      <0? if drop -1 exit endif 
                                      drop 
                                      1 
                                      end 
                                      0 signum 
                                      -5 signum 
                                      10 signum       ) (quote ())) '(1 -1 0))
               (test (interpret #(   define -- 1 - end 
                                      define =0? dup 0 = end 
                                      define =1? dup 1 = end 
                                      define factorial 
                                      =0? if drop 1 exit endif 
                                      =1? if drop 1 exit endif 
                                      dup -- 
                                      factorial 
                                      * 
                                      end 
                                      0 factorial 
                                      1 factorial 
                                      2 factorial 
                                      3 factorial 
                                      4 factorial     ) (quote ())) '(24 6 2 1 1))
               (test (interpret #(   define =0? dup 0 = end 
                                      define =1? dup 1 = end 
                                      define -- 1 - end 
                                      define fib 
                                      =0? if drop 0 exit endif 
                                      =1? if drop 1 exit endif 
                                      -- dup 
                                      -- fib 
                                      swap fib 
                                      + 
                                      end 
                                      define make-fib 
                                      dup 0 < if drop exit endif 
                                      dup fib 
                                      swap -- 
                                      make-fib 
                                      end 
                                      10 make-fib     ) (quote ())) '(0 1 1 2 3 5 8 13 21 34 55))
               (test (interpret #(   define =0? dup 0 = end 
                                      define gcd 
                                      =0? if drop exit endif 
                                      swap over mod 
                                      gcd 
                                      end 
                                      90 99 gcd 
                                      234 8100 gcd    ) '()) '(18 9))
               ; if-else
               (test (interpret #(1 if 10 else -10 endif) '()) '(10))
               (test (interpret #(0 if 10 else -10 endif) '()) '(-10))
               ; Циклы
               (test (interpret #(5 while 1 swap 1 - endwhile) '()) '(1 1 1 1 1))
               (test (interpret #(define fact
                                   1 1 rot
                                   1 -
                                   while
                                   rot 1 + dup rot * swap rot 1 -
                                   endwhile
                                   swap drop
                                   end
                                   3 fact
                                   5 fact
                                   7 fact
                                   1 fact) '())
                     '(1 5040 120 6))
               (test (interpret #(3 until 1 swap 1 - enduntil) '()) '(1 1 1 1))
               ; Вложенные циклы
               (test (interpret #(0 3
                                    while
                                    swap 5
                                    while swap 1 + swap 1 -
                                    endwhile
                                    swap 1 -
                                    endwhile)
                                '()) '(15))
               ; Операторы break/continue
               (test (interpret #(0 1 while swap 1 + swap dup 10 = if break endif 1 + endwhile) '()) '(10))
               (test (interpret #(2 10 while 1 - dup 2 mod if continue endif swap 2 * swap endwhile) '()) '(64))
               (test (interpret #(2 10 until 1 - dup 2 mod if continue endif swap 2 * swap enduntil) '()) '(64))
               (test (interpret #(2 8 while 1 - dup 2 mod not if continue endif swap 2 * swap endwhile) '()) '(32))
               (test (interpret #(2 8 until 1 - dup 2 mod not if continue endif swap 2 * swap enduntil) '()) '(64))
               ; Конструкция switch-case
               (test (interpret #(1 switch case 1 11 endcase case 2 22 endcase case 3 33 endcase endswitch) '()) '(11))
               (test (interpret #(2 switch case 1 11 endcase case 2 22 endcase case 3 33 endcase endswitch) '()) '(22))
               (test (interpret #(3 switch case 1 11 endcase case 2 22 endcase case 3 33 endcase endswitch) '()) '(33))
               ; Вложенный switch
               (test (interpret #(dup 3 mod
                                      switch
                                      case 0 0 endcase
                                      case 1 1 endcase
                                      case 2
                                      switch
                                      case 2 0 endcase
                                      endswitch
                                      endswitch
                                      ) '(17)) '())
               (test (interpret #(dup 3 mod
                                      switch
                                      case 0 0 endcase
                                      case 1 1 endcase
                                      case 2
                                      switch
                                      case 2 0 endcase
                                      endswitch
                                      endswitch
                                      ) '(2)) '(0))
               ; Глобальные переменные
               (test (interpret #(3 var a 4 var b a b *) '()) '(12))
               (test (interpret #(1 var prod
                                    1 var i
                                    5 var max
                                    define check i max > not end
                                    check while drop
                                    prod i * var prod
                                    i 1 + var i
                                    check
                                    endwhile
                                    prod) '()) '(120))
               ; Демонстрация хвостовой рекурсии
               (test (interpret #(define f dup if drop f endif end f) '(1 1 1 1 0 1  1 1 0)) '(0 1 1 1 0))
               (test (interpret #(define f dup if drop continue endif end f) '(1 1 1 1 0 1  1 1 0)) '(0 1 1 1 0))
               (test (interpret #(define fact
                1 1
                lambda
                rot 1 -
                dup
                if
                rot
                swap 1 +
                dup rot *
                continue
                endif
                endlambda
                do
                drop drop
                end
                5 fact
                6 fact
                8 fact) '()) '(40320 720 120))
               ; Lambda-функции
               (test (interpret #(lambda 1 endlambda do) '()) '(1))
               (test (interpret #(define f lambda 1 endlambda end f f do swap do) '()) '(1 1))
               ))

(run-tests tests)