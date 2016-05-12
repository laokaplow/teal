(if (= 7 6)
  (display "green")
  (display "blue"))

(if (= (quote red) (quote red))
    (display "green")
    (display "blue"))

(define cc 's')
(cond
  [(= cc "s")
    (begin
      ;(set! start-node here)
      (display (list (quote start) 1 2)))]
  [(= cc "e")
    (begin
      ;(set! end-node here)
      (display (list (quote end) 3 4)))])
