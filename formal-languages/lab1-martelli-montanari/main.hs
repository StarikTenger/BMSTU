import System.IO
import Debug.Trace
import Data.Char(digitToInt)
import Data.List
import UnitTesting
import qualified  Tokenizer as Tkn

data Result a = Error String | Success a

instance Show a => Show (Result a) where
    show (Success x) = show x
    show (Error x) = "Error: " ++ x

validate_Result :: (Result Bool) -> Bool
validate_Result (Success x) = x
validate_Result (Error x) = False

isError (Error _) = True
isError (Success _) = False

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
data VariableSignature = VariableSignature String deriving (Show, Eq, Ord)

-- TODO: 
---- validate only single letters
--                           Input      Output
read_ConstructorSignature :: [Tkn.Token] -> Result [ConstructorSignature]
read_ConstructorSignature [] = Error "empty"
read_ConstructorSignature (Tkn.Name "constructors":Tkn.EqualSign:xs) = read_constructors xs
    where
        read_constructors :: [Tkn.Token] -> Result [ConstructorSignature]
        read_constructors [] = Error "read_ConstructorSignature: expected more"
        read_constructors (Tkn.Name name : Tkn.ParL : Tkn.Val num : Tkn.ParR : Tkn.Comma : xs) = 
            fmap ((ConstructorSignature name num) :) $ read_constructors xs
        read_constructors [Tkn.Name name, Tkn.ParL, Tkn.Val num, Tkn.ParR] = 
            Success [ConstructorSignature name num]
        read_constructors _ = Error "read_ConstructorSignature: fucked up signature"
read_ConstructorSignature _ = Error "read_ConstructorSignature: bullshit tokens"

--                           Input      Output
read_VariableSignature :: [Tkn.Token] -> Result [VariableSignature]
read_VariableSignature [] = Error "empty"
read_VariableSignature (Tkn.Name "variables":Tkn.EqualSign:xs) = read_variables xs
    where
        read_variables :: [Tkn.Token] -> Result [VariableSignature]
        read_variables [] = Error "read_VariableSignature: expected more"
        read_variables (Tkn.Name name : Tkn.Comma : xs) = 
            fmap ((VariableSignature name) :) $ read_variables xs
        read_variables [Tkn.Name name] = 
            Success [VariableSignature name]
        read_variables _ = Error "read_VariableSignature: fucked up signature"
read_VariableSignature _ = Error "read_VariableSignature:bullshit tokens"

data Term = Variable VariableSignature | Constructor ConstructorSignature [Term] deriving (Eq, Ord)
instance Show Term where
    show (Variable (VariableSignature x)) = x
    show (Constructor (ConstructorSignature f _) []) = f
    show (Constructor (ConstructorSignature f _) xs) = 
        f ++ "(" ++ (foldl1 (\a b->a++","++b) $ map show xs) ++ ")"

get_ConstructorSignature (Constructor s _) = s
get_terms (Constructor _ ts) = ts
get_VariableSignature (Variable s) = s

is_in_list x [] = False
is_in_list x xs = foldl1 (||) $ map (==x) xs

--               Tokens         Possible constructors     Possible variables     Term          Rest tokens
read_Variable :: [Tkn.Token] -> [ConstructorSignature] -> [VariableSignature] -> (Result Term, [Tkn.Token])
read_Variable (Tkn.Name name : xs) cns vrs = 
    ((if is_in_list vrsgn vrs 
        then Success $ Variable vrsgn 
        else (if is_in_list cnsgn cns
            then Success $ Constructor cnsgn []
            else Error "read_Variable: Invalid signature")), 
    xs)
    where
        vrsgn = VariableSignature name
        cnsgn = ConstructorSignature name 0
read_Variable xs _ _ = (Error "expected Tkn.Name", xs)

