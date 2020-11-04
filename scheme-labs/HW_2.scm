;;; 1 LISTS ;;;
; range
(define (my-range a b d)
  (if (< a b)
      (append (list a) (my-range (+ a d) b d))
      '()
      )
  )
; flatten
(define (my-flatten-raw xs l)
  ( if (pair? xs)
       (if (pair? (car xs))
           (append (list) (my-flatten-raw (car xs) l) (my-flatten-raw (cdr xs) l))
           (append l (list (car xs)) (if (pair? (cdr xs))
                                         (my-flatten-raw (cdr xs) l)
                                         (list))))
       xs))

(define (my-flatten xs)
  (my-flatten-raw xs '()))

; element
(define (my-element? x xs)
  (and (pair? xs) (or (equal? (car xs) x) (my-element? x (cdr xs)))))

; filter
(define (my-filter pred? xs)
  (if (pair? xs)
      (append (if (pred? (car xs)) (list (car xs)) (list) ) (my-filter pred? (cdr xs)))
      (list)))

; fold
(define (my-fold-right op xs)
  (if (pair? (cdr xs))
      (op (car xs) (my-fold-right op (cdr xs)))
      (car xs)))

(define (my-fold-left-raw op xs val)
  (if (pair? (cdr xs))
      (my-fold-left-raw op (cdr xs) (op val (car xs)))
      (op val (car xs))))

(define (my-fold-left op xs)
  (my-fold-left-raw op (cdr xs) (car xs)))

; exapmles
(display "--- 1 ---\n" )
(my-range  0 11 3) ; (0 3 6 9)

(my-flatten '((1) 2 (3 (4 5)) 6)) ; (1 2 3 4 5 6)

(my-element? 1 '(3 2 1)) ; #t
(my-element? 4 '(3 2 1)) ; #f

(my-filter odd? (my-range 0 10 1)) ; (1 3 5 7 9)
(my-filter (lambda (x) (= (remainder x 3) 0)) (my-range 0 13 1)) ; (0 3 6 9 12)

(my-fold-left  quotient '(16 2 2 2 2)) ; 1
(my-fold-right expt     '(2 3 4))      ; 2417851639229258349412352
;

;;; 2 SETS ;;;
; delete
(define (delete l pred?)
  (if (pair? l)
      (append (if (pred? (car l)) (list) (list (car l)) ) (delete (cdr l) pred?))
      (list)))

; list->set
(define (list->set xs)
  (if (and (pair? xs))
      (append (list (car xs)) (list->set (delete (cdr xs) (lambda (x) (= x (car xs))))))
      xs))

; set?
(define (set? xs)
  (or (not (pair? xs))
      (and (not (my-element? (car xs) (cdr xs))) (set? (cdr xs)))))

; union
(define (union xs ys)
  (list->set (append xs ys)))

; intersection
(define (intersection xs ys)
  ( if (pair? xs)
       (append
        (if (my-element? (car xs) ys)
            (list (car xs))
            (list))
        (intersection (cdr xs) ys))
       (list)))

; difference
(define (difference xs ys)
  ( if (pair? xs)
       (append
        (if (my-element? (car xs) ys)
            (list)
            (list (car xs)))
        (difference (cdr xs) ys))
       (list)))

; symmetric-difference
(define (symmetric-difference xs ys)
  (union (difference xs ys) (difference ys xs)))

; subset
(define (set-subset? xs ys)
  ( or
    (not (pair? xs))
    (and
     ( my-element? (car xs) ys)
     (set-subset? (cdr xs) ys))))

; equality
(define (set-eq? xs ys)
  (and (set-subset? xs ys) (set-subset? ys xs)))

; exapmles
(display "--- 2 ---\n" )
(my-range  0 11 3) ; (0 3 6 9)

(list->set '(1 1 2 3))                       ; (3 2 1)
(set? '(1 2 3))                              ; #t
(set? '(1 2 3 3))                            ; #f
(set? '())                                   ; #t
(union '(1 2 3) '(2 3 4))                    ; (4 3 2 1)
(intersection '(1 2 3) '(2 3 4))             ; (2 3)
(difference '(1 2 3 4 5) '(2 3))             ; (1 4 5)
(symmetric-difference '(1 2 3 4) '(3 4 5 6)) ; (6 5 2 1)
(set-eq? '(1 2 3) '(3 2 1))                  ; #t
(set-eq? '(1 2) '(1 3))                      ; #f
;

;;; 3 STRINGS ;;;
(define (list-trim-left str) ; in list format
  (if (or (not (pair? str))
          (or (equal? (car str) #\newline)
              (equal? (car str) #\space)
              (equal? (car str) #\tab)))
      (list-trim-left (cdr str))
      str))

; string-trim-left
(define (string-trim-left str)
  (list->string (list-trim-left (string->list str))))

; reverse
(define (my-reverse xs)
  (if (pair? xs)
      (append (reverse (cdr xs)) (list (car xs)))
      (list)))

; string-trim-right
(define (string-trim-right str)
  (list->string (my-reverse (list-trim-left (my-reverse (string->list str))))))

; string-trim
(define (string-trim str)
  (string-trim-left (string-trim-right str))
  )

; prefix
(define (list-prefix? a b)
  (or
   (not (pair? a))
   (and
    (pair? b)
    (and (equal? (car a) (car b)) (list-prefix? (cdr a) (cdr b))))))

(define (string-prefix? a b)
  (list-prefix? (string->list a) (string->list b)))

; suffix
(define (list-suffix? a b)
  (list-prefix? (my-reverse a) (my-reverse b)))

(define (string-suffix? a b)
  (list-suffix? (string->list a) (string->list b)))

; infix
(define (list-infix? a b)
  (and (pair? b) (or (list-prefix? a b) (list-infix? a (cdr b)))))

(define (string-infix? a b)
  (list-infix? (string->list a) (string->list b)))

; some functions
(define (cdr-iter xs i) ; applies cdr for i times
  (if (and (> i 0) (pair? xs))
      (cdr-iter  (cdr xs) (- i 1))
      xs))

(define (size xs) ; list size
  (if (pair? xs)
      (+ 1 (size (cdr xs)))
      0))

(define (list-apply f xs) ; applies f to every element
  (if (pair? xs)
      (append (list (f (car xs))) (list-apply f (cdr xs)))
      (list)))

; split
(define (list-split-p l sep buff sep-size)
  (if (pair? l)
      (if (list-prefix? sep l)
          (append (list buff) (list-split-p (cdr-iter l sep-size) sep (list) sep-size))
          (list-split-p (cdr l) sep (append buff (list (car l))) sep-size))
      (list buff)))

(define (list-split l sep)
  (list-split-p l sep (list) (size sep)))

(define (string-split str sep)
  (list-apply list->string (list-split (string->list str) (string->list sep))))


; exapmles
(display "--- 3 ---\n" )
(string-trim-left  "\t\tabc def")   ; "abc def"
(string-trim-right "abc def\t")     ; "abc def"
(string-trim       "\t abc def \n") ; "abc def"

(string-prefix? "abc" "abcdef")  ; #t
(string-prefix? "bcd" "abcdef")  ; #f

(string-suffix? "def" "abcdef")  ; #t
(string-suffix? "bcd" "abcdef")  ; #f

(string-infix? "def" "abcdefgh") ; #t
(string-infix? "abc" "abcdefgh") ; #t
(string-infix? "fgh" "abcdefgh") ; #t
(string-infix? "ijk" "abcdefgh") ; #f

(string-split "x;y;z" ";")       ; ("x" "y" "z")
(string-split "x-->y-->z" "-->") ; ("x" "y" "z")
;

;;; 4 multi-dim vector ;;;

; multiplies all elements in list by each other
(define (multiply xs)
  (if (pair? xs)
      (* (car xs) (multiply (cdr xs)))
      1))

; sets elements to xs values startine from index i
(define (vector-set-range! vec i xs)
  (if (pair? xs)
      (vector-set! vec i (car xs)))
  (if (pair? xs)
      (vector-set-range! vec ( + i 1) ( cdr xs))))

; subvector from i to j represented as list
(define (subvector->list vec i j)
  (if (> i j)
      '()
      (cons (vector-ref vec i) (subvector->list vec (+ i 1) j))))

; makes multi filled multi-vector
(define (make-multi-vector-fill sizes fill)
  (define vec (make-vector (+ 1 (size sizes) (multiply sizes)) fill))
  (vector-set! vec 0 (size sizes))
  (vector-set-range! vec 1 sizes)
  vec)

; makes filled/not filled multi-vector depends on number of arguments
(define (make-multi-vector . args)
  (if (= (size args) 2)
      (make-multi-vector-fill (car args) (car (cdr args)))
      (make-multi-vector-fill (car args) 0)))

; checks if m is multi-vector
(define (multi-vector? m)
  (define len (vector-length m))
  (and
   (> len 0)
   (> len (vector-ref m 0))
   (= len (+ 1 (vector-ref m 0) (multiply (subvector->list m 1 (vector-ref m 0)))))))

; scalar product of a and b, where a and b are lists
(define (dot-prod a b)
  (if (pair? a)
      (+ (* (car a) (car b)) (dot-prod (cdr a) (cdr b)))
      0))

; shape of multi-vector
(define (multi-vector-shape m)
  (subvector->list m 1 (vector-ref m 0)))

; converts indicies to one-number index
(define (index shape indicies)
  (define s (cons 1 (cdr (reverse shape))))
  (+ (dot-prod s indicies) 1 (size indicies)))

; gets element
(define (multi-vector-ref m indices)
  (define s (multi-vector-shape m)) ; shape
  (vector-ref m (index s indices)))

; sets element
(define (multi-vector-set! m indices x)
  (define s (multi-vector-shape m)) ; shape
  (vector-set! m (index s indices) x))

; exapmles
(display "--- 4 ---\n" )
(define m (make-multi-vector '(11 12 9 16)))
(multi-vector? m)
(multi-vector-set! m '(10 7 6 12) 'test)
(multi-vector-ref m '(10 7 6 12)) ; test

(define m (make-multi-vector '(3 5 7) -1))
(multi-vector-ref m '(0 0 0)) ; -1
;


;;; 5 func-composition ;;;
(define (o-raw args)
  (if (pair? args)
      (lambda (x) ((o-raw (cdr args)) ((car args) x)))
      (if (pair? args)
          (car args)
          (lambda (x) x))))

(define (o . args)
  (o-raw (reverse args)))

; examples
(display "--- 5 ---\n" )
(define (f x) (* x 2))
(define (g x) (* x 3))
(define (h x) (- x))
    
((o f g h) 1) ; -6
((o f g) 1)   ; 6
((o h) 1)     ; -1
((o) 1)       ; 1

;;; quick-sort ;;
; filter
(define (filter pred? xs)
  ( if (null? xs)
       '()
       (append
        (if (pred? (car xs))
            (list (car xs))
            '())
        (filter pred? (cdr xs)))))

; sort
(define (qsort xs)
  (if ( or (null? xs) )
      '()
      (append
       (qsort (filter (lambda (x) (< x (car xs))) xs))
       (list (car xs))
       (qsort (cdr (filter (lambda (x) (>= x (car xs))) xs))))))