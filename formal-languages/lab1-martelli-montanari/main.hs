import Data.Char(digitToInt)
import Data.List
import UnitTesting
import qualified  Tokenizer as Tkn

data Result a = Error String | Success a deriving (Show)

validate_Result :: (Result Bool) -> Bool
validate_Result (Success x) = x
validate_Result (Error x) = False

take_Success (Success x) = x

instance Functor Result where
    fmap f (Success x) = Success (f x)
    fmap f (Error x) = Error x

class Functor2 f where 
   fmap2 :: (a -> b -> c) -> f a -> f b -> f c

instance Functor2 Result where
    fmap2 f (Success x) (Success y) = Success (f x y)
    fmap2 f (Success x) (Error y) = Error y
    fmap2 f (Error x) (Success y) = Error x
    fmap2 f (Error x) (Error y) = Error (x ++ ", " ++ y)

data ConstructorSignature = ConstructorSignature String Int deriving (Show, Eq, Ord)
data VaraibleSignature = VaraibleSignature String deriving (Show, Eq, Ord)

-- TODO: 
---- validate only single letters
--                           Input      Output
read_ConstructorSignature :: [Tkn.Token] -> Result [ConstructorSignature]
read_ConstructorSignature [] = Error "empty"
read_ConstructorSignature (Tkn.Name "constructors":Tkn.EqualSign:xs) = read_constructors xs
    where
        read_constructors :: [Tkn.Token] -> Result [ConstructorSignature]
        read_constructors [] = Error "expected more"
        read_constructors (Tkn.Name name : Tkn.ParL : Tkn.Val num : Tkn.ParR : Tkn.Comma : xs) = 
            fmap ((ConstructorSignature name num) :) $ read_constructors xs
        read_constructors [Tkn.Name name, Tkn.ParL, Tkn.Val num, Tkn.ParR] = 
            Success [ConstructorSignature name num]
        read_constructors _ = Error "fucked up signature"
read_ConstructorSignature _ = Error "bullshit tokens"

--                           Input      Output
read_VaraibleSignature :: [Tkn.Token] -> Result [VaraibleSignature]
read_VaraibleSignature [] = Error "empty"
read_VaraibleSignature (Tkn.Name "varaibles":Tkn.EqualSign:xs) = read_varaibles xs
    where
        read_varaibles :: [Tkn.Token] -> Result [VaraibleSignature]
        read_varaibles [] = Error "expected more"
        read_varaibles (Tkn.Name name : Tkn.Comma : xs) = 
            fmap ((VaraibleSignature name) :) $ read_varaibles xs
        read_varaibles [Tkn.Name name] = 
            Success [VaraibleSignature name]
        read_varaibles _ = Error "fucked up signature"
read_VaraibleSignature _ = Error "bullshit tokens"

data Term = Varaible VaraibleSignature | Constructor ConstructorSignature [Term] deriving (Eq, Ord)
instance Show Term where
    show (Varaible (VaraibleSignature x)) = x
    show (Constructor (ConstructorSignature f _) xs) = 
        f ++ "(" ++ (foldl1 (\a b->a++","++b) $ map show xs) ++ ")"

is_in_list x xs = foldl1 (||) $ map (==x) xs

--               Tokens         Possible constructors     Possible varaibles     Term          Rest tokens
read_Varaible :: [Tkn.Token] -> [ConstructorSignature] -> [VaraibleSignature] -> (Result Term, [Tkn.Token])
read_Varaible (Tkn.Name name : xs) cns vrs = 
    ((if is_in_list vrsgn vrs 
        then Success $ Varaible vrsgn 
        else Error "Invalid signature"), 
    xs)
    where
        vrsgn = VaraibleSignature name
read_Varaible xs _ _ = (Error "expected Tkn.Name", xs)

--                  Tokens         Possible constructors     Possible varaibles     Term          Rest tokens
read_Constructor :: [Tkn.Token] -> [ConstructorSignature] -> [VaraibleSignature] -> (Result Term, [Tkn.Token])
read_Constructor (Tkn.Name name : Tkn.ParL : xs) cns vrs = 
    ((if validate_Result $ fmap2 (\a b-> a b) (fmap is_in_list cnsgn) (Success cns)
        then fmap2 (\a b-> a b) (fmap Constructor cnsgn) (fst terms) -- try fmap, not fmap2
        else Error "Invalid signature"),
    (snd terms))
    where
        read_terms :: [Tkn.Token] -> (Result [Term], [Tkn.Token])
        read_terms (Tkn.ParR:ts) = (Success [], ts)
        read_terms (Tkn.Comma:ts) = read_terms ts
        read_terms tokens = (fmap2 (:) (fst res) $ fst terms_read, snd terms_read)
            where
                res = _read_Term tokens cns vrs 
                terms_read = read_terms $ snd res
        terms = read_terms xs
        cnsgn :: (Result ConstructorSignature)
        cnsgn = fmap (ConstructorSignature name) (fmap length $ fst terms)
