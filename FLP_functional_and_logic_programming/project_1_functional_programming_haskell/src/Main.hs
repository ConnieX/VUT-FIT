-- projekt: FLP projekt 1: Haskell (VUT FIT)
-- autor: Monika Rosinská, xrosin00
-- datum: 7. 4. 2021

module Main where

import System.Environment

import SaveGrammar (saveGrammar)
import GrammarWithoutSimpleRules (removeSimpleRules)
import GrammarInCNF (toCNF)

main :: IO ()
main = do
  args <- getArgs

  let (activity, file) = processArguments args

  content <- readFromFile file
  let grammar = saveGrammar (lines content)


  case activity of
    "BKG" -> print grammar
    "without_simple_rules" -> print (removeSimpleRules grammar)
    "CNF" -> print (toCNF(removeSimpleRules grammar))
    _ -> error "Unkown argument."
  return()

processArguments :: [String] -> (String, String)
processArguments [] = error "At least one argument expected. Choose -i for printing BKG, -1 for printig BKG without simple rules, -2 for printing CNF. Next argument can be file."
processArguments [x] = processArguments [x, ""]
processArguments [x,y]
  | x=="-i" = ("BKG", y)
  | x=="-1" = ("without_simple_rules", y)
  | x=="-2" = ("CNF", y)
  | otherwise = error "Wrong argument"
processArguments _ = error "Too many arguments. Expected 2 arguments max. Choose -i for printing BKG, -1 for printig BKG without simple rules, -2 for printing CNF. Next argument can be file."

readFromFile :: String ->  IO String
readFromFile [] = getContents
readFromFile file = readFile file
