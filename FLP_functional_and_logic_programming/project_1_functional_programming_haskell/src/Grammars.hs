-- projekt: FLP projekt 1: Haskell (VUT FIT)
-- autor: Monika Rosinská, xrosin00
-- datum: 7. 4. 2021

module Grammars where
import Data.List (intercalate)

data Grammar = Grammar {
  nonterminals:: [String],
  terminals:: [String],
  startNonterminal:: String,
  rules :: [Rule]
}

instance Show Grammar where
  show (Grammar nonterms terms s ruls) =
    intercalate "," nonterms ++ "\n" --print nonterminals split by comma
    ++ intercalate "," terms ++ "\n" --print terminals split by comma
    ++ s ++ "\n" --print startNonterminal
    ++ intercalate "\n" (map show ruls) --print rules split by comma using show function of Rule

data Rule = Rule {
  leftSide:: String,
  rightSide:: [String]
} deriving (Eq)

instance Show Rule where
    show (Rule left right) = left ++ "->" ++ intercalate "" right --add -> between left and right side of rule