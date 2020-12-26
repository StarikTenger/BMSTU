(load "trace.scm")
(load "unit-test.scm")
(load "assert.scm")
(load "LAB_4.scm")
(load "HW_4.scm")

;; == 1 : SIMPLE SCANERS ===============================================
; <Digit> ::= 1|2|3|4|5|6|7|8|9|0
; <Number> ::= <Digit> | <Digit> <Number>
; <Sign> = + | -
; <Signed Number> ::= <Number> | <Sign> <Number>
; <Fraction> ::= <Signed Number> / <Number>
; <Complete Fraction> ::= <Fraction> finish-symbol
; <Barrier Symbol> ::= space | tab | newline
; <Barrier> ::= <> | <Barrier Symbol> | <Barrier Symbol> <Barrier>
; <Isolated Fraction> ::= <Barrier> <Fraction> <Barrier>
; <Sequence> ::= <Isolated Fraction> | <Isolated Fraction> <Sequence>
; <Complete Sequence> ::= <Sequence> finish-symbol

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

; <Digit> ::= 1|2|3|4|5|6|7|8|9|0
(define digits (map (lambda (x) (list x)) (string->list "0123456789")))
(define (scan-digit stream)
  (let ((symbol (stream-peek stream)))
    (and (assq symbol digits)
         (begin (stream-next stream) symbol))))

; <Number> ::= <Digit> | <Digit> <Number>
(define (scan-number stream)
  (let ((digit (scan-digit stream)))
    (and digit
         (let ((number (scan-number stream)))
           (if number
               (string-append (string digit) number)
               (string digit))))))

