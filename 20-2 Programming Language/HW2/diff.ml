type aexp =
  | Const of int
  | Var of string
  | Power of string * int
  | Times of aexp list
  | Sum of aexp list

let rec diff : aexp * string -> aexp
  = fun (exp, x) ->
    match exp with
    | Const n -> Const 0    
    | Var v -> if v = x then Const 1 else Const 0
    | Power (v, n) ->
      if v <> x then Const 0
      else
        begin
          match n with
          | 0 -> Const 0
          | 1 -> Const 1
          | _ -> Times[Const n; Power(v, n - 1)]
        end
    | Sum (explst) ->
      begin
        let rec diff_sumlst : aexp list * string -> aexp list
          = fun (explst, x) ->
            match explst with
            | [] -> []
            | hd::tl -> diff(hd, x)::diff_sumlst(tl, x)
        in Sum (diff_sumlst(explst, x))
      end
    | Times(explst) ->
      begin
        match explst with
        | [] -> Times []
        | hd::[] -> diff(hd, x)
        | hd::tl -> Sum [Times(diff(hd, x)::tl); Times [hd; diff(Times(tl), x)]]
      end;;
(* begin
   let rec diff_timeslst : aexp list * string -> aexp list
    = fun (explst, x) ->
      match explst with
      | [] -> []
      | hd::[] -> [diff(hd, x)]
      | hd::tl -> [Times (diff(hd, x)::tl)] @ ([Times [hd]] @ [Sum (diff_timeslst(tl, x))])
   in Sum(diff_timeslst(explst, x))
   end;; *)

diff (Sum [Power ("x", 2); Times [Const 2; Var "x"]; Const 1], "x");; (* x^2 + 2x + 1 *)
diff (Times[Const 2; Var "x"; Var "y"], "x");;  (* 2xy *)
diff (Times[Const 2; Var "x"; Var "y"], "y");;



diff (Const 1, "x");;

diff (Var "y", "x");;

diff (Var "x", "x");;

diff (Power ("xxx",4) , "xxx");;

diff (Power ("xy",3) , "yx");;

diff (Times [Const 3; Var "x" ; Power ("y", 2) ; Var "z"] , "y");;
(* 3xy^2z -> 6xyz *)


diff (Sum [(Times [Const 3; Var "x" ; Power ("y",2) ; Var "z"]); Var "y"] , "x");;
(* 3xy^2z + y -> 3y^2z*)

diff (Sum [(Times [Const 2; Power ("x",2)]); (Times [Const 3; Var "x"]); Const 4] , "x");;
(* 2x^2 + 3x + 4 *)


diff(Sum[Times[Const 10;Var "x"];Var "y"], "xy");; (*10x + y*)
diff(Times[Var "x"; Var "x"], "x") ;;
diff(Power ("xxx",4) , "xxx");;
diff(Power ("xy",3) , "yx");;



(* Times [Const 2; y] *)