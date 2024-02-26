type exp =
  | X
  | INT of int
  | ADD of exp * exp
  | SUB of exp * exp
  | MUL of exp * exp
  | DIV of exp * exp
  | SIGMA of exp * exp * exp

let rec calc_var : exp -> int -> int
  = fun exp v ->
    match exp with
    | X -> v
    | INT(n) -> n
    | ADD(e1, e2) -> (calc_var e1 v) + (calc_var e2 v)
    | SUB(e1, e2) -> (calc_var e1 v) - (calc_var e2 v)
    | MUL(e1, e2) -> (calc_var e1 v) * (calc_var e2 v)
    | DIV(e1, e2) -> (calc_var e1 v) / (calc_var e2 v)
    | SIGMA(e1, e2, e3) ->
      let l, r = calc_var e1 v, calc_var e2 v in
      if l > r then 0
      else (calc_var e3 l) + (calc_var (SIGMA(INT(l + 1), INT(r), e3)) v);;

let rec calculator : exp -> int
  = fun exp -> 
    match exp with
    | X -> raise (Failure("Unbound Value X"))
    | INT(n) -> n
    | ADD(e1, e2) -> calculator e1 + calculator e2
    | SUB(e1, e2) -> calculator e1 - calculator e2
    | MUL(e1, e2) -> calculator e1 * calculator e2
    | DIV(e1, e2) -> calculator e1 / calculator e2 
    | SIGMA(e1, e2, e3) ->
      let l, r = calculator e1, calculator e2 in
      if l > r then 0
      else (calc_var e3 l) + calculator (SIGMA(INT(l + 1), INT(r), e3));;

calculator (SIGMA(INT 1, INT 10, SUB(MUL(X, X), INT 1)));;
calculator (SIGMA(SIGMA(INT (-2), INT 2, X), SIGMA(INT 1, INT 4, X), MUL(X, MUL(X, X))));;