--                  Tokens         Possible constructors     Possible variables     Term          Rest tokens
read_Constructor :: [Tkn.Token] -> [ConstructorSignature] -> [VariableSignature] -> (Result Term, [Tkn.Token])
read_Constructor (Tkn.Name name : Tkn.ParL : xs) cns vrs = 
    ((if validate_Result $ fmap2 (\a b-> a b) (fmap is_in_list cnsgn) (Success cns)
        then fmap2 (\a b-> a b) (fmap Constructor cnsgn) (fst terms) -- try fmap, not fmap2
        else Error "read_Constructor: Invalid signature"),
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

--           Tokens         Possible constructors     Possible variables     Term          Rest tokens
_read_Term :: [Tkn.Token] -> [ConstructorSignature] -> [VariableSignature] -> (Result Term, [Tkn.Token])
_read_Term (Tkn.Name name : Tkn.ParL : xs) = read_Constructor (Tkn.Name name : Tkn.ParL : xs)
_read_Term xs = read_Variable xs

read_Term1 str cns vrs 
    | [Tkn.Name "First", Tkn.Name "term", Tkn.Name ":"] `isPrefixOf` tokens = fst $ _read_Term (drop 3 tokens) cns vrs
    | otherwise = Error "expected: First term:"
    where tokens = Tkn.tokenize str

read_Term2 str cns vrs 
    | [Tkn.Name "Second", Tkn.Name "term", Tkn.Name ":"] `isPrefixOf` tokens = fst $ _read_Term (drop 3 tokens) cns vrs
    | otherwise = Error "expected: Second term:"
    where tokens = Tkn.tokenize str

join_Results :: [Result x] -> Result [x]
join_Results [] = Success []
join_Results (x:xs) = fmap2 (:) x $ join_Results xs

type MultiEquation = ([Term], [Term])

-- use :set -XFlexibleInstances
instance {-# OVERLAPPING #-} Show MultiEquation where
    show (vars, terms) = "{" ++  (show_terms vars) ++ "} = {" ++ (show_terms terms) ++ "}"
        where
            show_terms [] = ""
            show_terms ts =  (foldl1 (\v1 v2 -> v1 ++ ", " ++ v2) $ map show ts)

instance {-# OVERLAPPING #-} Show [MultiEquation] where
    show [] = "{}"
    show ms = "{\n  " ++ (foldl1 (\v1 v2 -> v1 ++ ",\n  " ++ v2) $ map show ms) ++ "\n}"

is_Variable :: Term -> Bool
is_Variable (Variable _) = True
is_Variable (Constructor _ _) = False

allTheSame :: (Eq a) => [a] -> Bool
allTheSame [] = True
allTheSame xs = and $ map (== head xs) (tail xs)

common_tree :: [Term] -> Result (Term, [MultiEquation])
common_tree [] = Error "no terms given"
common_tree terms
    | vars_len == 0 && (allTheSame $ map get_ConstructorSignature cons) = fmap2 (,)
        (fmap (Constructor $ get_ConstructorSignature $ cons!!0) $ fmap (map fst) common_trees)
        (fmap (foldl (++) []) $ fmap (map snd) common_trees)
    | vars_len == 0 = Error $ "Constructor collison: " ++ show terms
    | otherwise = Success (vars!!0, [(vars, cons)])
    where
        vars = filter is_Variable terms
        cons = filter (not . is_Variable) terms
        vars_len = length vars
        cons_len = length cons
        common_trees :: Result [(Term, [MultiEquation])]
        common_trees = join_Results $ map common_tree $ transpose $ map get_terms cons


flatten_Result :: Result (Result a) -> Result a
flatten_Result (Success x) = x
flatten_Result (Error x) = (Error x)

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

dups [] = False
dups [_] = False
dups xs = foldl1 (||) $ zipWith (==)  xs $ tail xs

-- check if MEq have common variables
check_MEqs_Collision :: MultiEquation -> MultiEquation -> Bool
check_MEqs_Collision eq1 eq2 = dups $ merge (fst eq1) (fst eq2)

--sys = fmap common_tree $ join_Results [term1, term2]

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
term_in_MEq t (vs, cs) = foldl (||) False $ map findt $ vs ++ cs
    where
        findt :: Term -> Bool
        findt (Variable x) = (Variable x) == t
        findt (Constructor s ts) = ((Constructor s ts) == t) || (foldl (||) False $ map findt ts)

choose_MEq :: [MultiEquation] -> (Result MultiEquation)
choose_MEq ms
    | filtered /= [] = (Success $ filtered !! 0)
    | otherwise = (Error "unification: step 1")
    where
        check_MEq :: MultiEquation -> [MultiEquation] -> Bool
        check_MEq (vs, _) ms = 1 == (length $ filter (\x->x) $ 
            map (\meq -> foldl (||) False [term_in_MEq x meq | x <- vs]) ms)
        filtered = filter ((flip check_MEq) ms) ms 

replace y z [] = []
replace y z (x:xs)
  | x==y           = z:replace y z xs
  | otherwise      = x:replace y z xs

extract_variables :: Term -> [Term]
extract_variables (Variable x) = [(Variable x)]
extract_variables (Constructor x xs) = foldl (++) [] $ map extract_variables xs

-- U: {x} = (t1, t2), {xi} = ∅
start_sys term1 term2 = fmap2 (:)
    (fmap ((,) [(Variable $ VariableSignature "_")]) $ join_Results [term1, term2]) $
    fmap ((map (\vs -> ([vs!!0],[]))) .  group . sort) $ 
        fmap2 (++) (fmap extract_variables term1) (fmap extract_variables term2)

--     fmap ((map (\vs -> ([vs!!0],[]))) .  group . sort) $ 
pair_Results :: Result (a, b) -> (Result a, Result b)
pair_Results x = (fmap fst x, fmap snd x)

unpair_Results :: (Result a, Result b) -> Result (a, b)
unpair_Results (a, b) = fmap2 (,) a b

unify_sys :: [MultiEquation] -> Result [MultiEquation]
unify_sys sys
    | sys == [] = Success []
    | otherwise = fmap2 (:) meq $ flatten_Result $ fmap unify_sys sys_new
    where
        chosen = choose_MEq sys
        (c, f) = pair_Results $ flatten_Result $ fmap (common_tree . snd) chosen
        replacer = unpair_Results $ (fmap fst chosen, fmap (:[]) c)
        sys_reduced = fmap compactificate $ fmap2 (++) f $ fmap2 (\a b -> replace a b sys) chosen replacer
        meq | isError chosen = chosen
            | otherwise = fmap ((!!0) . (filter $ check_MEqs_Collision $ take_Success chosen)) sys_reduced
        sys_new :: Result [MultiEquation]
        sys_new  = fmap compactificate $ fmap2 delete meq sys_reduced

--               Cons      Vars      Term1     Term2     Output
process_input :: String -> String -> String -> String -> String
process_input cons_str vars_str term1_str term2_str = 
    show $ fmap unify_sys $ flatten_Result $ fmap2 start_sys term1 term2
    where
        cons = read_ConstructorSignature $ Tkn.tokenize cons_str
        vars = read_VariableSignature $ Tkn.tokenize vars_str
        term1 = fmap2 (read_Term1 term1_str) cons vars
        term2 = fmap2 (read_Term2 term2_str) cons vars


main = do
    cons_str <- getLine
    vars_str <- getLine
    fst_term_str <- getLine
    snd_term_str <- getLine
    putStrLn $ process_input cons_str vars_str fst_term_str snd_term_str