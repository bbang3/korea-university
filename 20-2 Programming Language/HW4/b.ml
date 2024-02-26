type exp =
  | NUM of int | TRUE | FALSE | UNIT
  | VAR of id
  | ADD of exp * exp
  | SUB of exp * exp
  | MUL of exp * exp
  | DIV of exp * exp
  | EQUAL of exp * exp
  | LESS of exp * exp
  | NOT of exp
  | SEQ of exp * exp                 (* sequence *)
  | IF of exp * exp * exp            (* if-then-else *)
  | WHILE of exp * exp               (* while loop *)
  | LETV of id * exp * exp           (* variable binding *)
  | LETF of id * id list * exp * exp (* procedure binding *)
  | CALLV of id * exp list           (* call by value *)
  | CALLR of id * id list            (* call by referenece *)
  | RECORD of (id * exp) list        (* record construction *)
  | FIELD of exp * id                (* access record field *)
  | ASSIGN of id * exp               (* assgin to variable *)
  | ASSIGNF of exp * id * exp        (* assign to record field *)
  | WRITE of exp
and id = string

type loc = int
type value =
  | Num of int
  | Bool of bool
  | Unit
  | Record of record 
and record = (id * loc) list
type memory = (loc * value) list
type env = binding list
and binding = LocBind of id * loc | ProcBind of id * proc
and proc = id list * exp * env

(********************************)
(*     Handling environment     *)
(********************************)

let rec lookup_loc_env : id -> env -> loc
  = fun x env ->
    match env with
    | [] -> raise(Failure ("Variable "^x^" is not included in environment"))
    | hd::tl ->
      begin match hd with
        | LocBind (id,l) -> if(x=id) then l else lookup_loc_env x tl
        | ProcBind _ -> lookup_loc_env x tl
      end

let rec lookup_proc_env : id -> env -> proc
  = fun x env ->
    match env with
    | [] -> raise(Failure ("Variable "^x^" is not included in environment"))
    | hd::tl ->
      begin match hd with
        | LocBind _ -> lookup_proc_env x tl
        | ProcBind (id,binding) -> if (x=id) then binding else lookup_proc_env x tl
      end

let extend_env : binding -> env -> env
  = fun e env -> e::env

let empty_env = []


(***************************)
(*     Handling memory     *)
(***************************)

let rec lookup_mem : loc -> memory -> value
  = fun l mem ->
    match mem with
    | [] -> raise(Failure ("location "^(string_of_int l)^" is not included in memory"))
    | (loc,v)::tl -> if(l=loc) then v else lookup_mem l tl

let extend_mem : (loc * value) -> memory -> memory
  = fun (l,v) mem -> (l,v)::mem

let empty_mem = []

(***************************)
(*     Handling record     *)
(***************************)

let rec lookup_record : id -> record -> loc
  = fun id record -> 
    match record with
    | [] -> raise(Failure ("field "^ id ^" is not included in record"))
    | (x,l)::tl -> if(id=x) then l else lookup_record id tl


let extend_record : (id * loc) -> record -> record
  = fun (x,l) record -> (x,l)::record

let empty_record = []

(***************************)

let counter = ref 0
let new_location () = counter:=!counter+1;!counter

exception NotImplemented
exception UndefinedSemantics

let rec list_fold2 : ('a -> 'b -> 'c -> 'c)-> 'a list -> 'b list -> 'c -> 'c
  = fun func l1 l2 acc ->
    match (l1,l2) with
    | ([],[]) -> acc
    | (hd1::tl1,hd2::tl2) -> list_fold2 func tl1 tl2 (func hd1 hd2 acc)
    | _ -> raise (Failure "two lists have different length")

