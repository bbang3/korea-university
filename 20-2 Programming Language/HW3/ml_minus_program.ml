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
  | MRecProcedure of var * var * exp * var * var * exp * env
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
  =fun exp env ->
    match exp with
    | UNIT -> Unit
    | TRUE -> Bool true
    | FALSE -> Bool false
    | CONST n -> Int n
    | VAR v -> lookup_env v env
    | ADD (a, b) -> let v1 = eval a env in 
      let v2 = eval b env in
      (match (v1, v2) with 
       | Int n1, Int n2 -> Int (n1 + n2)
       | _ -> raise UndefinedSemantics)
    | SUB (a, b) -> let v1 = eval a env in 
      let v2 = eval b env in
      (match (v1, v2) with 
       | Int n1, Int n2 -> Int (n1 - n2)
       | _ -> raise UndefinedSemantics)
    | MUL (a, b) -> let v1 = eval a env in 
      let v2 = eval b env in
      (match v1, v2 with 
       | Int n1, Int n2 -> Int (n1 * n2)
       | _ -> raise UndefinedSemantics)
    | DIV (a, b) -> let v1 = eval a env in 
      let v2 = eval b env in
      (match v1, v2 with 
       | Int n1, Int n2 -> Int (n1 / n2)
       | _ -> raise UndefinedSemantics)
    | EQUAL (a, b) -> let v1 = eval a env in 
      let v2 = eval b env in
      (match (v1, v2) with 
       | Int n1, Int n2 -> if n1 = n2 then Bool true
         else Bool false
       | Bool b1, Bool b2 -> if b1 = b2 then Bool true
         else Bool false
       | _ -> raise (Failure "1"))
    | LESS (a, b) -> let v1 = eval a env in 
      let v2 = eval b env in
      (match (v1, v2) with 
       | Int n1, Int n2 -> if n1 < n2 then Bool true
         else Bool false
       | _ -> raise (Failure "2"))
    | NOT e -> let v1 = eval e env in
      (match v1 with
       | Bool b -> if b = true then Bool false
         else Bool true
       | _ -> raise (Failure "3"))
    | NIL -> List []
    | CONS (a, b) -> let v1 = eval a env in
      let v2 = eval b env in 
      (match v2 with
       | List list -> List ((v1) :: list)
       | _ -> raise (Failure "4"))
    | APPEND (a, b) -> let v1 = eval a env in
      let v2 = eval b env in 
      (match (v1, v2) with
       | List l1, List l2 -> List (l1 @ l2)
       | _ -> raise (Failure "5"))
    | HEAD e -> let v1 = eval e env in
      (match v1 with
       | List list ->
         (match list with
          | hd :: tl -> hd
          | _ -> raise (Failure "6"))
       | _ -> raise (Failure "7"))
    | TAIL e -> let v1 = eval e env in
      (match v1 with
       | List list ->
         (match list with
          | hd :: tl -> List tl
          | _ -> raise (Failure "8"))
       | _ -> raise (Failure "9"))
    | ISNIL e -> let v1 = eval e env in
      (match v1 with
       | List [] -> Bool true
       | _ -> Bool false)
    | IF (a, b, c) -> let v1 = eval a env in
      (match v1 with
       | Bool bl -> if bl = true then eval b env
         else eval c env
       | _ -> raise (Failure "10"))
    (* let x = E in E *)
    | LET (a, b, c) -> let v1 = eval b env in
      let p2 = extend_env (a, v1) env in
      eval c p2
    (* letrec f(x) = E in E *)
    | LETREC (v1, v2, e1, e2) -> let p2 = extend_env (v1, RecProcedure (v1, v2, e1, env)) env in
      eval e2 p2
    (* Mutual recursion, so that letrec f(x1) = E1 and g(x2) = E2 in E *)
    | LETMREC ((v1, v2, e1), (v3, v4, e2), e3) -> let p2 = extend_env (v1, MRecProcedure (v1, v2, e1, v3, v4, e2, env)) env in
      let p3 = extend_env (v3, MRecProcedure (v3, v4, e2, v1, v2, e1, env)) p2 in
      eval e3 p3
    | PROC (v1, e1) -> Procedure (v1, e1, env)
    | CALL (a, b) -> let v1 = eval a env in
      let v2 = eval b env in
      (match v1 with
       | Procedure (v3, c, p2) -> let p3 = extend_env (v3, v2) p2 in
         eval c p3
       | RecProcedure (f1, v3, c, p2) -> let p3 = extend_env (v3, v2) p2 in
         let p4 = extend_env (f1, RecProcedure (f1, v3, c, p2)) p3 in
         eval c p4
       | MRecProcedure (f1, v3, c, g1, v4, d, p2) -> let p3 = extend_env (v3, v2) p2 in
         let p4 = extend_env (f1, MRecProcedure (f1, v3, c, g1, v4, d, p2)) p3 in
         let p5 = extend_env (g1, MRecProcedure (g1, v4, d, f1, v3, c, p2)) p4 in
         eval c p5
       | _ -> raise (Failure "11"))
    | PRINT e -> (print_endline (string_of_value (eval e env)); Unit)
    | SEQ (a, b) -> let v1 = eval a env in
      let v2 = eval b env in
      v2


let runml : program -> value
  =fun pgm -> eval pgm empty_env 
;;

runml(LET ("x", CONST 1,
           LET ("f", PROC ("y", ADD (VAR "x", VAR "y")),
                LET ("x", CONST 2,
                     LET ("g", PROC ("y", ADD (VAR "x", VAR "y")),
                          ADD (CALL (VAR "f", CONST 1), CALL (VAR "g", CONST 1)))))));;


runml(LETREC ("double", "x",
              IF (EQUAL (VAR "x", CONST 0), CONST 0,
                  ADD (CALL (VAR "double", SUB (VAR "x", CONST 1)), CONST 2)),
              CALL (VAR "double", CONST 6)));;

runml(LETMREC
        (("even", "x",
          IF (EQUAL (VAR "x", CONST 0), TRUE,
              CALL (VAR "odd", SUB (VAR "x", CONST 1)))),
         ("odd", "x",
          IF (EQUAL (VAR "x", CONST 0), FALSE,
              CALL (VAR "even", SUB (VAR "x", CONST 1)))),
         CALL (VAR "odd", CONST 13)));;

runml(LETREC ("factorial", "x",
              IF (EQUAL (VAR "x", CONST 0), CONST 1,
                  MUL (CALL (VAR "factorial", SUB (VAR "x", CONST 1)), VAR "x")),
              LETREC ("loop", "n",
                      IF (EQUAL (VAR "n", CONST 0), UNIT,
                          SEQ (PRINT (CALL (VAR "factorial", VAR "n")),
                               CALL (VAR "loop", SUB (VAR "n", CONST 1)))),
                      CALL (VAR "loop", CONST 10))));;

runml(LETREC ("range", "n",
              IF (EQUAL (VAR "n", CONST 1), CONS (CONST 1, NIL),
                  CONS (VAR "n", CALL (VAR "range", SUB (VAR "n", CONST 1)))),
              CALL (VAR "range", CONST 10)));;

runml(LETREC ("reverse", "l",
              IF (ISNIL (VAR "l"), NIL,
                  APPEND (CALL (VAR "reverse", TAIL (VAR "l")), CONS (HEAD (VAR "l"), NIL))),
              CALL (VAR "reverse", CONS (CONST 1, CONS (CONST 2, CONS (CONST 3, NIL))))));;

runml(LET ("fix",
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
                CALL (VAR "f", CONST 10))));;

runml(LET ("fix",
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
                CALL (VAR "f", CONST 10))));;