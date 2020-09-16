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
                                         (list)
                                         )
                   )
           )
       xs
       )
  )
(define (my-flatten xs)
  (my-flatten-raw xs '())
  )

; element
(define (my-element? x xs)
  (and (pair? xs) (or (equal? (car xs) x) (my-element? x (cdr xs))))
  )

; filter
(define (my-filter pred? xs)
  (if (pair? xs)
      (append (if (pred? (car xs)) (list (car xs)) (list) ) (my-filter pred? (cdr xs)))
      (list)
      )
  )

; fold
(define (my-fold-right op xs)
  (if (pair? (cdr xs))
      (op (car xs) (my-fold-right op (cdr xs)))
      (car xs)
      )
  )

(define (my-fold-left-raw op xs val)
  (if (pair? (cdr xs))
      (my-fold-left-raw op (cdr xs) (op val (car xs)))
      (op val (car xs))
      )
  )

(define (my-fold-left op xs)
  (my-fold-left-raw op (cdr xs) (car xs))
  )

;;; 2 SETS ;;;
; delete
(define (delete l pred?)
  (if (pair? l)
      (append (if (pred? (car l)) (list) (list (car l)) ) (delete (cdr l) pred?))
      (list)
      )
  )

; list->set
(define (list->set xs)
  (if (and (pair? xs))
      (append (list (car xs)) (list->set (delete (cdr xs) (lambda (x) (= x (car xs))))))
      xs
      )
  )

; set?
(define (set? xs)
  (or (not (pair? xs))
      (and (not (my-element? (car xs) (cdr xs))) (set? (cdr xs)))
      )
  )

; union
(define (union xs ys)
  (list->set (append xs ys))
  )

; intersection
(define (intersection xs ys)
  ( if (pair? xs)
       (append
        (if (my-element? (car xs) ys)
            (list (car xs))
            (list))
        (intersection (cdr xs) ys))
       (list)
       )
  )

; difference
(define (difference xs ys)
  ( if (pair? xs)
       (append
        (if (my-element? (car xs) ys)
            (list)
            (list (car xs)))
        (difference (cdr xs) ys))
       (list)
       )
  )

; symmetric-difference
(define (symmetric-difference xs ys)
  (union (difference xs ys) (difference ys xs))
  )

; subset
(define (set-subset? xs ys)
  ( or
    (not (pair? xs))
    (and
     ( my-element? (car xs) ys)
     (set-subset? (cdr xs) ys)
     )
    )
  )

; equality
(define (set-eq? xs ys)
  (and (set-subset? xs ys) (set-subset? ys xs))
  )

;;; 3 STRINGS ;;;
(define (list-trim-left str) ; in list format
  (if (or (not (pair? str)) (or (equal? (car str) #\space) (equal? (car str) #\tab)))
      (list-trim-left (cdr str))
      str
      )
  )

; string-trim-left
(define (string-trim-left str)
  (list->string (list-trim-left (string->list str)))
  )

; reverse
(define (my-reverse xs)
  (if (pair? xs)
      (append (reverse (cdr xs)) (list (car xs)))
      (list)
      )
  )

; string-trim-right
(define (string-trim-right str)
  (list->string (my-reverse (string-trim-left-p (my-reverse (string->list str)))))
  )

; prefix
(define (list-prefix? a b)
  (or
   (not (pair? a))
   (and
    (pair? b)
    (and (equal? (car a) (car b)) (list-prefix? (cdr a) (cdr b)))
    )
   )
  )

(define (string-prefix? a b)
  (list-prefix? (string->list a) (string->list b))
  )

; suffix
(define (list-suffix? a b)
  (list-prefix? (my-reverse a) (my-reverse b))
  )

(define (string-suffix? a b)
  (list-suffix? (string->list a) (string->list b))
  )

; infix
(define (list-infix? a b)
  (and (pair? b) (or (list-prefix? a b) (list-infix? a (cdr b))))
  )

(define (string-infix? a b)
  (list-infix? (string->list a) (string->list b))
  )

; some functions
(define (cdr-iter xs i) ; applies cdr for i times
  (if (and (> i 0) (pair? xs))
      (cdr-iter  (cdr xs) (- i 1))
      xs
      )
  )

(define (size xs) ; list size
  (if (pair? xs)
      (+ 1 (size (cdr xs)))
      0
      )
  )

(define (list-apply f xs) ; applies f to every element
  (if (pair? xs)
      (append (list (f (car xs))) (list-apply f (cdr xs)))
      (list)
      )
  )

; split
(define (list-split-p l sep buff sep-size)
  (if (pair? l)
      (if (list-prefix? sep l)
          (append (list buff) (list-split-p (cdr-iter l sep-size) sep (list) sep-size))
          (list-split-p (cdr l) sep (append buff (list (car l))) sep-size)
          )
      (list buff)
      )
  )

(define (list-split l sep)
  (list-split-p l sep (list) (size sep))
  )

(define (string-split str sep)
  (list-apply list->string (list-split (string->list str) (string->list sep)))
  )


;;; 4 multi-dim vector ;;;

(define (multiply xs)
  (if (pair? xs)
      (* (car xs) (multiply (cdr xs)))
      1
      )
  )

(define (vector-set-range! vec i xs)

  (if (pair? xs)
      (vector-set! vec i (car xs))
      )
  (if (pair? xs)
      (vector-set-range! vec ( + i 1) ( cdr xs))
      )
      
  )

(define (subvector->list vec i j)
  (if (> i j)
      '()
      (cons (vector-ref vec i) (subvector->list vec (+ i 1) j))
      )
  )

(define (make-multi-vector-fill sizes fill)
  (define vec (make-vector (+ 1 (size sizes) (multiply sizes)) fill))
  (vector-set! vec 0 (size sizes))
  (vector-set-range! vec 1 sizes)
  vec
  )

(define (make-multi-vector . args)
  (if (= (size args) 2)
      (make-multi-vector-fill (car args) (car (cdr args)))
      (make-multi-vector-fill (car args) 0)
      )
  )

(define (multi-vector? m)
  (define len (vector-length m))
  (and
   (> len 0)
   (> len (vector-ref m 0))
   (= len (+ 1 (vector-ref m 0) (multiply (subvector->list m 1 (vector-ref m 0)))))
   )
  )

(define (dot-prod a b)
  (if (pair? a)
      (+ (* (car a) (car b)) (dot-prod (cdr a) (cdr b)))
      0
      )
  )

(define (index shape indicies)
  (define s (cons 1 (cdr (reverse shape))))
  (dot-prod s indicies)
  )

(define (multi-vector-shape m)
  (subvector->list m 1 (vector-ref m 0))
  )

(define (multi-vector-ref m indices)
  (define s (multi-vector-shape m)) ; shape
  (vector-ref m (index s indices))
  )

(define (multi-vector-set! m indices x)
  (define s (multi-vector-shape m)) ; shape
  (vector-set! m (index s indices) x)
  )


;;; 5 func-composition ;;;
(define (o-raw args)
  (if (pair? args)
      (lambda (x) ((o-raw (cdr args)) ((car args) x)))
      (if (pair? args)
          (car args)
          (lambda (x) x)
          )
      )
  )

(define (o . args)
  (o-raw (reverse args))
  )