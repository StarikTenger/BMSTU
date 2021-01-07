(load "trace.scm")
(load "unit-test.scm")
(load "assert.scm")
(load "LAB_4.scm")
(load "HW_4.scm")

;; Stream
(define finish-symbol #\⛔)
(define-struct stream (symbols))
(define (stream-peek stream)
  (and
   (pair? (stream-symbols stream))
   (let ((symbol (car (stream-symbols stream))))
     symbol)))
(define (stream-next stream)
  (let ((symbol (car (stream-symbols stream))))
    (set-stream-symbols! stream (cdr (stream-symbols stream)))
    symbol))
(define (string->stream str)
  (make-stream (string->list (string-append str (string finish-symbol)))))

; <Token Seq> ::= <Token> <Token Seq> | finish-symbol
; <Token> ::= <Bracket> | <Operation> | <Number> | <Varaible>
; <Bracket> ::= ( | )
; <Operation> ::= + | - | * | /
; <Number> ::= <Digit> | <Digit> <Number>
; <Digit> ::= 0 | 1 | 2 ...
; <Varaible> ::= <Letter> | <Varaible> <Letter>
; <Letter> ::= a | b | c ...
; <Barrier> ::= <> | <Barrier Symbol> | <Barrier Symbol> <Barrier>
; <Barrier Symbol> ::= space | tab | newline

(define (tokenize str)
  (letrec ((stream (string->stream str))
           ; <Token Seq> ::= <Barrier> <Token> <Barrier> <Token Seq> | finish-symbol
           (scan-token-seq (lambda ()
                             (let* ((fin-symb (scan-finish-symbol))
                                    (token (trace-ex (scan-token)))
                                    (barrier1 (scan-barrier))
                                    (token-seq (and token (scan-token-seq)))
                                    (barrier2 (scan-barrier)))
                             (if fin-symb
                                 '()
                                 (and token (cons token token-seq))))))
           ; <Token> ::= <Bracket> | <Operation> | <Number> | <Varaible>
           (scan-token (lambda ()
                         (let* ((bracket  (scan-bracket))
                               (operation (and (not bracket) (trace-ex (scan-operation))))
                               (number    (and (not operation) (not bracket) (scan-number)))
                               (varaible  (and (not bracket) (not operation) (not number)  (trace-ex (scan-varaible)))))
                           (or
                            bracket
                            (and operation (string->symbol (string operation)))
                            (and number (string->number number))
                            (and varaible (string->symbol varaible))))))
           ; <Bracket> ::= ( | )
           (scan-bracket (lambda ()
                           (let ((symb (stream-peek stream)))
                             (and
                              (or (equal? symb #\() (equal? symb #\)))
                              (begin (stream-next stream) (string symb))))))
           ; <Operation> ::= + | - | * | /
           (scan-operation (lambda ()
                             (let ((symb (stream-peek stream)))
                               (and
                                (or (equal? symb #\+) (equal? symb #\-) (equal? symb #\*) (equal? symb #\/) (equal? symb #\^))
                                (begin (stream-next stream) symb)))))
           ; <Number> ::= <Digit> | <Digit> <Number>
           (scan-number (lambda ()
                          (let* ((digit (scan-digit))
                                 (number (and digit (scan-number))))
                            (if number
                                (string-append (string digit) number)
                                (and digit (string digit))))))
           ; <Digit> ::= 0 | 1 | 2 ...
           (scan-digit (lambda ()
                         (let ((symb (stream-peek stream)))
                           (and (assq symb (map (lambda (x) (list x)) (string->list "0123456789")))
                                (begin (stream-next stream) symb)))))
           ; <Varaible> ::= <Letter> | <Varaible> <Letter>
           (scan-varaible (lambda ()
                            (let* ((letter (scan-letter))
                                   (varaible (and letter (scan-varaible))))
                              (if varaible
                                  (string-append (string letter) varaible)
                                  (and letter (string letter))))))
           ; <Letter> ::= a | b | c ...
           (scan-letter (lambda ()
                          (let ((symb (stream-peek stream)))
                            (and (assq symb (map (lambda (x) (list x)) (string->list "qwertyuiopasdfghjklzxcvbnm")))
                                 (begin (stream-next stream) symb)))))
           ; <Barrier> ::= <> | <Barrier Symbol> | <Barrier Symbol> <Barrier>
           (scan-barrier (lambda ()
                           (let* ((barrier-symbol (scan-barrier-symbol))
                                  (barrier (and barrier-symbol (scan-barrier))))
                             (if barrier-symbol
                                 (string-append (string barrier-symbol) barrier)
                                 (if barrier-symbol
                                     (string barrier-symbol)
                                     "")))))
           ; <Barrier Symbol> ::= space | tab | newline
           (scan-barrier-symbol (lambda ()
                                  (let ((symb (stream-peek stream)))
                                    (and
                                     (or (equal? symb #\space) (equal? symb #\tab) (equal? symb #\newline))
                                     (begin (stream-next stream) symb)))))
           ; Finish Symbol
           (scan-finish-symbol (lambda ()
                                 (let ((symb (stream-peek stream)))
                                   (and
                                    (or (equal? symb finish-symbol))
                                    (begin (string symb))))))
           )
    (scan-token-seq)))

; Unit tests

(define tests (list
               (test (tokenize "1") '(1))
               (test (tokenize "-a") '(- a))
               (test (tokenize "-a + b * x^2 + dy") '(- a + b * x ^ 2 + dy))
               (test (tokenize "(a - 1)/(b + 1)") '("(" a - 1 ")" / "(" b + 1 ")"))
               ))

(run-tests tests)