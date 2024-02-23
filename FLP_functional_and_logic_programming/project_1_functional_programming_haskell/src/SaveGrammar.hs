-- projekt: FLP projekt 1: Haskell (VUT FIT)
-- autor: Monika Rosinská, xrosin00
-- datum: 7. 4. 2021

module SaveGrammar where

import Grammars

-- save grammar to inner representation
saveGrammar :: [String] -> Grammar
saveGrammar [] = error "Error while reading grammar."
saveGrammar x
    | length x < 4 = error "Wrong input grammare."
    | otherwise =
      Grammar {
        nonterminals = splitInput (head x) [], -- take first line of input and split it into nonterminals
        terminals = splitInput (x !! 1) [], -- take second line of input and split it into terminals
        startNonterminal = x !! 2, -- take third line of input and set it as start nonterminal
        rules = getRules (drop 3 x) -- take all other lines of input and split them into rules
      }

-- split input (expect (non)terminals divided by some symbol)
splitInput :: String -> [String] -> [String]
splitInput [] output = output
--                        call again without first symbol and delimiter, add first symbol to output
splitInput input output = splitInput (drop 2 input) (output ++ [take 1 input])

-- get rules from input
getRules :: [String] -> [Rule]
getRules input = map parseRule input

-- parse rule and save it to data structure Rule
-- we assume that rule is in correct format
parseRule :: String -> Rule
parseRule [] = error "Error while parsing rule."
parseRule x =
  Rule {
    leftSide = take 1 x, -- it's non-contextual grammar, so we can take only first symbol
    rightSide = splitString (drop 3 x) -- drop 3 because we have (only one) start symbol and "->"
    }

-- split String to array of Strings ([String])
splitString :: String -> [String]
splitString [] = []
splitString (x:xs) = [x] : splitString xs
