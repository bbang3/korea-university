type program = exp
and exp =
  | CONST of int
  | VAR of var
  | ADD of exp * exp
  | SUB of exp * exp
  | EQUAL of exp * exp
  | READ
  | LET of var * exp * exp
  | IF of exp * exp * exp
  | ISZERO of exp
  | PROC of var * exp
  | CALL of exp * exp 
  | LETREC of var * var * exp * exp (* var1 -> fun name, var2 -> formal param *)
and var = string;;

type value = 
  | Int of int
  | Bool of bool
  | Procedure of var * exp * env
  | RecProcedure of var * var * exp * env
and env = var -> value
let empty_env : env = 
  fun var -> raise (Failure (var ^ " is not bound in env"))
let extend_env : var -> value -> env -> env = 
  fun new_var new_value env -> 
    begin 
      fun var -> 
        if var = new_var then new_value
        else env var
    end
let lookup_env : var -> env -> value = fun x env -> env x

(* type env = (var * value) list *)
(* let empty_env = []
   let extend_env (x, v) env = (x, v)::env
   let rec lookup_env x env = 
        match env with
        | [] -> raise (Failure (x ^ " is not bound in env"))
        | (y, v)::tl -> if x = y then v else lookup_env x tl *)

exception UndefinedSemantics

let rec eval : env -> exp -> value
  = fun env exp ->
    match exp with
    | CONST n -> Int n
    | VAR var -> lookup_env var env
    | ADD (exp1, exp2) ->
      let n1 = eval env exp1 in
      let n2 = eval env exp2 in 
      begin
        match n1, n2 with
        | Int(n1), Int(n2) -> Int(n1 + n2)
        | _, _ -> raise UndefinedSemantics
      end
    | SUB (exp1, exp2) ->
      let n1 = eval env exp1 in
      let n2 = eval env exp2 in 
      begin
        match n1, n2 with
        | Int(n1), Int(n2) -> Int(n1 - n2)
        | _, _ -> raise UndefinedSemantics
      end
    | EQUAL (exp1, exp2) ->
      let v1, v2 = eval env exp1, eval env exp2 in
      begin
        match v1, v2 with
        | Int n1, Int n2 -> Bool (n1 = n2)
        | Bool b1, Bool b2 -> Bool (b1 = b2)
        | _, _ -> raise UndefinedSemantics
      end
    | READ -> Int (read_int())
    | ISZERO exp ->
      begin
        let value = eval env exp in
        match value with
        | Int 0 -> Bool true
        | Int _ -> Bool false
        | _ -> raise UndefinedSemantics
      end
    | IF (condition_exp, exp1, exp2) ->
      begin
        let condition = eval env condition_exp in
        match condition with
        | Bool true -> eval env exp1
        | Bool false -> eval env exp2
        | _ -> raise UndefinedSemantics
      end
    | LET (var, value_exp, exp) ->
      let value = eval env value_exp in
      eval (extend_env var value env) exp
    | PROC (fparam, body_exp) -> Procedure(fparam, body_exp, env)
    | LETREC (func_name, fparam, body_exp, exp) ->
      let rec_func = RecProcedure(func_name, fparam, body_exp, env) in
      eval (extend_env func_name rec_func env) exp
    | CALL (func_exp, aparam_exp) ->
      begin
        let aparam = eval env aparam_exp in
        let func = eval env func_exp in
        match func with
        | Procedure(fparam, body, create_env) -> 
          eval (extend_env fparam aparam create_env) body
        | RecProcedure(func_name, fparam, body, create_env) ->
          let local_env = extend_env func_name func 
              (extend_env fparam aparam create_env) in
          eval local_env body
        | _ -> raise UndefinedSemantics
      end
let run : program -> value
  = fun pgm -> eval empty_env pgm


let pgm1 = ADD(CONST 7, CONST 6)
let pgm2 = LET("x", ADD(CONST 1, CONST 2), SUB(VAR "x", CONST 7))

let pgm3 = LET ("x", CONST 1,
                LET ("f", PROC ("y", ADD (VAR "x", VAR "y")),
                     LET ("x", CONST 2,
                          LET ("g", PROC ("y", ADD (VAR "x", VAR "y")),
                               ADD (CALL (VAR "f", CONST 1), CALL (VAR "g", CONST 1))))))

let pgm4 = LETREC ("double", "x",
                   IF (EQUAL (VAR "x", CONST 0), CONST 0,
                       ADD (CALL (VAR "double", SUB (VAR "x", CONST 1)), CONST 2)),
                   CALL (VAR "double", CONST 6))
