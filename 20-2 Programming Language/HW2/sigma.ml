let rec sigma : (int -> int) -> int -> int -> int
  = fun f a b -> (* TODO *)
    if a > b then 0
    else (f a) + (sigma f (a + 1) b);;

sigma(fun x -> x) 1 10;;
sigma(fun x -> x * x) 1 7;;


(* let sigma : (int -> int) -> int -> int -> int = fun f a b -> TODO *) 