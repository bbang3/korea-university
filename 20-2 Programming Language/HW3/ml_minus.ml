type program = exp
and exp = 
  | UNIT
  | TRUE
  | FALSE
  | CONST of int
  | VAR of var
  | ADD of exp * exp
  | SUB of exp * exp
  | MUL of exp * exp
  | DIV of exp * exp
  | EQUAL of exp * exp
  | LESS of exp * exp
  | NOT of exp
  | NIL
  | CONS of exp * exp      
  | APPEND of exp * exp
  | HEAD of exp
  | TAIL of exp
  | ISNIL of exp
  | IF of exp * exp * exp
  | LET of var * exp * exp
  | LETREC of var * var * exp * exp
  | LETMREC of (var * var * exp) * (var * var * exp) * exp
  | PROC of var * exp
  | CALL of exp * exp
  | PRINT of exp
  | SEQ of exp * exp
and var = string

type value = 
  | Unit 
  | Int of int 
  | Bool of bool 
  | List of value list
  | Procedure of var * exp * env 
  | RecProcedure of var * var * exp * env
  | MRecProcedure of (var * var * exp) * (var * var * exp) * env
and env = (var * value) list

let rec fold_left : ('a -> 'b -> 'a) -> 'a -> 'b list -> 'a
  = fun f accu lst ->
    match lst with
    | [] -> accu
    | hd::tl -> fold_left f (f accu hd) tl

let rec map : ('a -> 'b) -> 'a list -> 'b list
  = fun f lst ->
    match lst with
    | [] -> []
    | hd::tl -> (f hd)::(map f tl)

let rec string_of_value v = 
  match v with
  | Int n -> string_of_int n
  | Bool b -> string_of_bool b
  | List lst -> "[" ^ fold_left (fun s x -> s ^ ", " ^ x) "" (map string_of_value lst) ^ "]"
  | _ -> "(functional value)"

exception UndefinedSemantics

let empty_env = []
let extend_env (x,v) e = (x,v)::e
let rec lookup_env x e = 
  match e with
  | [] -> raise (Failure ("variable " ^ x ^ " is not bound in env")) 
  | (y,v)::tl -> if x = y then v else lookup_env x tl

let rec eval : exp -> env -> value
  = fun exp env ->
    match exp with
    | PRINT e -> (print_endline (string_of_value (eval e env)); Unit)
    | UNIT -> Unit
    | TRUE -> Bool true
    | FALSE -> Bool false
    | CONST n -> Int n
    | VAR v -> lookup_env v env
    | ADD (e1, e2) ->
      begin
        match eval e1 env, eval e2 env with
        | Int(n1), Int(n2) -> Int(n1 + n2)
        | _, _ -> raise UndefinedSemantics
      end
    | SUB (e1, e2) ->
      begin
        match eval e1 env, eval e2 env with
        | Int(n1), Int(n2) -> Int(n1 - n2)
        | _, _ -> raise UndefinedSemantics
      end
    | MUL (e1, e2) ->
      begin
        match eval e1 env, eval e2 env with
        | Int(n1), Int(n2) -> Int(n1 * n2)
        | _, _ -> raise UndefinedSemantics
      end
    | DIV (e1, e2) ->
      begin
        match eval e1 env, eval e2 env with
        | _, Int(0) -> raise UndefinedSemantics
        | Int(n1), Int(n2) -> Int(n1 / n2)
        | _, _ -> raise UndefinedSemantics
      end
    | EQUAL (e1, e2) ->
      begin
        match eval e1 env, eval e2 env with
        | Int n1, Int n2 -> Bool (n1 = n2)
        | Bool b1, Bool b2 -> Bool (b1 = b2)
        | _, _ -> raise UndefinedSemantics
      end
    | LESS (e1, e2) ->
      begin
        match eval e1 env, eval e2 env with
        | Int n1, Int n2 -> Bool (n1 < n2)
        | _, _ -> raise UndefinedSemantics
      end
    | NOT e ->
      begin
        match eval e env with
        | Bool true -> Bool false
        | Bool false -> Bool true
        | _ -> raise UndefinedSemantics
      end
    | NIL -> List []
    | CONS(hd_exp, tl_exp) ->
      begin
        match eval hd_exp env, eval tl_exp env with
        | hd, List tl -> List (hd::tl)
        | _, _ -> raise UndefinedSemantics
      end
    | APPEND(l1_exp, l2_exp) ->
      begin
        match eval l1_exp env, eval l2_exp env with
        | List l1, List l2 -> List (l1@l2)
        | _, _ -> raise UndefinedSemantics
      end
    | HEAD lst_exp ->
      begin
        match eval lst_exp env with
        | List (hd::tl) -> hd
        | _ -> raise UndefinedSemantics
      end
    | TAIL lst_exp ->
      begin
        match eval lst_exp env with
        | List (hd::tl) -> List tl
        | _ -> raise UndefinedSemantics
      end
    | ISNIL lst_exp ->
      begin
        match eval lst_exp env with
        | List [] -> Bool true
        | List (hd :: tl) -> Bool false
        | _ -> raise UndefinedSemantics
      end
    | IF (condition_exp, e1, e2) ->
      begin
        let condition = eval condition_exp env in
        match condition with
        | Bool true -> eval e1 env
        | Bool false -> eval e2 env
        | _ -> raise UndefinedSemantics
      end
    | LET (var, value_exp, exp) ->
      let value = eval value_exp env in
      let env' = extend_env (var, value) env in
      eval exp env'
    | LETREC (f, x, body, exp) ->
      let rfunc_value = RecProcedure (f, x, body, env) in
      let env' = extend_env (f, rfunc_value) env in
      eval exp env'
    | LETMREC ((f, x, f_body), (g, y, g_body), exp) ->
      let f_mrfunc_value = MRecProcedure((f, x, f_body), (g, y, g_body), env) in
      let g_mrfunc_value = MRecProcedure((g, y, g_body), (f, x, f_body), env) in
      let tmp_env = extend_env (f, f_mrfunc_value) env in
      let env' = extend_env (g, g_mrfunc_value) tmp_env in
      eval exp env'
    | PROC (f_param, body_exp) -> Procedure(f_param, body_exp, env)
    | CALL (func_exp, a_param_exp) ->
      let a_param = eval a_param_exp env in
      let func = eval func_exp env in
      begin
        match func with
        | Procedure(f_param, body, create_env) ->
          let create_env' = extend_env (f_param, a_param) create_env in
          eval body create_env'
        | RecProcedure(func_name, f_param, body, create_env) ->
          let tmp_env = extend_env (func_name, func) create_env in
          let create_env'  = extend_env (f_param, a_param) tmp_env in
          eval body create_env'
        | MRecProcedure((f, x, f_body), (g, y, g_body), create_env) ->
          let f_mrfunc_value = MRecProcedure((f, x, f_body), (g, y, g_body), create_env) in
          let g_mrfunc_value = MRecProcedure((g, y, g_body), (f, x, f_body), create_env) in
          let tmp_env1 = extend_env (f, f_mrfunc_value) create_env in
          let tmp_env2 = extend_env (g, g_mrfunc_value) tmp_env1 in
          let create_env' = extend_env (x, a_param) tmp_env2 in
          eval f_body create_env'
        | _ -> raise UndefinedSemantics
      end
    | SEQ (e1, e2) ->
      let v1 = eval e1 env in
      eval e2 env;;

let runml : program -> value
  =fun pgm -> eval pgm empty_env

let pgm1 = 
  LET ("x", CONST 1,
       LET ("f", PROC ("y", ADD (VAR "x", VAR "y")),
            LET ("x", CONST 2,
                 LET ("g", PROC ("y", ADD (VAR "x", VAR "y")),
                      ADD (CALL (VAR "f", CONST 1), CALL (VAR "g", CONST 1))))));;
let pgm2 = 
  LETREC ("double", "x",
          IF (EQUAL (VAR "x", CONST 0), CONST 0,
              ADD (CALL (VAR "double", SUB (VAR "x", CONST 1)), CONST 2)),
          CALL (VAR "double", CONST 6));;

let pgm3 = 
  LETMREC
    (("even", "x",
      IF (EQUAL (VAR "x", CONST 0), TRUE,
          CALL (VAR "odd", SUB (VAR "x", CONST 1)))),
     ("odd", "x",
      IF (EQUAL (VAR "x", CONST 0), FALSE,
          CALL (VAR "even", SUB (VAR "x", CONST 1)))),
     CALL (VAR "odd", CONST 13));;

let pgm4 = 
  LETREC ("factorial", "x",
          IF (EQUAL (VAR "x", CONST 0), CONST 1,
              MUL (CALL (VAR "factorial", SUB (VAR "x", CONST 1)), VAR "x")),
          LETREC ("loop", "n",
                  IF (EQUAL (VAR "n", CONST 0), UNIT,
                      SEQ (PRINT (CALL (VAR "factorial", VAR "n")),
                           CALL (VAR "loop", SUB (VAR "n", CONST 1)))),
                  CALL (VAR "loop", CONST 10)));;

let pgm5 = 
  LETREC ("range", "n",
          IF (EQUAL (VAR "n", CONST 1), CONS (CONST 1, NIL),
              CONS (VAR "n", CALL (VAR "range", SUB (VAR "n", CONST 1)))),
          CALL (VAR "range", CONST 10));;

let pgm6 = 
  LETREC ("reverse", "l",
          IF (ISNIL (VAR "l"), NIL,
              APPEND (CALL (VAR "reverse", TAIL (VAR "l")), CONS (HEAD (VAR "l"), NIL))),
          CALL (VAR "reverse", CONS (CONST 1, CONS (CONST 2, CONS (CONST 3, NIL)))));;

let pgm7_1 = 
  LET ("fix",
       PROC ("f",
             CALL
               (PROC ("x",
                      CALL (VAR "f", PROC ("y", CALL (CALL (VAR "x", VAR "x"), VAR "y")))),
                PROC ("x",
                      CALL (VAR "f", PROC ("y", CALL (CALL (VAR "x", VAR "x"), VAR "y")))))),
       LET ("f",
            CALL (VAR "fix",
                  PROC ("f",
                        PROC ("x",
                              IF (EQUAL (VAR "x", CONST 0), CONST 1,
                                  MUL (CALL (VAR "f", SUB (VAR "x", CONST 1)), VAR "x"))))),
            CALL (VAR "f", CONST 10)));;

let pgm7_2 =
  LET ("fix",
       PROC ("f",
             CALL
               (PROC ("x",
                      CALL (VAR "f", PROC ("y", CALL (CALL (VAR "x", VAR "x"), VAR "y")))),
                PROC ("x",
                      CALL (VAR "f", PROC ("y", CALL (CALL (VAR "x", VAR "x"), VAR "y")))))),
       LET ("f",
            CALL (VAR "fix",
                  PROC ("range",
                        PROC ("n",
                              IF (EQUAL (VAR "n", CONST 1), CONS (CONST 1, NIL),
                                  CONS (VAR "n", CALL (VAR "range", SUB (VAR "n", CONST 1))))))),
            CALL (VAR "f", CONST 10)));;

runml pgm1;;
runml pgm2;;
runml pgm3;;
runml pgm4;;
runml pgm5;;
runml pgm6;;
runml pgm7_1;;
runml pgm7_2;;