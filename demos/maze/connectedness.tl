(define lines (read-lines))
(define height (len lines))
(define width (len (first lines)))
;(display lines (list width height))(exit)
(define start-node ())
(define end-node ())

(define graph
  (for-each lines
    (lambda (line)
      (for-each line
        (lambda (c) ; make node from char
          (dict (
            [(quote neighbors) ()]
            [(quote contents) c]
            [(quote pos) ()]
            [(quote cc) ()]
            [(quote dist) (- 0 1)])))))))

;(display graph)(exit)

(define node-at
  (lambda (x y) (nth (nth graph y) x)))

(define print-maze
  (lambda ()
    (for-each (range 0 height)
      (lambda (y)
        (begin
          (for-each (range 0 width)
            (lambda (x)
              (display (get-member (node-at x y) (quote contents)))))
          (display (char "\n")))))))

(define print-cc
  (lambda ()
    (for-each (range 0 height)
      (lambda (y)
        (begin
          (for-each (range 0 width)
            (lambda (x)
              (display (len (get-member (node-at x y) (quote neighbors)))))
          (display (char "\n"))))))))


(define try-connect
  (lambda (node x y)
    (if (and (<= 0 x) (< x width) (<= 0 y) (< y height))
      ; then
      (begin
        (define other (node-at x y))
        (if (not (= (char "#") (get-member other (quote contents))))
          (begin
            (define neighbors (get-member node (quote neighbors)))
            (set-member! node (quote neighbors) (range 0 (+ 1 (len neighbors))))
            ; connection count
            (define cc (get-member node (quote cc)))
            (set-member! node (quote cc) (range 0 (+ 1 (len cc)))
            ))
          ()))
      ; else
      ())))

;(print-maze)

(define start-pos ())
(define end-pos ())


; hook up neighboring nodes
(for-each (range 0 height)
  (lambda (y)
    (begin
      (for-each (range 0 width)
        (lambda (x)
          (begin
            (define here (node-at x y))
            (set-member! here (quote pos) (list x y))
            (define contents (get-member here (quote contents)))
            (cond
              [(= contents (char "s")) (set! start-pos (list x y))]
              [(= contents (char "e")) (set! end-pos (list x y))])
            (try-connect here x (+ y 1))
            (try-connect here x (- y 1))
            (try-connect here (+ x 1) y)
            (try-connect here (- x 1) y)
            ))))))

(print-maze)
(print-cc)
(exit)

;(define hookup (lambda (from to)
;  (begin
;    (if (not (= (char "#") (get-member to (quote contents))))
;      (begin
;        (define old_list (get-member from (quote neighbors)))
;        (define new_list (cons to oldlist))
;        (set-member! from (quote neighbors) new_list))
;      ; else, do nothing, it is a wall
;      ()))))
;
;; hook up neighboring nodes
;(define last ())
;(for-each (range 0 height) (lambda (y)
;  (begin
;    (set! last ())
;    (for-each (range 0 width) (lambda (x)
;      (begin
;        (define here (node-at x y))
;        (set-member! here (quote pos) (list x y))
;        (define contents (get-member here (quote contents)))
;        (cond
;          [(= contents (char "s")) (set! start-pos (list x y))]
;          [(= contents (char "e")) (set! end-pos (list x y))])
;        (if (not (empty? last)
;          (begin
;            (hookup here last)
;            (hookup last here))
;          ()))
;        (set! last here)))))))
;
(display (get-member start-node (quote pos)) (char "\n\n"))
(display (get-member end-node (quote pos)) (char "\n\n"))
(exit)

; do bfs
(begin
  (set-member! start-node (quote dist) 0)
  (define q (list start-node))
  (define next ())
  (while (not (empty? q))
    (while (not (empty? q))
      (begin
        (define here (first q))
        (set! q (rest q))
        (if (= (get-member here (quote contents)) 'e')
          ; then
          (set! q ()) ; break
          ; else
          (for-each (get-member here (quote neighbors))
            (lambda (there)
              (cond
                [(= (get-member there (quote dist)) (- 0 1))
                  (begin
                    (define cur-dist (get-member here (quote dist)))
                    (set-member! there (quote dist) (+ cur-dist 1))
                    (set! next (cons there next)))]))))
        (set! q next)
        (set! next ())))))

; mark return path
(define here end-node)
(while (not (= (get-member here (quote contents))))
  (begin
    (define c (get-member here (quote contents)))
    (define dist (get-member here (quote dist)))
    (for-each (get-member here (quote neighbors))
      (lambda (neighbor)
        (if (= (- dist 1) (get-member neighbor (quote dist)))
          (set! here neighbor)
          nil)))))

; print results
(define dist-to-exit (get-member end-node (quote dist)))
(if (not (= dist-to-exit (- 0 1)))
  ; then
  (display "no path possible" (char "\n"))
  ; else
  (print-maze graph))
