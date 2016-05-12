(define lines (read-lines))
(define height (len lines))
(define width (len (first lines)))
;(display lines (list width height))(exit)
(define start-node ())
(define end-node ())

(define graph
  (for-each lines
    (lambda (line)
      (vectorize
        (for-each line
          (lambda (c) ; make node from char
            (dict (
              [(quote neighbors) ()]
              [(quote contents) c]
              [(quote pos) ()]
              [(quote es) (vectorize ())]
              [(quote dist) (- 0 1)]))))))))

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

; vectorized version
(define print-es
  (lambda ()
    (for-each (range 0 height)
      (lambda (y)
        (begin
          (for-each (range 0 width)
            (lambda (x)
              (display (len (get-member (node-at x y) (quote es)))))
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
            (define edges (get-member node (quote es)))
            (vector-append! edges (list x y))
            )
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
            () ; not intending to return a meaningfull value
            ))))))

(print-maze)
(print-cc)
(display (char "\n"))
(print-es)
(display (char "\n"))

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
