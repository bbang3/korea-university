(* drop all values equal to the key from the list*)
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

uniq [5;6;5;4];;

(* let uniq : 'a list -> 'a list = fun lst -> TODO  *)