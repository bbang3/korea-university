open Printf;;
(* prob 9 *)
let rec lst2int : int list -> int 
  = fun lst ->
    let ret_str = List.fold_left (fun s a -> s ^ string_of_int a) "" lst
    in int_of_string ret_str;;

let _ = printf "%d\n" (lst2int [1;2;3]);;