; <Sign> = + | -
(define (scan-sign stream)
  (let ((symbol (stream-peek stream)))
    (and (or (equal? symbol #\+) (equal? symbol #\-))
         (begin (stream-next stream) symbol))))

; <Signed Number> ::= <Number> | - <Number>
(define (scan-signed-number stream)
  (let ((sign (scan-sign stream))
        (number (scan-number stream)))
    (if sign
        (and number (string-append (string sign) number))
        number)))

; /
(define (scan/ stream)
  (let ((symbol (stream-peek stream)))
    (and (equal? symbol #\/)
         (begin (stream-next stream) symbol))))

; <Fraction> ::= <Signed Number> / <Number>
(define (scan-fraction stream)
  (let ((val
         (let ((signed-number (scan-signed-number stream))
               (slash (scan/ stream))
               (number (scan-number stream)))
           (and signed-number slash number
                (string-append signed-number (string slash) number)))))
    (and val (string->number val))))

; finish-symbol
(define (scan-finish-symbol stream)
  (let ((symbol (stream-peek stream)))
    (and (equal? symbol finish-symbol)
         (begin (stream-next stream) symbol))))

; <Complete Fraction> ::= <Fraction> finish-symbol
(define (scan-complete-fraction stream)
  (let ((fraction (scan-fraction stream))
        (finish-symbol (scan-finish-symbol stream)))
    (and fraction finish-symbol
         fraction)))

; <Barrier Symbol> ::= space | tab | newline
(define (scan-barrier-symbol stream)
  (let ((symbol (stream-peek stream)))
    (and (or (equal? symbol #\space) (equal? symbol #\tab) (equal? symbol #\newline))
         (begin (stream-next stream) symbol))))

; <Barrier> ::= <> | <Barrier Symbol> | <Barrier Symbol> <Barrier>
(define (scan-barrier stream)
  (let ((barrier-symbol (scan-barrier-symbol stream)))
    (if barrier-symbol
        (let ((barrier (scan-barrier stream)))
          (if barrier
              (string-append (string barrier-symbol) barrier)
              (string barrier-symbol)))
        "")))

; <Isolated Fraction> ::= <Barrier> <Fraction> <Barrier>
(define (scan-isolated-fraction stream)
  (let ((barrier-left (scan-barrier stream))
        (fraction (scan-fraction stream))
        (barrier-right (scan-barrier stream)))
    fraction))

; <Sequence> ::= <Isolated Fraction> | <Isolated Fraction> <Sequence>
(define (scan-sequence stream)
  (let ((isolated-fraction (scan-isolated-fraction stream)))
    (and isolated-fraction
         (let ((sequence (scan-sequence stream)))
           (if sequence
               (append (list isolated-fraction) sequence)
               (list isolated-fraction))))))

; <Complete Sequence> ::= <Sequence> finish-symbol
(define (scan-complete-sequence stream)
  (let ((sequence (scan-sequence stream))
        (finish-symbol (scan-finish-symbol stream)))
    (and sequence finish-symbol
         sequence)))

; -- Functions from task -----------------------------------------
(define (check-frac str)
  (let ((val (scan-complete-fraction (string->stream str))))
    (not (not val))))

(define (scan-frac str)
  (let ((val (scan-complete-fraction (string->stream str))))
    val))

(define (scan-many-fracs str)
  (let ((val (scan-complete-sequence (string->stream str))))
    val))

;; Unit-testing
(define tests (list
               (test (check-frac "110/111") #t)
               (test (check-frac "-4/3") #t)
               (test (check-frac "+5/10") #t)
               (test (check-frac "5.0/10") #f)
               (test (check-frac "FF/10") #f)
               
               (test (scan-frac "1/2") 1/2)
               (test (scan-frac "-1/2") -1/2)
               (test (scan-frac "110/111") 110/111)
               (test (scan-frac "-4/3") -4/3)
               (test (scan-frac "+5/10") 1/2)
               (test (scan-frac "5.0/10") #f)
               (test (scan-frac "FF/10") #f)
               (test (scan-frac "1/a") #f)
               (test (scan-frac "1/10a") #f)

               (test (scan-barrier (string->stream "123")) "")
               (test (scan-barrier (string->stream " 123")) " ")
               (test (scan-barrier (string->stream "   123")) "   ")
               (test (scan-barrier (string->stream "\n \t  ")) "\n \t  ")

               (test (scan-isolated-fraction (string->stream " \t\n 12/3 \t")) 4)
               (test (scan-sequence (string->stream "1/2 3/4")) '(1/2 3/4))

               (test (scan-many-fracs "\t1/2 1/3\n\n2/-5") #f)
               (test (scan-many-fracs "   1/2   4/5 \t\n  5/2  ") '(1/2 4/5 5/2))
               ))

(run-tests tests)

;; == 2 : PARSE ========================================================
; <Program>  ::= <Articles> <Body> .
; <Articles> ::= <Article> <Articles> | .
; <Article>  ::= define word <Body> end .
; <Body>     ::= if <Body> endif <Body> | integer <Body> | word <Body> | .

(define (vector->stream vec)
  (make-stream (append (vector->list vec) (list finish-symbol))))

(define (parse vec)
  (letrec (
           (stream (vector->stream vec))
           (forbidden-words (map list (list finish-symbol 'endif 'end)))
           ; finish-symbol
           (parse-finish-symbol (lambda ()
                                  (let ((finish (stream-peek stream)))
                                    (and (equal? finish finish-symbol)
                                         (begin (stream-next stream) finish)))))
           ; <Program>  ::= <Articles> <Body> .
           (parse-program (lambda ()
                            (let ((articles (parse-articles))
                                  (body (parse-body))
                                  (finish (parse-finish-symbol)))
                              (and (and articles body finish)
                                  (list articles body)))))
           ; <Articles> ::= <Article> <Articles> | .
           (parse-articles (lambda ()
                             (let* ((article (parse-article))
                                    (articles (and
                                               (not (equal? article '()))
                                               article
                                               (parse-articles))))
                               (if (and article articles (not (equal? article '())))
                                   (cons article articles)
                                   article))))
           ; define
           (parse-define (lambda ()
                           (let ((def (stream-peek stream)))
                             (and (equal? def 'define)
                                  (begin (stream-next stream) def)))))
           ; end
           (parse-end (lambda ()
                        (let ((end (stream-peek stream)))
                          (and (equal? end 'end)
                               (begin (stream-next stream) end)))))
           ; word
           (parse-word (lambda ()
                         (let ((word (stream-peek stream)))
                           (begin
                             (stream-next stream)
                             word))))
           ; <Article>  ::= define word <Body> end .
           (parse-article (lambda ()
                            (let* ((def (parse-define))
                                   (word (and def (parse-word)))
                                   (body (and word (parse-body)))
                                   (end (and body (parse-end))))
                              (if def
                                  (and def word body end
                                   (list word body))
                                  '()))))
           ; endif
           (parse-endif (lambda ()
                          (let ((endif (stream-peek stream)))
                            (and (equal? endif 'endif)
                                 (begin (stream-next stream) endif)))))
           ; <Body> ::= if <Body> endif <Body> | integer <Body> | word <Body> | .
           (parse-body (lambda ()
                         (let ((first (stream-peek stream)))
                           (cond
                             ; if <Body> endif <Body>
                             ((equal? first 'if)
                              (stream-next stream)
                              (let* ((body (parse-body))
                                     (endif (and body (parse-endif)))
                                     (body-next (and endif (parse-body))))
                                (and first body endif body-next
                                     (append (list (append (list first body)))  body-next))))
                             ; integer <Body>
                             ((number? first)
                              (stream-next stream)
                              (let ((body (parse-body)))
                                (and first body
                                     (cons first body))))
                             ; word <Body>
                             ((not (assq first forbidden-words))
                              (stream-next stream)
                              (let ((body (parse-body)))
                                (and first body
                                     (cons first body))))
                             ; else
                             (else '())
                             )))))
    (parse-program)))


;; Unit-testing
(define tests (list
               (test (parse #(1 2 +)) '(() (1 2 +)))
               (test (parse #(x dup 0 swap if drop -1 endif)) '(() (x dup 0 swap (if (drop -1)))))
               (test (parse #( define -- 1 - end
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
                                4 factorial )) '(((-- (1 -))
                                                  (=0? (dup 0 =))
                                                  (=1? (dup 1 =))
                                                  (factorial
                                                   (=0? (if (drop 1 exit)) =1? (if (drop 1 exit)) dup -- factorial *)))
                                                 (0 factorial 1 factorial 2 factorial 3 factorial 4 factorial)))
               (test (parse #(if 1 2 endif if 3 4)) #f)
               (test (parse #(define word w1 w2 w3)) #f)
               ))

(run-tests tests)

