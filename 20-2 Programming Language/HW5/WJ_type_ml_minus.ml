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

exception TypeError

type typ = 
    TyUnit 
  | TyInt 
  | TyBool 
  | TyFun of typ * typ 
  | TyList of typ
  | TyVar of tyvar
  | NotFound
  | TyEql
and tyvar = string


(* You can invoke "fresh_tyvar()" to generate a fresh type variable *)
let tyvar_num = ref 0
let fresh_tyvar () = (tyvar_num := !tyvar_num + 1; (TyVar ("t" ^ string_of_int !tyvar_num)))

let empty_type_env = []
let extend_type_env (x,v) env = (x,v)::env
let rec lookup_type_env x env = 
  match env with
  | [] -> raise (Failure ("variable " ^ x ^ " is not bound in env")) 
  | (y,v)::tl -> if x = y then v else lookup_type_env x tl

let empty_subst = []
let rec lookup_subst x subst =
  match subst with
  | [] -> NotFound
  | (y, type_val)::tl -> if x = y then type_val else lookup_subst x tl

let rec type_eq_gen : (var * typ) list -> exp -> typ -> (typ * typ) list
  = fun type_env e result_type -> 
    match e with
    | UNIT -> [(result_type, TyUnit)]
    | TRUE -> [(result_type, TyBool)]
    | FALSE -> [(result_type, TyBool)]
    | CONST n -> [(result_type, TyInt)]
    | VAR x -> [(result_type, lookup_type_env x type_env)]
    | ADD (e1, e2) -> (result_type, TyInt)::((type_eq_gen type_env e1 TyInt) @ (type_eq_gen type_env e2 TyInt))
    | SUB (e1, e2) -> (result_type, TyInt)::((type_eq_gen type_env e1 TyInt) @ (type_eq_gen type_env e2 TyInt))
    | MUL (e1, e2) -> (result_type, TyInt)::((type_eq_gen type_env e1 TyInt) @ (type_eq_gen type_env e2 TyInt))
    | DIV (e1, e2) -> (result_type, TyInt)::((type_eq_gen type_env e1 TyInt) @ (type_eq_gen type_env e2 TyInt))
    | EQUAL (e1, e2) -> let e_type = fresh_tyvar() in
      (result_type, TyBool)::((e_type, TyEql)::((type_eq_gen type_env e1 e_type) @ (type_eq_gen type_env e2 e_type)))
    | LESS (e1, e2) -> (result_type, TyBool)::((type_eq_gen type_env e1 TyInt) @ (type_eq_gen type_env e2 TyInt))
    | NOT sube -> (result_type, TyBool)::(type_eq_gen type_env sube TyBool)
    | NIL -> let element_type = fresh_tyvar() in
      [(result_type, TyList element_type)]
    | CONS (element, lst) -> let element_type = fresh_tyvar() in
      (result_type, (TyList element_type))::((type_eq_gen type_env element element_type) @ (type_eq_gen type_env lst (TyList element_type)))
    | APPEND (lst1, lst2) -> let element_type = fresh_tyvar() in
      (result_type, (TyList element_type))::((type_eq_gen type_env lst1 (TyList element_type)) @ (type_eq_gen type_env lst2 (TyList element_type)))
    | HEAD lst -> (type_eq_gen type_env lst (TyList result_type))
    | TAIL lst -> let element_type = fresh_tyvar() in
      (result_type, (TyList element_type))::(type_eq_gen type_env lst (TyList element_type))
    | ISNIL lst -> let element_type = fresh_tyvar() in
      (result_type, TyBool)::(type_eq_gen type_env lst (TyList element_type))
    | IF (cond, e1, e2) -> ((type_eq_gen type_env cond TyBool) @ (type_eq_gen type_env e1 result_type) @ (type_eq_gen type_env e2 result_type))
    | LET (x, x_val, sube) -> let x_type = fresh_tyvar() in
      let extended_env = extend_type_env (x, x_type) type_env in
      (type_eq_gen type_env x_val x_type) @ (type_eq_gen extended_env sube result_type)
    | LETREC (f, x, body, sube) -> let arg_type = fresh_tyvar() in
      let arg_extended_env = extend_type_env (x, arg_type) type_env in
      let body_type = fresh_tyvar() in
      let fun_type = TyFun(arg_type, body_type) in
      let fun_extended_env = extend_type_env (f, fun_type) type_env in
      let body_eval_env = extend_type_env (f, fun_type) arg_extended_env in
      (type_eq_gen body_eval_env body body_type) @ (type_eq_gen fun_extended_env sube result_type)
    | LETMREC ((f, x, fbody), (g, y, gbody), sube) -> let farg_type = fresh_tyvar() in
      let farg_extended_env = extend_type_env (x, farg_type) type_env in
      let fbody_type = fresh_tyvar() in
      let f_type = TyFun(farg_type, fbody_type) in
      let f_extended_env = extend_type_env (f, f_type) type_env in
      let garg_type = fresh_tyvar() in
      let garg_extended_env = extend_type_env (y, garg_type) type_env in
      let gbody_type = fresh_tyvar() in
      let g_type = TyFun(garg_type, gbody_type) in
      let fbody_eval_env = extend_type_env (g, g_type) farg_extended_env in
      let gbody_eval_env = extend_type_env (f, f_type) garg_extended_env in
      let full_extended_env = extend_type_env (g, g_type) f_extended_env in
      (type_eq_gen fbody_eval_env fbody fbody_type) @ (type_eq_gen gbody_eval_env gbody gbody_type) @ (type_eq_gen full_extended_env sube result_type)
    | PROC (x, body) -> let arg_type = fresh_tyvar() in
      let extended_env = extend_type_env (x, arg_type) type_env in
      let body_type = fresh_tyvar() in
      (result_type, TyFun(arg_type, body_type))::(type_eq_gen extended_env body body_type)
    | CALL (f, arg) -> let arg_type = fresh_tyvar() in
      (type_eq_gen type_env f (TyFun(arg_type, result_type))) @ (type_eq_gen type_env arg arg_type)
    | PRINT content -> let content_type = fresh_tyvar() in
      (result_type, TyUnit)::(type_eq_gen type_env content content_type)
    | SEQ (e1, e2) -> let e1_type = fresh_tyvar() in
      (type_eq_gen type_env e1 e1_type) @ (type_eq_gen type_env e2 result_type)

