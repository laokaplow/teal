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
            [(quote dist) (- 0 1)])))))))

;(display graph)(exit)

(define node-at
  (lambda (x y) (nth (nth graph y) x)))

;(display (node-at 0 0) (char "\n"))
;(display (node-at 1 0) (char "\n"))
;(display (node-at 2 0) (char "\n"))
;(display (node-at 0 1) (char "\n"))
;(display (node-at 1 1) (char "\n"))
;(display (node-at 2 1) (char "\n"))
;(display (node-at 0 2) (char "\n"))
;(display (node-at 1 2) (char "\n"))
;(display (node-at 2 2) (char "\n"))
;(exit)

(define print-maze
  (lambda ()
    (for-each (range 0 height)
      (lambda (y)
        (begin
          (for-each (range 0 width)
            (lambda (x)
              (display (get-member (node-at x y) (quote contents)))))
          (display (char "\n")))))))

;(print-maze)

(define try-connect
  (lambda (node x y)
    (if (and (<= 0 x) (< x width) (<= 0 y) (< y height))
      ; then
      (begin
        (define other (node-at x y))
        (if (not (= "#" (get-member other (quote contents))))
          (begin
            (define neighbors (get-member node (quote neighbors)))
            (set-member! node (quote neighbors) (cons other neighbors)))
          ()))
      ())))

(print-maze)

(define cc '*')

; hook up neighboring nodes
(for-each (range 0 height)
  (lambda (y)
    (begin
      (for-each (range 0 width)
        (lambda (x)
          (begin
            (define here (node-at x y))
            ;(define contents (get-member here (quote contents)))
            (set! cc (get-member here (quote contents)))
            (display cc)
            (cond
              [(= cc "s")
                (begin
                  (set! start-node here)
                  (display (list (quote start) x y)))]
              [(= cc "s")
                (begin
                  (set! end-node here)
                  (display (list (quote end) x y)))])
              ;[(= contents 's') (set! start-node here)]
              ;[(= contents 'e') (set! end-node here)])
            ;(try-connect here x (+ y 1))
            ;(try-connect here x (- y 1))
            ;(try-connect here (+ x 1) y)
            ;(try-connect here (- x 1) y)
            ))))))

(display (list start-node end-node))(exit)

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
