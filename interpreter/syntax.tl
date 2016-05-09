(: Position (Record (
  [line Int]
  [column Int])))

(: Location (Record (
  [start Position]
  [end Position]
  [file String])))

(: Syntax::Node (Record (
  [provenance Location]
  [datum (Union (
    [atom Atom]
    [int Integer]
    [float64 Float64]
    [string String]
    [sexpr (List Syntax)]))])))
