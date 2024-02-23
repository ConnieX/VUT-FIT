-- projekt: FLP projekt 1: Haskell (VUT FIT)
-- autor: Monika Rosinská, xrosin00
-- datum: 7. 4. 2021

module GrammarInCNF where

import Grammars
import Data.List (intercalate)

-- check if rule is in given rules
notInRules :: [Rule] -> Rule -> Bool
notInRules rls rule = not ((`elem` rls) rule)

-- removes redundant rules from list of rules
adjustRules :: [Rule] -> [Rule] -> [Rule]
adjustRules newRules [] = newRules
adjustRules newRules ((Rule l r):rls)
  -- if rule is not in list, add it
  | notInRules newRules (Rule l r) = adjustRules (newRules ++ [Rule l r]) rls
  | otherwise = adjustRules newRules rls

-- add new non-terminals to list of non-terminals if they are not in it
-- new non-terminals discovers in rules
newNonterminals :: [Rule] -> [String] -> [String]
newNonterminals [] nonterm = nonterm
newNonterminals ((Rule l _):rls) nonterms
  -- if left side of rule is in nonterminals, skip it, otherwise add it to non-terminals
  | l `elem` nonterms = newNonterminals rls nonterms
  | otherwise = newNonterminals rls (nonterms ++ [l])

-- add nonterminal with a '
newNonterm :: String -> String
newNonterm nonterm
  -- if this string is originally terminal, add ', otherwise no change is needed (to get non-terminal)
  | (`elem` ['a'..'z']) (head nonterm) = nonterm ++ "'"
  | otherwise = nonterm

-- add rule for new nonterminal x'
addFinalRule :: String -> [Rule]
addFinalRule nonterm
  -- if this string is originally terminal, create new rule x' -> x, otherwise no rule is needed
  | (`elem` ['a'..'z']) (head nonterm) = [Rule (nonterm ++ "'") [nonterm]]
  | otherwise = []

-- add rules for new nonterminals; for array of rules
addFinalRules :: [String] -> [Rule] -> [Rule]
addFinalRules [] newRules = newRules
addFinalRules (nonterm:rest) newRules
  -- if this string is originally terminal, create new rule x' -> x, otherwise no rule is needed
  | (`elem` ['a'..'z']) (head nonterm) = addFinalRules rest (newRules ++ [Rule (nonterm ++ "'") [nonterm]])
  | otherwise = addFinalRules rest newRules


-- input string (left side of new rule) -> created rules -> result
-- from X'<YZABC> makes new rule X' -> Y'<ZABC> (first case)
-- from X'<YZ> makes new rule X' -> Y'Z' (second case)
splitRules :: String -> [Rule] -> [Rule]
splitRules [] _ = []
splitRules [_] _ = []
splitRules [_,_] _ = []
splitRules (bracket:first:rest) newRules
   | length rest >= 3 = newRules ++ [Rule (bracket:first:rest) (newNonterm [first] : [bracket:rest])] ++ splitRules (bracket:rest) newRules
   | length rest == 2 = newRules ++ [Rule (bracket:first:rest) (newNonterm [first] : [newNonterm [head rest]])] ++ addFinalRule [first] ++ addFinalRule [head rest]
   | length rest == 1 = newRules ++ [Rule (bracket:first:rest) (newNonterm [first] : [newNonterm [head rest]])] ++ addFinalRule [first]
   | otherwise = newRules ++ splitRules (bracket:rest) newRules
--       left side of rule  -> input string (prev right side of rule) -> created rules -> result
-- from A->XYZ makes new rule A->X'<YZ>
splitRulesFirstStep :: String -> [String] -> [Rule] -> [Rule]
splitRulesFirstStep [] _ _ = []
splitRulesFirstStep _ [] newRules = newRules
splitRulesFirstStep left (x:xs) newRules = newRules ++ [Rule left (newNonterm x : (["<"] ++ xs ++ [">"]))] ++ addFinalRule x
                                               ++ splitRules (intercalate "" (["<"] ++ xs ++ [">"])) newRules

-- add rules to grammar
addRules :: [Rule] -> [Rule] -> [Rule]
addRules newRules [] = newRules
addRules newRules ((Rule l r):rls)
  -- if length of right side of rule is 1, it's rule A->x and no change is needed -> add rule to new rules
  | length (intercalate "" r) == 1 = [Rule l r] ++ addRules newRules rls
  -- if length of right side of rule is 2 and on right side are only non-teerminals,
  -- it's rule A->XY and no change is needed -> add rule to new rules
  | length (intercalate "" r) == 2 && all ((`elem` ['A'..'Z']) .  head) r = [Rule l r] ++ addRules newRules rls
  -- otherwise if length of right side of rule is 2, it's rule A->Xy or A->xY or A->xy and there's need of adding
  -- new non-terminals and rules for them
  | length (intercalate "" r) == 2 = [Rule l (map newNonterm r)] ++ addRules newRules rls ++ addFinalRules r []
  -- if length of right side of rule larger than 3, it's rule A->XYZ and there's need of splitting this rule and
  -- adding new non-terminals and new rules for them
  | length (intercalate "" r) > 2 = splitRulesFirstStep l r newRules ++ addRules newRules rls
  | otherwise = addRules newRules rls


-- transfer grammar to CNF
toCNF :: Grammar -> Grammar
toCNF (Grammar _ t s rls) = do
    let newRules = addRules [] rls
    Grammar (newNonterminals newRules []) t s (adjustRules [] newRules)
