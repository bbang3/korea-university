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

let rec generate_value_list : exp list -> value list -> env -> memory -> (env -> memory -> exp -> (value * memory)) -> (value list * memory)
= fun elist vlist env mem evalf ->
    match elist with
      | [] -> (vlist, mem)
      | hd::tl -> let (v, mem1) = evalf env mem hd in
                    generate_value_list tl (vlist @ [v]) env mem1 evalf
                  
let rec arg_binding_val_aux : id list -> value list -> env -> memory -> (env * memory)
  = fun idlst valuelst env mem ->
      match (idlst, valuelst) with
        | ([], []) -> (env, mem)
        | (idhd::idtl, valhd::valtl) -> let new_loc = new_location() in
                                        arg_binding_val_aux idtl valtl (extend_env (LocBind (idhd, new_loc)) env) (extend_mem (new_loc, valhd) mem)
        | _ -> raise (Failure "Not enough arguments")
  
let arg_binding_val : id list -> exp list -> env -> memory -> (env -> memory -> exp -> (value * memory)) -> (env * memory)
= fun arg_id_lst arg_val_lst env mem evalf ->
    let (valuelst, mem1) = generate_value_list arg_val_lst [] env mem evalf in
      arg_binding_val_aux arg_id_lst valuelst env mem1
      
let rec arg_binding_loc : id list -> id list -> env -> env -> env
= fun fun_arg_list fed_arg_list fun_env curr_env ->
    match (fun_arg_list, fed_arg_list) with
      | ([], []) -> fun_env
      | (funargid::funargtl, fedargid::fedargtl) -> arg_binding_loc funargtl fedargtl (extend_env (LocBind (funargid, (lookup_loc_env fedargid curr_env))) fun_env) curr_env
      | (_, _) -> raise (Failure "Not enough arguments")

let rec list_transform1 : ('a * 'b) list -> 'a list
= fun tuple_lst -> match tuple_lst with
                    | [] -> []
                    | (el, _)::tl -> el::(list_transform1 tl)
      
let rec list_transform2 : ('a * 'b) list -> 'b list
= fun tuple_lst -> match tuple_lst with
                    | [] -> []
                    | (_, el)::tl -> el::(list_transform2 tl)
                    
let rec record_process_aux : id list -> value list -> record -> memory -> (value * memory)
= fun idlst valuelst out_record mem -> match (idlst, valuelst) with
                              | ([], []) -> (Record out_record, mem)
                              | (idhd::idtl, valhd::valtl) -> let new_loc = new_location() in
                                                                record_process_aux idtl valtl (extend_record (idhd, new_loc) out_record) (extend_mem (new_loc, valhd) mem)
                              | (_, _) -> raise (Failure "Invalid Record")
                    
let rec record_process : (id * exp) list -> env -> memory -> (env -> memory -> exp -> (value * memory)) -> (value * memory)
= fun input_fieldlst env mem evalf -> let (valuelst, mem1) = generate_value_list (list_transform2 input_fieldlst) [] env mem evalf in
                                        record_process_aux (list_transform1 input_fieldlst) valuelst empty_record mem1

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
  | NUM value -> (Num value, mem)
  | TRUE -> (Bool true, mem)
  | FALSE -> (Bool false, mem)
  | UNIT -> (Unit, mem)
  | VAR id -> (lookup_mem (lookup_loc_env id env) mem, mem)
  | ADD (e1, e2) -> eval_aop env mem e1 e2 (fun x y -> x + y)
  | SUB (e1, e2) -> eval_aop env mem e1 e2 (fun x y -> x - y)
  | MUL (e1, e2) -> eval_aop env mem e1 e2 (fun x y -> x * y)
  | DIV (e1, e2) -> eval_aop env mem e1 e2 (fun x y -> x / y)
  | EQUAL (e1, e2) -> let (v1, mem1) = eval env mem e1 in
                        let (v2, mem2) = eval env mem1 e2 in
                          begin
                            match v1, v2 with
                              | Num n1, Num n2 -> (Bool (n1 = n2), mem2)
                              | Bool b1, Bool b2 -> (Bool (b1 = b2), mem2)
                              | Unit, Unit -> (Bool true, mem2)
                              | _, _ -> (Bool false, mem2)
                          end
  | LESS (e1, e2) -> let (v1, mem1) = eval env mem e1 in
                        let (v2, mem2) = eval env mem1 e2 in
                          begin
                            match v1, v2 with
                              | Num n1, Num n2 -> (Bool (n1 < n2), mem2)
                              | _ -> raise UndefinedSemantics
                          end
  | NOT e -> let (v, mem1) = eval env mem e in
                              begin
                                match v with
                                  | Bool b -> (Bool (not(b)), mem1)
                                  | _ -> raise UndefinedSemantics
                              end
  | SEQ (e1, e2) -> let (_, mem1) = eval env mem e1 in
                      eval env mem1 e2
  | IF (cond, et, ef) -> let (condv, mem1) = eval env mem cond in
                          begin
                            match condv with
                              | Bool b -> if b then eval env mem1 et
                                          else eval env mem1 ef
                              | _ -> raise UndefinedSemantics
                          end
  | WHILE (cond, e) -> let (condv, mem1) = eval env mem cond in
                        begin
                          match condv with
                            | Bool b -> if b then
                                        begin
                                          let (_, mem2) = eval env mem1 e in
                                            eval env mem2 (WHILE (cond, e))
                                        end
                                      else (Unit, mem1)
                            | _ -> raise UndefinedSemantics
                        end
  | LETV (x, ev, e) -> let (v, mem1) = eval env mem ev in
                        let new_loc = new_location() in
                          eval (extend_env (LocBind (x, new_loc)) env) (extend_mem (new_loc, v) mem1) e
  | LETF (f, argl, fun_body, e) -> eval (extend_env (ProcBind (f, (argl, fun_body, env))) env) mem e
  | CALLV (f, argl) -> let (argidlst, fun_body, fun_env) = lookup_proc_env f env in
                        let (newenv, newmem) = arg_binding_val argidlst argl fun_env mem eval in
                                          eval newenv newmem fun_body
  | CALLR (f, locl) -> let (argidlst, fun_body, fun_env) = lookup_proc_env f env in
                        eval (arg_binding_loc argidlst locl fun_env env) mem fun_body
  | RECORD fieldlst -> begin
                        match fieldlst with
                          | [] -> (Unit, mem)
                          | _ -> record_process fieldlst env mem eval
                       end
                        
  | FIELD (record_exp, id) -> let (record_val, mem1) = eval env mem record_exp in
                                begin
                                  match record_val with
                                    | Record fieldlst -> (lookup_mem (lookup_record id fieldlst) mem1, mem1)
                                    | _ -> raise UndefinedSemantics
                                end
  | ASSIGN (id, e) -> let (expval, mem1) = eval env mem e in
                        (expval, extend_mem ((lookup_loc_env id env), expval) mem)
  | ASSIGNF (recname, fieldid, e) -> let (recval, mem1) = eval env mem recname in
                                            let (newval, mem2) = eval env mem1 e in
                                              begin
                                                match recval with
                                                  | Record fieldlst -> (newval, extend_mem ((lookup_record fieldid fieldlst), newval) mem2)
                                                  | _ -> raise UndefinedSemantics
                                              end
  
let runb : exp -> value 
=fun exp -> let (v, _) = eval empty_env empty_mem exp in v;;
  
  