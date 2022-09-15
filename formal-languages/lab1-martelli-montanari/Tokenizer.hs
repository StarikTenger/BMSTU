module Tokenizer(
    Token(..),
    tokenize
) where

import Data.Char(digitToInt)

data Token = Val Int | Name String | EqualSign | Comma | ParL | ParR | Add | Sub | Mul | Div deriving (Show, Eq)

isNum x = x >= '0' && x <= '9'
isLetter x = x >= 'a' && x <= 'z' || x >= 'A' && x <= 'Z'

--          Input     Acc       Output   New string
read_name :: String -> String -> (String, String)
read_name [] acc = (acc, [])
read_name (x:xs) acc
    | (isNum x) || (isLetter x) = read_name xs $ x:acc
    | otherwise = (acc, x:xs)

--          Input     Acc       Output   New string
read_val :: String -> String -> (Int,    String)
read_val str acc  = (\(n, s) -> (read $ reverse n, s)) $ read_val_stringed str acc
    where
        read_val_stringed [] acc = (acc, [])
        read_val_stringed (x:xs) acc
            | (isNum x) = read_val_stringed xs $ x:acc
            | otherwise = (acc, x:xs)

tokenize :: String -> [Token]
tokenize str = reverse $ read [] str
    where
        read acc [] = acc
        read acc (' ':str) = read acc str
        read acc ('=':str) = read (EqualSign:acc) str
        read acc (',':str) = read (Comma:acc) str
        read acc ('(':str) = read (ParL:acc) str
        read acc (')':str) = read (ParR:acc) str
        read acc ('+':str) = read (Add:acc) str
        read acc ('-':str) = read (Sub:acc) str
        read acc ('*':str) = read (Mul:acc) str
        read acc ('/':str) = read (Div:acc) str
        read acc (c:str)
            | isLetter c = let (name, rest) = read_name (c:str) "" in read ((Name $ reverse name):acc) rest
            | isNum c = let (val, rest) = read_val (c:str) "" in read ((Val val):acc) rest
            | otherwise = read ((Name [c]):acc) str
                    