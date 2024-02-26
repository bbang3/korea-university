type digit = ZERO | ONE
type bin = digit list

let rec binary_to_decimal : bin -> int -> int
  = fun bin_num cur_result->
    match bin_num with
    | [] -> raise(Failure("Invalid Input"))
    | hd::[] -> 
      if hd = ZERO then cur_result * 2
      else cur_result * 2 + 1
    | hd::tl ->
      if hd = ZERO then binary_to_decimal tl (cur_result * 2)
      else binary_to_decimal tl (cur_result * 2 + 1)

let rec decimal_to_binary : int -> bin
  = fun n ->
    if n = 0 then [ZERO]
    else if n = 1 then [ONE]
    else decimal_to_binary(n / 2) @ [if n mod 2 = 0 then ZERO else ONE]

let bmul : bin -> bin -> bin
  = fun a b ->
    let dec_a = binary_to_decimal a 0 in
    let dec_b = binary_to_decimal b 0 in
    decimal_to_binary(dec_a * dec_b);;

bmul [ONE;ZERO;ONE;ONE] [ONE;ONE;ONE;ONE;ZERO];;