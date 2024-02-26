open Printf;;

(* prob 10: using fold_right and fold_left, redefine the given functions *)
let length l = List.fold_left (fun a b -> a + 1) 0 l;;
() = printf "%d\n" (length [1;2;3;4]);;
let reverse l = List.fold_left (fun a b -> b::a) [] l;;
let _ = List.iter (printf "%d ") (reverse [1;4;3;2]);;
let _ = printf "\n";;
let is_all_pos l = List.fold_left (fun a b -> a && (b > 0)) true l;;
let _ = printf "%B\n" (is_all_pos [1;3;1;1]);;
let map f l = List.fold_left (fun a b -> a @ [f b]) [] l;;
let _ = List.iter (printf "%B ") (map (fun x -> x > 0) [1;3;1;-1]);;
let _ = printf "\n";;
let map_r f l = List.fold_right (fun a b -> f a::b) l [];;
let _ = List.iter (printf "%B ") (map_r (fun x -> x > 0) [1;3;1;-1]);;
let _ = printf "\n";;
let filter p l = List.fold_left (fun a b -> if p b then a@[b] else a) [] l;;
let _ = List.iter (printf "%d ") (filter (fun x -> x > 0) [1;2;-1;3;-4]);;
let _ = printf "\n";;
let filter_r p l = List.fold_right (fun a b -> if p a then a::b else b) l [];;
let _ = List.iter (printf "%d ") (filter (fun x -> x > 0) [1;2;-1;3;-4]);;
let _ = printf "\n";;