let rec list_fold : ('a -> 'b -> 'b) -> 'a list -> 'b -> 'b
  = fun func l acc ->
    match l with
    | [] -> acc
    | hd::tl -> list_fold func tl (func hd acc)

let value2str : value -> string
  = fun v ->
    match v with
    | Num n -> string_of_int n
    | Bool b -> string_of_bool b
    | Unit -> "unit"
    | Record _ -> "record" 

let rec eval_aop : env -> memory -> exp -> exp -> (int -> int -> int) -> (value * memory)
  = fun env mem e1 e2 op ->
    let (v1,mem1) = eval env mem e1 in
    let (v2,mem2) = eval env mem1 e2 in
    match (v1,v2) with
    | (Num n1, Num n2) -> (Num (op n1 n2), mem2)
    | _ -> raise (Failure "arithmetic operation type error")

and eval : env -> memory -> exp -> (value * memory) 
  =fun env mem e -> 
    match e with
    | WRITE e -> 
      let (v1,mem1) = eval env mem e in
      let _ = print_endline(value2str v1) in
      (v1,mem1)
    | NUM(n) -> (Num n, mem)
    | TRUE -> (Bool true, mem)
    | FALSE -> (Bool false, mem)
    | UNIT -> (Unit, mem)
    | VAR x -> ((lookup_mem (lookup_loc_env x env) mem), mem)
    | ADD (e1, e2) -> eval_aop env mem e1 e2 (+)
    | SUB (e1, e2) -> eval_aop env mem e1 e2 (-)
    | MUL (e1, e2) -> eval_aop env mem e1 e2 ( * )
    | DIV (e1, e2) -> eval_aop env mem e1 e2 (/)
    | EQUAL (e1, e2) ->
      let (v1, mem1) = eval env mem e1 in
      let (v2, mem2) = eval env mem1 e2 in
      begin
        match (v1, v2) with
        | (Num n1, Num n2) -> (Bool (n1 = n2), mem2)
        | (Bool b1, Bool b2) -> (Bool (b1 = b2), mem2)
        | (Unit, Unit) -> (Bool true, mem2)
        | _, _ -> (Bool false, mem2)
      end
    | LESS (e1, e2)->
      let (v1, mem1) = eval env mem e1 in
      let (v2, mem2) = eval env mem1 e2 in
      begin
        match(v1, v2) with
        | (Num n1, Num n2) -> (Bool (n1 < n2), mem2)
        | _, _ -> raise UndefinedSemantics
      end
    | NOT exp ->
      let (v, mem1) = eval env mem exp in
      begin
        match v with
        | Bool b -> (Bool (not b), mem1)
        | _ -> raise UndefinedSemantics
      end
    | SEQ (e1, e2) ->
      let (_, mem1) = eval env mem e1 in
      eval env mem1 e2
    | IF (cond_exp, e1, e2) ->
      let (cond, mem1) = eval env mem cond_exp in
      begin
        match cond with
        | Bool true -> eval env mem1 e1
        | Bool false -> eval env mem1 e2
        | _ -> raise UndefinedSemantics
      end
    | WHILE (cond_exp, exp) ->
      let (cond, mem1) = eval env mem cond_exp in
      begin
        match cond with
        | Bool true -> 
          let (v, mem2) = eval env mem1 exp in
          eval env mem2 (WHILE(cond_exp, exp))
        | Bool false -> (Unit, mem1)
        | _ -> raise UndefinedSemantics
      end
    | LETV (id, value_exp, exp) ->
      let (v, mem1) = eval env mem value_exp in
      let new_loc = new_location () in
      let env' = extend_env (LocBind(id, new_loc)) env in
      let mem2 = extend_mem (new_loc, v) mem1 in
      eval env' mem2 exp
    | LETF (f, f_param_lst, body, exp) ->
      let f_value = ProcBind(f, (f_param_lst, body, env)) in
      let env' = extend_env f_value env in
      eval env' mem exp
    | CALLV (f, value_exp_lst) ->
      let (v_lst, mem1) = get_value_lst_mem value_exp_lst env mem in 
      let (f_param_lst, body, create_env) = lookup_proc_env f env in
      let rec extend_by_param : id list -> value list -> env -> memory -> (env * memory)
        = begin fun id_lst v_lst env mem ->
          match id_lst, v_lst with
          | [], [] -> env, mem
          | id_hd::id_tl, v_hd::v_tl ->
            let new_loc = new_location () in
            let env' = extend_env (LocBind(id_hd, new_loc)) env in
            let mem' = extend_mem (new_loc, v_hd) mem in
            extend_by_param id_tl v_tl env' mem'
          | _, _ -> raise UndefinedSemantics
        end in
      let (create_env1, mem') = extend_by_param f_param_lst v_lst create_env mem1 in
      let create_env' = extend_env (ProcBind(f, (f_param_lst, body, create_env))) create_env1 in
      eval create_env' mem' body
    | CALLR (f, ref_lst) ->
      let (f_param_lst, body, create_env) = lookup_proc_env f env in
      let create_env' = update_env_by_ref f_param_lst ref_lst create_env env in
      let create_env' = extend_env (ProcBind(f, (f_param_lst, body, create_env'))) create_env' in 
      eval create_env' mem body
    | RECORD (field_lst) ->
      let (r, mem') = make_record field_lst env mem in
      begin
        match r with
        | [] -> (Unit, mem')
        | hd::tl -> (Record r, mem')
      end
    | FIELD (record_exp, id) ->
      let (record_value, mem1) = eval env mem record_exp in
      begin
        match record_value with
        | Record(r) -> 
          let l = lookup_record id r in
          ((lookup_mem l mem1) , mem1)
        | _ -> raise UndefinedSemantics
      end
    | ASSIGN (id, value_exp) ->
      let (v, mem1) = eval env mem value_exp in
      let l = lookup_loc_env id env in
      v, (extend_mem (l, v) mem1)
    | ASSIGNF (record_exp, id, value_exp) ->
      let (record_value, mem1) = eval env mem record_exp in
      begin
        match record_value with
        | Record(r) ->
          let (v, mem2) = eval env mem1 value_exp in
          let l = lookup_record id r in
          (Record r, extend_mem (l, v) mem2)
        | _ -> raise UndefinedSemantics
      end
    | _ -> raise NotImplemented
and get_value_lst_mem : exp list -> env -> memory -> (value list * memory)
  = fun value_exp_lst env mem ->
    match value_exp_lst with
    | [] -> ([], mem)
    | hd::tl -> 
      let (v, mem1) = eval env mem hd in
      let (value_lst, mem2) = get_value_lst_mem tl env mem1 in
      (v::value_lst, mem2)
and update_env_by_ref : id list -> id list -> env -> env -> env
  = fun f_param_lst ref_lst create_env call_env ->
    match f_param_lst, ref_lst with
    | [], [] -> create_env
    | f_param_hd::f_param_tl, ref_hd::ref_tl ->
      let ref_loc = lookup_loc_env ref_hd call_env in
      let create_env' = extend_env (LocBind(f_param_hd, ref_loc)) create_env in
      update_env_by_ref f_param_tl ref_tl create_env' call_env
    | _, _ -> raise UndefinedSemantics
and make_record : (id * exp) list -> env -> memory -> (record * memory)
  =  fun field_lst env mem ->
    match field_lst with
    | [] -> (empty_record, mem)
    | (id, value_exp)::tl ->
      let (v, mem1) = eval env mem value_exp in
      let (r, mem2) = make_record tl env mem1 in
      let new_loc = new_location () in
      let mem' = extend_mem (new_loc, v) mem2 in
      (extend_record (id, new_loc) r), mem'         
let runb : exp -> value 
  =fun exp -> let (v, _) = eval empty_env empty_mem exp in v;;


let pgm : exp
  = (LETV("n",NUM 5,
          LETF("f",["x"],
               IF(EQUAL(VAR "n",NUM 0),
                  UNIT,
                  SEQ(SEQ(ASSIGN("x",ADD(VAR "x",NUM 1)), ASSIGN("n",SUB(VAR "n",NUM 1))), CALLR("f",["x"]))
                 ),
               LETV("x",NUM 10,SEQ(CALLR("f",["x"]),VAR "x")))))