#use "syntax.ml";;
#use "evaluator.ml";;
let run : program -> value
  = fun pgm -> eval empty_env pgm

let pgm1 = ADD(CONST 7, CONST 6)
let pgm2 = LET("x", ADD(CONST 1, CONST 2), SUB(VAR "x", CONST 7))