read_Constructor xs _ _ = (Error "expected Tkn.Name", xs)

--           Tokens         Possible constructors     Possible varaibles     Term          Rest tokens
_read_Term :: [Tkn.Token] -> [ConstructorSignature] -> [VaraibleSignature] -> (Result Term, [Tkn.Token])
_read_Term (Tkn.Name name : Tkn.ParL : xs) = read_Constructor (Tkn.Name name : Tkn.ParL : xs)
_read_Term xs = read_Varaible xs

read_Term str cns vrs = fst $ _read_Term (Tkn.tokenize str) cns vrs

constructors = take_Success $ read_ConstructorSignature $ Tkn.tokenize "constructors = f(2), h(2), g(1)"
varaibles = take_Success $ read_VaraibleSignature $ Tkn.tokenize "varaibles = x, y, x1, x2, x3, x4, x5, x6, x7"
term1 = read_Term "f(g(x3), h(x4, g(x5)))" constructors varaibles
term2 = read_Term "f(x4, h(g(g(x6)), x7)))" constructors varaibles

[x,y,x1,x2,x3,x4,x5,x6,x7] = map Varaible varaibles

join_Results :: [Result x] -> Result [x]
join_Results [] = Success []
join_Results (x:xs) = fmap2 (:) x $ join_Results xs

type MultiEquation = ([Term], [Term])

-- use :set -XFlexibleInstances
instance {-# OVERLAPPING #-} Show MultiEquation where
    show (vars, terms) = "{" ++  (show_terms vars) ++ "} = {" ++ (show_terms terms) ++ "}"
        where
            show_terms ts =  (foldl1 (\v1 v2 -> v1 ++ ", " ++ v2) $ map show ts)

common_tree :: Term -> Term -> (Result Term, [MultiEquation])
common_tree (Varaible x) (Varaible y)
    | (x == y) = (Success $ Varaible x, [])
    | otherwise = (Error "2 varaibles", [])
common_tree (Varaible x) (Constructor f xs) = (Success $ Varaible x, [([Varaible x], [(Constructor f xs)])])
common_tree (Constructor f xs) (Varaible x) = common_tree (Varaible x) (Constructor f xs)
common_tree (Constructor s1 xs) (Constructor s2 ys)
    | (s1 == s2) = (
        fmap (Constructor s1) $ join_Results $ map fst common_trees, 
        foldl1 (++) $ map snd common_trees)
    | otherwise = (Error "constructor signatures mismatch", [])
         where
            common_trees = zipWith common_tree xs ys

merge_MEq :: MultiEquation -> MultiEquation -> MultiEquation
merge_MEq eq1 eq2 = (merge_trms (sort $ fst eq1) (sort $ fst eq2), merge_trms (sort $ snd eq1) (sort $ snd eq2))
    where
        merge_trms :: [Term] -> [Term] -> [Term]
        merge_trms xs [] = xs
        merge_trms [] ys = ys
        merge_trms (x:xs) (y:ys)
            | (x < y) = x : (merge_trms xs $ y:ys)
            | (x > y) = y : (merge_trms (x:xs) ys)
            | otherwise = x : (merge_trms xs ys)

merge :: Ord a => [a] -> [a] -> [a]
merge xs [] = xs
merge [] ys = ys
merge (x:xs) (y:ys)
  | x <= y = x : merge xs (y:ys)
  | otherwise = y : merge (x:xs) ys

dups xs = foldl1 (||) $ zipWith (==)  xs $ tail xs

check_MEqs_Collision :: MultiEquation -> MultiEquation -> Bool
check_MEqs_Collision eq1 eq2 = dups $ merge (fst eq1) (fst eq2)

sys = snd $ take_Success $ fmap2 common_tree term1 term2

compactificate :: [MultiEquation] -> [MultiEquation]
compactificate [] = []
compactificate (x:xs) = foldl1 merge_MEq (x:intrs) : compactificate rest
    where
        intrs = filter (check_MEqs_Collision x) xs
        rest = filter (\y -> not $ check_MEqs_Collision x y) xs


-- TODO:
-- choose S = M
-- common_tree
-- reduction
-- add to 

term_in_MEq :: Term -> MultiEquation -> Bool
term_in_MEq t (vs, cs) = foldl1 (||) $ map findt $ vs ++ cs
    where
        findt :: Term -> Bool
        findt (Varaible x) = (Varaible x) == t
        findt (Constructor s ts) = ((Constructor s ts) == t) || (foldl1 (||) $ map findt ts)

choose_MEq :: [MultiEquation] -> (Result MultiEquation)
choose_MEq ms
    | filtered /= [] = (Success $ filtered !! 0)
    | otherwise = (Error "unification: step 1")
    where
        check_MEq :: MultiEquation -> [MultiEquation] -> Bool
        check_MEq (vs, _) ms = 1 == (length $ filter (\x->x) $ 
            map (\meq -> foldl1 (||) [term_in_MEq x meq | x <- vs]) ms)
        filtered = filter ((flip check_MEq) ms) ms 