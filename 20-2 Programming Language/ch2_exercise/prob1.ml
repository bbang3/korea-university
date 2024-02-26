open Printf;;
let rec range : int -> int -> int list
  = fun n m ->
    if n > m then []
    else n::(range (n + 1) m);;

let _ = List.iter (printf "%d ") (range 3 7);;