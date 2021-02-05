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
(define (list->stream lst)
  (make-stream (append lst (list finish-symbol))))

;; == 1 : TOKENIZE =================================================

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
                                    (token (scan-token))
                                    (barrier1 (scan-barrier))
                                    (token-seq (and token (scan-token-seq)))
                                    (barrier2 (scan-barrier)))
                               (if fin-symb
                                   '()
                                   (and token (cons token token-seq))))))
           ; <Token> ::= <Bracket> | <Operation> | <Number> | <Varaible>
           (scan-token (lambda ()
                         (let* ((bracket  (scan-bracket))
                                (operation (and (not bracket) (scan-operation)))
                                (number    (and (not operation) (not bracket) (scan-number)))
                                (varaible  (and (not bracket) (not operation) (not number)  (scan-varaible))))
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

;; == 2 : PARSE =====================================================
;; == 3 : PARSE =====================================================

; Expr    ::= Term Expr' .
; Expr'   ::= AddOp Term Expr' | .
; Term    ::= Factor Term' .
; Term'   ::= MulOp Factor Term' | .
; Factor  ::= Power Factor' .
; Factor' ::= PowOp Power Factor' | .
; Power   ::= value | "(" Expr ")" | unaryMinus Power .

(define (parse str)
  (letrec ((stream (list->stream str))
           ; FinishedExpr ::= Expr finish-symbol
           (scan-finished-expr (lambda ()
                                 (let* ((expr (scan-expr))
                                        (finish-symbol (scan-finish-symbol)))
                                   (and expr finish-symbol
                                        expr))))
           ; finish-symbol
           (scan-finish-symbol (lambda ()
                                 (let ((symb (stream-peek stream)))
                                   (and
                                    (or (equal? symb finish-symbol))
                                    (begin (string symb))))))
           ; Expr    ::= Term Expr' .
           (scan-expr (lambda ()
                        (let* ((term (trace-ex (scan-term)))
                               (expr- (and term (scan-expr- term))))
                          (and term expr-))))
           ; Expr'   ::= AddOp Term Expr' | .
           (scan-expr- (lambda (term-prev)
                         (let* ((add-op (trace-ex (scan-add-op)))
                                (term (and add-op (trace-ex (scan-term))))
                                (expr- (and term (scan-expr- (list term-prev add-op term)))))
                           (if (and add-op term)
                               expr-
                               term-prev))))
           ; AddOp
           (scan-add-op (lambda ()
                          (let ((op (stream-peek stream)))
                            (and (or (equal? op '+) (equal? op '-))
                                 (begin (stream-next stream) op)))))
           ; Term    ::= Factor Term' .
           (scan-term (lambda ()
                        (let* ((factor (scan-factor))
                               (term- (and factor (scan-term- factor))))
                          (and factor term-))))
           ; Term'   ::= MulOp Factor Term' | .
           (scan-term- (lambda (factor-prev)
                         (let* ((mul-op (scan-mul-op))
                                (factor (and mul-op (trace-ex (scan-factor))))
                                (term- (and factor (scan-term- (list factor-prev mul-op factor)))))
                           (if (and mul-op factor term-)
                               term-
                               factor-prev))))
           ; MulOp
           (scan-mul-op (lambda ()
                          (let ((op (stream-peek stream)))
                            (and (or (equal? op '*) (equal? op '/))
                                 (begin (stream-next stream) op)))))
           ; Factor  ::= Power Factor' .
           (scan-factor (lambda ()
                          (let* ((power (trace-ex (scan-power)))
                                 (factor- (and power (trace-ex (scan-factor-)))))
                            (and power factor-
                                 (if (list? power)
                                     (append power factor-)
                                     (if (equal? factor- '())
                                         power
                                         (cons power factor-)))))))
           ; Factor' ::= PowOp Power Factor' | .
           (scan-factor- (lambda ()
                           (let* ((pow-op (scan-pow-op))
                                  (power (and pow-op (scan-power)))
                                  (factor- (and power (scan-factor-))))
                             (if (and pow-op power factor-)
                                 (if (equal? factor- '())
                                     (list pow-op power)
                                     (list pow-op (cons power factor-)))
                                 '()))))
           ; PowOp
           (scan-pow-op (lambda ()
                          (let ((op (stream-peek stream)))
                            (and (equal? op '^)
                                 (begin (stream-next stream) op)))))
           ; Power   ::= value | "(" Expr ")" | unaryMinus Power .
           (scan-power (lambda ()
                         (let* ( ; value
                                (value (trace-ex (scan-value)))
                                ; "(" Expr ")"
                                (open-bracket (scan-open-bracket))
                                (expr (and open-bracket (scan-expr)))
                                (close-bracket (and expr (scan-close-bracket)))
                                ; unaryMinus Power .
                                (unary-minus (and (not value) (scan-unary-minus)))
                                (power (and unary-minus (scan-power))))
                           (or
                            value
                            (and close-bracket expr)
                            (and power (list unary-minus power))))))
           ; Value
           (scan-value (lambda ()
                         (let ((val (stream-peek stream)))
                           (or
                            (and (symbol? val) (not (equal? val '-))
                                 (begin (stream-next stream) val))
                            (and (number? val)
                                 (begin (stream-next stream) val))))))
           ; OpenBracket
           (scan-open-bracket (lambda ()
                                (let ((op (stream-peek stream)))
                                  (and (equal? op "(")
                                       (begin (stream-next stream) op)))))
           ; CloseBracket
           (scan-close-bracket (lambda ()
                                 (let ((op (stream-peek stream)))
                                   (and (equal? op ")")
                                        (begin (stream-next stream) op)))))
           ; UnaryMinus
           (scan-unary-minus (lambda ()
                               (let ((op (stream-peek stream)))
                                 (and (equal? op '-)
                                      (begin (stream-next stream) op)))))
           )
    (scan-finished-expr)))

; Unit tests

(define tests (list
               (test (parse (tokenize "(a - 1)/(b + 1)")) '((a - 1) / (b + 1)))
               (test (parse (tokenize "a/b/c/d")) '(((a / b) / c) / d))
               (test (parse (tokenize "a^b^c^d")) '(a ^ (b ^ (c ^ d))))
               (test (parse (tokenize "a/(b/c)")) '(a / (b / c)))
               (test (parse (tokenize "a + b/c^2 - d")) '((a + (b / (c ^ 2))) - d))
               (test (parse (tokenize "a ^ -b")) '(a ^ (- b)))))

(run-tests tests)

;; == 3 : TREE TO SCHEME ===========================================

(define (tree->scheme lst)
  (if (list? lst)
      (let* ((a (tree->scheme (car lst)))
             (b (tree->scheme (cadr lst)))
             (c (and (equal? (length lst) 3) (tree->scheme  (caddr lst)))))
        (if c
            (list b a c)
            (list a b)))
      (if (equal? lst '^)
          'expt
          lst)))

; Unit tests

(define tests (list
               (test (tree->scheme (parse (tokenize "x^(a + 1)"))) '(expt x (+ a 1)))
               (test (tree->scheme (parse (tokenize "-a"))) '(- a))
               (test (eval (tree->scheme (parse (tokenize "2^2^2^2"))) (interaction-environment)) 65536)
               ))

(run-tests tests)