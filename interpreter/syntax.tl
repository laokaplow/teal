(: Position (Record (
  [line Int]
  [column Int])))

(: Location (Record (
  [start Position]
  [end Position]
  [file String])))

(: Syntax::Node (Record (
  [provenance Location]
  [datum (Union [
    Atom
    Integer
    Float64
    String
    (List Syntax)])])))
