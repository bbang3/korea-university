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
and tyvar = string

type typ_eqn = (typ * typ) list

let rec map : ('a -> 'b) -> 'a list -> 'b list
  = fun f lst ->
    match lst with
    | [] -> []
    | hd::tl -> (f hd)::(map f tl)

module TyEnv = struct
  type t = var -> typ
  let empty = fun _ -> raise (Failure "Type Env is empty")
  let extend : (var * typ) -> t -> t
    = fun (x, t) tenv ->
      fun y -> if x = y then t else tenv y
  let lookup : var -> t -> typ
    = fun x tenv -> tenv x
end

module Subst = struct
  type t = (tyvar * typ) list
  let empty = []
  let rec lookup : tyvar -> t -> typ
    = fun x subst -> 
      match subst with
      | [] -> raise (Failure ("Variable " ^ x ^ "is not bound in Subst"))
      | (hdv, hdt)::tl -> if x = hdv then hdt else lookup x tl
  let rec apply : typ -> t -> typ
    = fun typ subst ->
      match typ with
      | TyUnit -> TyUnit
      | TyInt -> TyInt
      | TyBool -> TyBool
      | TyFun(t1, t2) -> TyFun (apply t1 subst, apply t2 subst)
      | TyList t_elem -> TyList (apply t_elem subst)
      | TyVar x ->
        try lookup x subst
        with _ -> typ
  let extend : (tyvar * typ) -> t -> t
    = fun (tyvar, typ) subst -> 
      (tyvar, typ)::(map(fun (x, t) -> (x, apply t [tyvar, typ])) subst)
end


(* You can invoke "fresh_tyvar()" to generate a fresh type variable *)
let tyvar_num = ref 0
let fresh_tyvar () = (tyvar_num := !tyvar_num + 1; (TyVar ("t" ^ string_of_int !tyvar_num)))
(* tyvar for operands of equal is named as e1, e2, ... *)
let fresh_tyvar_equal () = (tyvar_num := !tyvar_num + 1; (TyVar ("e" ^ string_of_int !tyvar_num)))

let rec gen_equations : TyEnv.t -> exp -> typ -> typ_eqn
  = fun tenv exp typ ->
    match exp with
    | UNIT -> [typ, TyUnit]
    | TRUE | FALSE -> [typ, TyBool]
    | CONST n -> [typ, TyInt]
    | VAR x -> begin 
        try
          let t_x = TyEnv.lookup x tenv in
          [typ, t_x]
        with _ -> raise TypeError
      end
    | ADD (e1, e2) | SUB(e1, e2) | MUL (e1, e2) | DIV (e1, e2) -> 
      (typ, TyInt) :: (gen_equations tenv e1 TyInt) @ (gen_equations tenv e2 TyInt)
    | EQUAL (e1, e2) -> 
      let t1 = fresh_tyvar_equal () in
      (typ, TyBool) :: (gen_equations tenv e1 t1) @ (gen_equations tenv e2 t1)
    | LESS (e1, e2) ->
      (typ, TyBool) :: (gen_equations tenv e1 TyInt) @ (gen_equations tenv e2 TyInt)
    | NOT e -> (typ, TyBool)::(gen_equations tenv e TyBool)
    | NIL -> [typ, TyList(fresh_tyvar ())]
    | CONS (hd, tl) ->
      let t_elem = fresh_tyvar () in 
      let eqn_hd = gen_equations tenv hd t_elem in
      let eqn_tl = gen_equations tenv tl (TyList t_elem) in
      (typ, TyList t_elem) :: eqn_hd @ eqn_tl
    | APPEND (l1, l2) ->
      let t_elem = fresh_tyvar () in
      let eqn_l1 = gen_equations tenv l1 (TyList t_elem) in 
      let eqn_l2 = gen_equations tenv l2 (TyList t_elem) in
      (typ, TyList t_elem) :: eqn_l1 @ eqn_l2
    | HEAD (lst) -> 
      let t_elem = fresh_tyvar () in
      let eqn_lst = gen_equations tenv lst (TyList t_elem) in
      (typ, t_elem) :: eqn_lst
    | TAIL (lst) ->
      let t_elem = fresh_tyvar () in
      let eqn_lst = gen_equations tenv lst (TyList t_elem) in
      (typ, TyList t_elem) :: eqn_lst
    | ISNIL (lst) ->
      let t_elem = fresh_tyvar () in
      let eqn_lst = gen_equations tenv lst (TyList t_elem) in 
      (typ, TyBool) :: eqn_lst
    | IF (cond, e1, e2) ->
      let eqn_cond = gen_equations tenv cond TyBool in
      let eqn_e1 = gen_equations tenv e1 typ in
      let eqn_e2 = gen_equations tenv e2 typ in
      eqn_cond @ eqn_e1 @ eqn_e2
    | LET (x, v, exp) ->
      let t1 = fresh_tyvar () in
      let eqn_v = gen_equations tenv v t1 in
      let eqn_exp = gen_equations (TyEnv.extend (x, t1) tenv) exp typ in
      eqn_v @ eqn_exp
    | LETREC (f, x, body, exp) ->
      let t_arg, t_ret = fresh_tyvar(), fresh_tyvar() in
      let tenv = TyEnv.extend (f, TyFun(t_arg, t_ret)) tenv in
      let eqn_body = gen_equations (TyEnv.extend (x, t_arg) tenv) body t_ret in
      let eqn_exp = gen_equations tenv exp typ in
      eqn_body @ eqn_exp
    | LETMREC ((f, x, body_f), (g, y, body_g), exp) ->
      let t_farg, t_fret = fresh_tyvar(), fresh_tyvar() in
      let t_garg, t_gret = fresh_tyvar(), fresh_tyvar() in
      let tenv = TyEnv.extend (g, TyFun(t_garg, t_gret)) (TyEnv.extend (f, TyFun(t_farg, t_fret)) tenv) in
      let tenv_f = TyEnv.extend (x, t_farg) tenv in
      let tenv_g = TyEnv.extend (y, t_garg) tenv in 
      let eqn_fbody = gen_equations tenv_f body_f t_fret in
      let eqn_gbody = gen_equations tenv_g body_g t_gret in
      let eqn_exp = gen_equations tenv exp typ in
      eqn_fbody @ eqn_gbody @ eqn_exp
    | PROC (x, body) ->
      let t_arg, t_ret = fresh_tyvar (), fresh_tyvar () in
      let eqn_body = gen_equations (TyEnv.extend (x, t_arg) tenv) body t_ret in
      (typ, TyFun(t_arg, t_ret))::eqn_body
    | CALL (f, arg) ->
      let t_arg = fresh_tyvar () in
      let eqn_f = gen_equations tenv f (TyFun(t_arg, typ)) in
      let eqn_arg = gen_equations tenv arg t_arg in
      eqn_f @ eqn_arg
    | PRINT e -> 
      let t1 = fresh_tyvar () in
      (typ, TyUnit) :: (gen_equations tenv e t1) 
    | SEQ (e1, e2) ->
      let t1 = fresh_tyvar () in
      (gen_equations tenv e1 t1) @ (gen_equations tenv e2 typ)