let rec apply : typ -> (tyvar * typ) list -> typ
  = fun typ subst ->
    match typ with 
    | TyUnit -> TyUnit
    | TyInt -> TyInt
    | TyBool -> TyBool
    | TyFun (arg, result) -> TyFun((apply arg subst), (apply result subst))
    | TyList element_type -> TyList(apply element_type subst)
    | TyVar x -> let value = lookup_subst x subst in
      begin
        match value with
        | NotFound -> TyVar x
        | _ -> value
      end
    | TyEql -> TyEql

let rec apply_to_substitution : (tyvar * typ) -> (tyvar * typ) list -> (tyvar * typ) list
  = fun new_eq subst -> 
    match subst with
    | [] -> subst
    | (x, v)::tl -> (x, (apply v [new_eq]))::(apply_to_substitution new_eq tl)

let rec rearrange_eq_set : (typ * typ) list -> (typ * typ) list
  = fun eq_set ->
    match eq_set with
    | [] -> []
    | (TyEql, v)::tl -> (rearrange_eq_set tl) @ [(TyEql, v)]
    | (v, TyEql)::tl -> (rearrange_eq_set tl) @ [(v, TyEql)]
    | hd::tl -> hd::(rearrange_eq_set tl)

(* return value: true if variable occurs in type_val(program rejected), false otherwise *)
let rec occurence_check : tyvar -> typ -> bool
  = fun x type_val ->
    match type_val with
    | TyVar y -> x = y
    | TyList t -> occurence_check x t
    | TyFun (t1, t2) -> (occurence_check x t1) || (occurence_check x t2)
    | _ -> false

let extend_subst : (tyvar * typ) -> (tyvar * typ) list -> (tyvar * typ) list
  = fun (x, v) subst ->
    let applied_eq = (x, apply v subst) in
    let applied_subst = apply_to_substitution applied_eq subst in
    applied_eq::applied_subst

let rec solve_unit : (typ * typ) -> (tyvar * typ) list -> (tyvar * typ) list
  = fun (typ1, typ2) subst ->
    let eq = ((apply typ1 subst), (apply typ2 subst)) in
    match eq with
    | (TyUnit, TyUnit) -> subst
    | (TyInt, TyInt) -> subst
    | (TyBool, TyBool) -> subst
    | (TyList t1, TyList t2) -> solve_unit (t1, t2) subst
    | (TyFun (t1, t2), TyFun (t1', t2')) -> let subst1 = solve_unit (t1, t1') subst in
      solve_unit ((apply t2 subst1), (apply t2' subst1)) subst1
    | (TyVar x, TyVar y) -> if x = y then subst else (extend_subst (x, TyVar y) subst)
    | (TyVar x, type_val) -> if (occurence_check x type_val) then (raise TypeError) else (extend_subst (x, type_val) subst)
    | (type_val, TyVar x) -> solve_unit (TyVar x, type_val) subst
    | (TyEql, type_val) ->
      begin
        match type_val with
        | TyInt -> subst
        | TyBool -> subst
        | _ -> raise TypeError
      end
    | (type_val, TyEql) -> 
      begin
        match type_val with
        | TyInt -> subst
        | TyBool -> subst
        | _ -> raise TypeError
      end
    | (_, _) -> raise TypeError

let rec solve : (typ * typ) list -> (tyvar * typ) list -> (tyvar * typ) list
  = fun eq_set subst -> 
    match eq_set with
    | [] -> subst
    | hd::tl -> let new_subst = solve_unit hd subst in
      solve tl new_subst

let solve_wrapper : (typ * typ) list -> (tyvar * typ) list -> (tyvar * typ) list
  = fun eq_set subst ->
    let rearranged_eq_set = rearrange_eq_set eq_set in
    solve rearranged_eq_set empty_subst

let typeof : exp -> typ 
  =fun exp -> let result_type = fresh_tyvar() in
    let equation_set = type_eq_gen empty_type_env exp result_type in
    let solved_equation_set = solve_wrapper equation_set empty_subst in
    match result_type with
    | TyVar x -> lookup_subst x solved_equation_set
    | _ -> raise (Failure "Sth went wrong with typeof...");;

typeof (
  LETREC ("reverse", "l",
          IF (ISNIL (VAR "l"), NIL,
              APPEND (CALL (VAR "reverse", TAIL (VAR "l")), CONS (HEAD (VAR "l"), NIL))),
          CALL (VAR "reverse",
                CONS (CONS (CONST 1, NIL),
                      CONS (CONS (CONST 2, NIL), CONS (CONS (CONST 3, NIL), NIL)))))
);;