(load "trace.scm")

(define (my-map-1 func lst)
  (if (null? lst)
      '()
      (cons (func (car lst)) (my-map-1 func (cdr lst)))))

(define (my-map-2-listed func lsts)
  (define (any-null? lst)
    (and (not (null? lst))
         (or (null? (car lst))
             (any-null? (cdr lst)))))
  (if (any-null? lsts)
      '()
      (cons (apply func (my-map-1 car lsts))
            (my-map-2-listed func (my-map-1 cdr lsts)))))

(define (my-map-2 func . lsts)
  (my-map-2-listed func lsts))

(define (subst lst ass)
  (if (list? lst)
      (if (null? lst)
          '()
          (cons (subst (car lst) ass) (subst (cdr lst) ass)))
      (if (assq lst ass)
          (cadr (assq lst ass))
          lst)))