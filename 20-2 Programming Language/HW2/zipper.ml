let rec zipper : int list * int list -> int list
  = fun (l1, l2) -> 
    match l1, l2 with
    | [], _ -> l2
    | _, [] -> l1
    | h1::t1, h2::t2 -> h1::h2::zipper(t1, t2);;

zipper ([1;3;5],[2;4;6]);;
zipper ([1;3],[2;4;6;8]);;
zipper ([1;3;5;7],[2;4]);;