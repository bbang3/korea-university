let rec reduce : ('a -> 'b -> 'c -> 'c) -> 'a list -> 'b list -> 'c -> 'c
  = fun f l1 l2 accu -> (* TODO *)
    match l1, l2 with
    | [], [] -> accu
    | [], _ -> raise(Failure("Invalid Input"))
    | _, [] -> raise(Failure("Invalid Input"))
    | h1::t1, h2::t2 ->
      reduce f t1 t2 (f h1 h2 accu);;

reduce (fun x y z -> x * y + z) [1;2;3] [0;1;2] 0;;
(* let reduce : ('a -> 'b -> 'c -> 'c) -> 'a list -> 'b list -> 'c -> 'c = fun f l1 l2 accu -> TODO *)
