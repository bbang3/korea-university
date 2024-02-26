open Printf;;
let rec unzip : ('a * 'b) list -> 'a list * 'b list 
  =fun tup_lst ->
    match tup_lst with
    | [] -> [], []
    | (a, b)::tl ->
      let alst, blst = unzip tl in 
      (a::alst, b::blst);;

unzip [(1, "one"); (2, "two"); (3, "three")];;