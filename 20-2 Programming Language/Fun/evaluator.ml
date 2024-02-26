#use "syntax.ml";;
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