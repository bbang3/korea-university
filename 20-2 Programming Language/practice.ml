
type exp =
  | X
  | INT of int
  | ADD of exp * exp
  | SUB of exp * exp
  | MUL of exp * exp
  | DIV of exp * exp
  | SIGMA of exp * exp * exp
;;

let rec cal
  = fun a b f -> 
    match f with
      X -> a
    |INT n -> n
    |ADD(e1, e2) -> (cal a b e1) + (cal a b e2)
    |SUB(e1, e2) -> (cal a b e1) - (cal a b e2)
    |MUL(e1, e2) -> (cal a b e1) * (cal a b e2)
    |DIV(e1, e2) -> (cal a b e1) / (cal a b e2)
    |SIGMA(INT n1, INT n2, e3) -> if n1 > n2 then 0 else (cal n1 n2 e3) + (cal (n1+1) n2 (SIGMA(INT (n1+1), INT n2, e3)))
;;


let rec calculator : exp -> int
  = fun exp -> 
    match exp with
      X -> raise (Failure "Problem")
    |INT n -> n
    |ADD(e1, e2) -> (calculator e1) + (calculator e2)
    |SUB(e1, e2) -> calculator e1 - calculator e2
    |MUL(e1, e2) -> calculator e1 * calculator e2
    |DIV(e1, e2) -> calculator e1 / calculator e2
    |SIGMA(INT n1, INT n2, e3) -> if n1 > n2 then 0 else (cal n1 n2 e3) + calculator (SIGMA(INT (n1+1), INT n2, e3))
;; 

(* calculator (SIGMA(INT 1 + INT 2, INT 3, INT 4));; *)