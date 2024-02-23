-- projekt: FLP projekt 1: Haskell (VUT FIT)
-- autor: Monika Rosinská, xrosin00
-- datum: 7. 4. 2021

module GrammarWithoutSimpleRules where

import Grammars

-- create help set for non-terminal (algorithm 4.5 in TIN scripts; step 1)
--     current nonterm -> set Ni -> rules to process -> original set of rules -> result (help set)
createHelpSet :: String -> [String] -> [Rule] -> [Rule] -> Int -> [String]
createHelpSet _ [] _ _ _ = [] -- we already processed all non-terminals in help set
createHelpSet _ nSet [] originalRules nontermId -- we processed all rules for this non-terminal
  -- if length of help set nSet is greater than nontermId+1, then we didn't process all non-terminals in help set
  --                               createHelpSet  next non-terminal from nSet  rules to process  original rules  nontermId+1
  | length nSet > succ nontermId = createHelpSet (nSet !! succ nontermId) nSet originalRules originalRules (succ nontermId)
  -- otherwise there're no more non-terminals to process and we got the result help set
  | otherwise = nSet
-- we didn't process all rules for this non-terminal, so continue:
createHelpSet nonterm nSet ((Rule l r):nontermRules) originalRules nontermId
  -- check if (Rule l r) is simple rule - right side has length 1 and it's a non-terminal
  -- check if the right side matches current non-terminal
  -- check if the left side of the rule is not in the help set (we want it here only once)
  -- if yes, add left side of rule to the help set and continue with next rule
  | (length r == 1) &&  all ((`elem` ['A'..'Z']) . head) r && (l == nonterm) && not (any ((`elem` head r) . head) nSet) =
    createHelpSet nonterm (nSet ++ [head r]) nontermRules originalRules nontermId
   -- otherwise don't add anything to help set and continue with next rule
  | otherwise = createHelpSet nonterm nSet nontermRules originalRules nontermId


-- add new rules to the grammar (remove simple rules) (algorithm 4.5 in TIN scripts; step 2)
-- current non-term -> help set of this non-term -> new rules -> original rules -> result (new rules)
addRules :: String -> [String] -> [Rule] -> [Rule] -> [Rule]
addRules _ [] _ _ = []
addRules _ _ rls [] = rls
addRules nonterm helpSetNonterms newRules ((Rule l r):originalRules)
  -- if left side of the rule matches current nonterminal and it's not simple rule (right side length > 1 or it's terminal), save the rule
  | l == nonterm && ((length r > 1) || any ((`elem` ['a'..'z']) .  head) r) =
     addRules nonterm helpSetNonterms (newRules ++ [Rule l r]) originalRules
  -- if left side of the rule is in the help set, and it's not simple rule, save the rule
  | any ((`elem` l) . head) helpSetNonterms && ((length r > 1) || any ((`elem` ['a'..'z']) .  head) r) =
     addRules nonterm helpSetNonterms (newRules ++ [Rule nonterm r]) originalRules
  -- otherwise don't save the rule (it's not simple rule or it does nothing to do with current terminal)
  | otherwise = addRules nonterm helpSetNonterms newRules originalRules


-- create new grammar (algorithm 4.5 in TIN scripts; step 3)
generateGrammarWithoutSimpleRules :: Grammar -> [Rule]
generateGrammarWithoutSimpleRules (Grammar [] _ _ _) = []
generateGrammarWithoutSimpleRules (Grammar _ _ [] _) = []
generateGrammarWithoutSimpleRules (Grammar _ _ _ []) = []
generateGrammarWithoutSimpleRules (Grammar (n:nonterms) terms s rls) =
  addRules n (createHelpSet n [n] rls rls 0) [] rls ++ generateGrammarWithoutSimpleRules (Grammar nonterms terms s rls)
  -- for first non-terminal create help set and use it for adding new rules; append calling for other non-terminals

-- generate grammar without simple rules
removeSimpleRules :: Grammar -> Grammar
removeSimpleRules (Grammar n t s r) = Grammar n t s (generateGrammarWithoutSimpleRules (Grammar n t s r))