(* check whether given tyvar occurs in type expression *)
let rec occurence_check : tyvar -> typ -> bool (**)
  =fun x t ->
    match t with
    | TyUnit | TyBool | TyInt -> false
    | TyVar y -> (x = y)
    | TyFun (t1, t2) -> (occurence_check x t1) || (occurence_check x t2)
    | TyList (t1) -> occurence_check x t1

(* move single type equation to current substitution *)
let rec unify : (typ * typ) -> Subst.t -> Subst.t
  = fun (lhs, rhs) subst ->
    let lhs = Subst.apply lhs subst in
    let rhs = Subst.apply rhs subst in 
    match (lhs, rhs) with
    | TyUnit, TyUnit | TyInt, TyInt | TyBool, TyBool -> subst
    | TyVar x, TyVar y -> if x = y then subst else Subst.extend (x, TyVar y) subst 
    | TyVar x, t -> 
      if occurence_check x t then raise TypeError
      else Subst.extend (x, t) subst
    | t, TyVar x -> unify (rhs, lhs) subst
    | TyFun(a, b), TyFun(a', b') ->
      let subst = unify(a, a') subst in
      let subst = unify(Subst.apply b subst, Subst.apply b' subst) subst in
      subst
    | TyList a, TyList b -> unify(a, b) subst
    | _, _ -> raise TypeError

let rec unifyall : typ_eqn -> Subst.t -> Subst.t
  = fun tyeqn subst ->
    match tyeqn with
    | [] -> subst
    | (t1, t2)::tl -> 
      (* let t1 = Subst.apply t1 subst in *)
      (* let t2 = Subst.apply t2 subst in *)
      let subst = unify (t1, t2) subst in
      unifyall tl subst

let rec forall p lst = 
  match lst with 
  |[] -> true 
  | hd::tl -> p hd && forall p tl

let rec check_eq_tyvar : (tyvar * typ) -> bool
  = fun (tv, ty) ->
    if tv.[0] = 'e' then 
      begin
        match ty with
        | TyBool -> true
        | TyInt -> true
        | TyVar _ -> true
        | _ -> false
      end
    else true

let typeof : exp -> typ 
  = fun exp -> 
    (* tyvar_num := 0; *)
    let t = fresh_tyvar () in
    let eqns = gen_equations TyEnv.empty exp t in
    let subst = unifyall eqns Subst.empty in
    if forall check_eq_tyvar subst then Subst.apply t subst
    else raise TypeError