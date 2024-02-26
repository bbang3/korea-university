open Printf;;
(* prob 5: return a list that excludes first n elements in a given list *)
let rec drop_n : 'a list -> int -> 'a list
  = fun l n ->
    match l with
    | [] -> []
    | hd::tl -> 
      if n = 0 then hd::drop_n tl n
      else drop_n tl (n - 1);;

drop_n [1;2;3;4;5] 2;;
drop_n [1;2] 3;;