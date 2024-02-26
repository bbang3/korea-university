let rec forall : ('a -> bool) -> 'a list -> bool
  = fun f lst -> 
    match lst with
    | [] -> true
    | hd::tl -> (f hd) && (forall f tl);;



forall (fun x -> x mod 2 = 0) [1;2;3];;
forall (fun x -> x > 5) [7;8;9];;

(* let forall : ('a -> bool) -> 'a list -> bool = fun f lst -> TODO *)