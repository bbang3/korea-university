type exp = 
  | V of var
  | P of var * exp
  | C of exp * exp
and var = string

let rec lookup_env : var list -> var -> bool =
  fun env v ->
    match env with
    | [] -> false
    | hd::tl -> if v = hd then true else lookup_env tl v;;


let rec extend_env : var list -> var -> var list =
  fun env v -> v::env;;

let rec check_with_env : var list -> exp -> bool =
  fun env exp ->
    match exp with
    | V(v) -> lookup_env env v
    | P(v, e) -> check_with_env (extend_env env v) e
    | C(e1, e2) -> (check_with_env env e1) && (check_with_env env e2);;

let rec check : exp -> bool
  = fun exp -> check_with_env [] exp;;

check(P("a", V "a"));;
check(P("a", P ("a", V "a")));;
check(P("a", P ("b", C (V "a", V "b"))));;
check(P("a", C (V "a", P ("b", V "a"))));;
check(P("a", V "b"));;
check(P("a", C (V "a", P ("b", V "c"))));;
check(P("a", P ("b", C (V "a", V "c"))));;