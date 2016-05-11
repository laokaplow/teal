(define maze_rows (read_lines IO::STDIN))

; convert maze (make links to adjacent cells)
; return graph, start_node, end node, visited
; cells have neighbors

;do bfs
(define q (list start_node))

define bfs
