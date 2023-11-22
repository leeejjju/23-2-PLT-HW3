#lang plai
; HW3 due 11.03 Fri
; 22100579 JinjuLee


;; abstract
(define-type FAE
  [num  (n number?)]
  [add  (lhs FAE?) (rhs FAE?)]
  [sub  (lhs FAE?) (rhs FAE?)]
  [id   (name symbol?)]
  [fun  (param symbol?) (body FAE?)]
  [app  (fun-expr FAE?) (arg-expr FAE?)])

(define-type FAE-Value
  [numV       (n number?)]
  [closureV   (param symbol?)
              (body FAE?)
              (ds DefrdSub?)])

(define-type DefrdSub
  [mtSub]
  [aSub   (name symbol?) (value FAE-Value?) 
          (ds DefrdSub?)])

; [contract]    lookup : symbol DefrdSub → FAE-Value
; [purpose]     find the value of the variable in ds with the name recursively
; [test]        (test (lookup 'x dss) (numV 10))
;               (test (lookup 'y dss) (numV 20))
;               (test (lookup 'z dss) (numV 30))
(define (lookup name ds)
    (type-case DefrdSub ds
        [mtSub () (error 'lookup "no binding for identifier")]
        [aSub (bound-name bound-value rest-ds)
                (if (symbol=? bound-name name)
                bound-value
                (lookup name rest-ds))]))

(define dss (aSub 'x (numV 10) (aSub 'y (numV 20) (aSub 'z (numV 30) (mtSub)))))
(test (lookup 'x dss) (numV 10))
(test (lookup 'y dss) (numV 20))
(test (lookup 'z dss) (numV 30))


; [contract] num+ : numV numV -→ numV
; [purpose] calcutate mongs numVs
; [test]    (test (num+ (numV 10) (numV 10)) (numV 20))
;           (test (num+ (numV 10) (numV 2)) (numV 12))
;           (test (num+ (numV 5) (numV 5)) (numV 10))
(define (num+ n1 n2)
  (numV (+ (numV-n n1) (numV-n n2))))

(test (num+ (numV 10) (numV 10)) (numV 20))
(test (num+ (numV 10) (numV 2)) (numV 12))
(test (num+ (numV 5) (numV 5)) (numV 10))


; [contract] num- : numV numV -→ numV
; [purpose] calcutate mongs numVs
; [test]    (test (num- (numV 10) (numV 10)) (numV 0))
;           (test (num- (numV 10) (numV 2)) (numV 8))
;           (test (num- (numV 15) (numV 5)) (numV 10))
(define (num- n1 n2)
  (numV (- (numV-n n1) (numV-n n2))))

(test (num- (numV 10) (numV 10)) (numV 0))
(test (num- (numV 10) (numV 2)) (numV 8))
(test (num- (numV 15) (numV 5)) (numV 10))





;; parser
; [contract] parse : sexp -> FAE
; [purpose] parse expression to FAE abstract form.
; [test]    (test (parse '(+ 10 10)) (add (num 10) (num 10)))
;           (test (parse '(- 10 2)) (sub (num 10) (num 2)))
;           (test (parse 30) (num 30))
(define (parse sexp)
   (match sexp
        [(? number?)                (num sexp)]
        [(list '+ l r)              (add (parse l) (parse r))]
        [(list '- l r)              (sub (parse l) (parse r))]
        [(list 'with (list i v) e)  (app (fun i (parse e)) (parse v))]
        [(? symbol?)                (id sexp)]
        [(list 'fun (list p) b)     (fun p (parse b))]
        [(list f a)                 (app (parse f) (parse a))]
        [else                       (error 'parse "bad syntax: ~a" sexp)]))

(test (parse '(+ 10 10)) (add (num 10) (num 10)))
(test (parse '(- 10 2)) (sub (num 10) (num 2)))
(test (parse 30) (num 30))


;; interpreter
; [contract] interp : FAE -> FAE-Value
; [purpose] parse expression to FAE abstract form.
; [test]    (test (interp (parse '(+ 10 10)) mtSub) (numV 20))
;           (test (interp (parse '(- 10 2)) mtSub) (numV 8))
;           (test (interp (parse 30) mtSub) (numV 30))
(define (interp fae ds)
  (type-case FAE fae
    [num (n) (numV n)]
    [add (l r) (num+ (interp l ds) (interp r ds))]
    [sub (l r) (num- (interp l ds) (interp r ds))]
    [id (s) (lookup s ds)]
    [fun (p b) (closureV p b ds)]
    [app (f a) (local   
                    [(define fun-val (interp f ds))
                    (define arg-val (interp a ds))]
                    (interp (closureV-body fun-val)
                            (aSub   (closureV-param fun-val)
                                    arg-val
                                    (closureV-ds fun-val))))]))

(test (interp (parse '(+ 10 10)) mtSub) (numV 20))
(test (interp (parse '(- 10 2)) mtSub) (numV 8))
(test (interp (parse 30) mtSub) (numV 30))
(parse '(with (y 10) (+ y y)))
(interp (parse '(with (y 10) (+ y y))) (mtSub))
(parse '(1 2))
