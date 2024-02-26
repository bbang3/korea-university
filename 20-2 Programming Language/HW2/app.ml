(* drop all values equal to a key from the list*)
let rec drop : 'a -> 'a list -> 'a list
  = fun key lst ->
    match lst with
    | [] -> []
    | hd::tl -> 
      if hd = key then drop key tl 
      else hd::drop key tl

let rec uniq : 'a list -> 'a list
  = fun lst -> 
    match lst with
    | [] -> []
    | hd::tl -> hd::uniq(drop hd tl);;

let app : 'a list -> 'a list -> 'a list
  = fun l1 l2 -> uniq (l2@l1);;

app [4;5;6;7] [1;2;3;4];;

(* let app : 'a list -> 'a list -> 'a list = fun l1 l2 -> TODO *)