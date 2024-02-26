open Printf;;
let all : ('a -> bool) -> 'a list -> bool
  = fun f lst ->
    List.fold_right (fun a b -> (f a) && b) lst true;; 

all (fun x -> x > 5) [7;8;9];;
all (fun x -> x > 5) [1;7;8;9];;