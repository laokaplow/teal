(define lines (read-lines))
(define height (len lines))
(define width (len (first lines)))
;(display lines (list width height))(exit)
(define start-node ())
(define end-node ())

(define nl (char "\n"))

(display nl nl "loading maze..." nl)

(define graph
  (for-each lines
    (lambda (line)
      (vectorize
        (for-each line
          (lambda (c) ; make node from char
            (dict (
              [(quote neighbors) (vectorize ())]
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


(define print-es
  (lambda ()
    (for-each (range 0 height)
      (lambda (y)
        (begin
          (for-each (range 0 width)
            (lambda (x)
              (display (len (get-member (node-at x y) (quote es)))))
          (display nl)))))))


(define try-connect
  (lambda (node x y)
    (if (and (<= 0 x) (< x width) (<= 0 y) (< y height))
      ; then
      (begin
        (define other (node-at x y))
        (if (not (= (char "#") (get-member other (quote contents))))
          (begin
            (define neighbors (get-member node (quote neighbors)))
            (vector-append! neighbors other)
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

(display "connecting graph..." nl)
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
              [(= contents (char "s")) (set! start-node here)]
              [(= contents (char "e")) (set! end-node here)])
            (try-connect here x (+ y 1))
            (try-connect here x (- y 1))
            (try-connect here (+ x 1) y)
            (try-connect here (- x 1) y)
            () ; not intending to return a meaningfull value
            ))))))

(display nl nl)
(print-maze)
(display nl)
(print-es) ; number of open edges per node
(display nl nl)


(display (quote start) start-node nl)
(display (quote end) end-node nl nl)
;(exit)

;(display (quote start) (get-member start-node (quote pos)) (char "\n\n"))
;(display (quote end) (get-member end-node (quote pos)) (char "\n\n"))
;(exit)

; do bfs
(display "begin pathfinding" nl)
(begin
  (set-member! start-node (quote dist) 0)
  (define q (vectorize (list start-node)))
  (define next (vectorize ()))
  (while (not (empty? q))
    (begin
      (while (not (empty? q))
        (begin
          (display (len q))
          (define here (vector-pop! q))
          (if (= (get-member here (quote contents)) (char 'e')) ; reached end
            ; then
            (begin ; break
              (set! q    (vectorize ()))
              (set! next (vectorize ()))
              )
            ; else
            (for-each (get-member here (quote neighbors))
              (lambda (there)
                (cond
                  ; don't enqueue nodes we have already seen
                  [(= (get-member there (quote dist)) (- 0 1))
                    (begin
                      (define cur-dist (get-member here (quote dist)))
                      (set-member! there (quote dist) (+ cur-dist 1))
                      (vector-append! next there))]))))))
        (set! q next)
        (set! next (vectorize ())))))

(display nl "path finding complete." nl nl)
;(exit)

(define breadcrumbs "\x1B[92m*\x1B[0m")

(display "marking path" nl)
; mark return path
(define here end-node)
(while (not (= 0 (get-member here (quote dist))))
  (begin
    (define c (get-member here (quote contents)))
    (define cur-dist (get-member here (quote dist)))
    (display cur-dist nl)
    (for-each (get-member here (quote neighbors))
      (lambda (neighbor)
        (if (= (- cur-dist 1) (get-member neighbor (quote dist)))
          (begin
            (display (get-member neighbor (quote pos) nl))
            (set-member! neighbor (quote contents) breadcrumbs)
            (set! here neighbor))
          ; else, not closer
          nil)))))


(display "printing results:" nl nl)
; print results
(define dist-to-exit (get-member end-node (quote dist)))
(if (= dist-to-exit (- 0 1))
  ; then
  (display "no path possible" nl)
  ; else
  (print-maze))

(display "done" nl nl)
