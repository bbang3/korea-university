open Printf;;
let rec concat : 'a list list -> 'a list
  = fun list_of_list ->
    match list_of_list with
    | [] -> []
    | hd_lst::tl_lst -> hd_lst@concat tl_lst;;

concat [[1;2];[3;4;5]];;