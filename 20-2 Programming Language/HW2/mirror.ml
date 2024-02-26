type btree = 
  | Leaf of int
  | Left of btree
  | Right of btree
  | LeftRight of btree * btree

let rec mirror : btree -> btree
  = fun tree -> 
    match tree with
    | Leaf(n) -> Leaf(n)
    | Left(node) -> Right(mirror node)
    | Right(node) -> Left(mirror node)
    | LeftRight(lnode, rnode) -> LeftRight(mirror rnode, mirror lnode);;

mirror (Left (LeftRight (Leaf 1, Leaf 2)));;