type program = exp
and exp =
  | CONST of int
  | VAR of var
  | ADD of exp * exp
  | SUB of exp * exp
  | READ
  | LET of var * exp * exp
  | IF of exp * exp * exp
  | ISZERO of exp
  | PROC of var * exp
  | CALL of exp * exp 
and var = string;;

type value = Int of int | Bool of bool
type env = var -> value

let empty_env : env = 
  fun var -> raise (Failure (var ^ " is not bound in env"))
let extend_env : var -> value -> env -> env = 
  fun new_var new_value env -> 
  fun var -> if var = new_var then new_value else env var
let lookup_env : var -> env -> value = fun x env -> env x

exception UndefinedSemantics
(* type env = (var * value) list *)
(* let empty_env = []
   let extend_env (x, v) env = (x, v)::env
   let rec lookup_env x env = 
        match env with
        | [] -> raise (Failure (x ^ " is not bound in env"))
        | (y, v)::tl -> if x = y then v else lookup_env x tl *)
