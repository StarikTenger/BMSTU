import Data.Char(digitToInt)
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

data ConstructorSignature = ConstructorSignature String Int deriving (Show, Eq)
data VaraibleSignature = VaraibleSignature String deriving (Show, Eq)

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

data Term = Varaible VaraibleSignature | Constructor ConstructorSignature [Term]
instance Show Term where
    show (Varaible (VaraibleSignature x)) = x
    show (Constructor (ConstructorSignature f _) xs) = 
        f ++ "(" ++ (foldl1 (\a b->a++","++b) $ map show xs) ++ ")"

find x xs = foldl1 (||) $ map (==x) xs

--               Tokens         Possible constructors     Possible varaibles     Term          Rest tokens
read_Varaible :: [Tkn.Token] -> [ConstructorSignature] -> [VaraibleSignature] -> (Result Term, [Tkn.Token])
read_Varaible (Tkn.Name name : xs) cns vrs = 
    ((if find vrsgn vrs 
        then Success $ Varaible vrsgn 
        else Error "Invalid signature"), 
    xs)
    where
        vrsgn = VaraibleSignature name
read_Varaible xs _ _ = (Error "expected Tkn.Name", xs)

--                  Tokens         Possible constructors     Possible varaibles     Term          Rest tokens
read_Constructor :: [Tkn.Token] -> [ConstructorSignature] -> [VaraibleSignature] -> (Result Term, [Tkn.Token])
read_Constructor (Tkn.Name name : Tkn.ParL : xs) cns vrs = 
    ((if validate_Result $ fmap2 (\a b-> a b) (fmap find cnsgn) (Success cns)
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

constructors = take_Success $ read_ConstructorSignature $ Tkn.tokenize "constructors = f(2), g(1)"
varaibles = take_Success $ read_VaraibleSignature $ Tkn.tokenize "varaibles = x, y"
term1 = read_Term "g(f(f(x, g(f(x, y))), g(x)))" constructors varaibles
term2 = read_Term "g(f(f(x, y), y))" constructors varaibles

join_Results :: [Result x] -> Result [x]
join_Results [] = Success []
join_Results (x:xs) = fmap2 (:) x $ join_Results xs

common_tree :: Term -> Term -> (Result Term)
common_tree (Varaible x) (Varaible y)
    | (x == y) = Success $ Varaible x
    | otherwise = Error "2 varaibles"
common_tree (Varaible x) (Constructor _ _) = Success $ Varaible x
common_tree (Constructor _ _) (Varaible x) = Success $ Varaible x
common_tree (Constructor s1 xs) (Constructor s2 ys)
    | (s1 == s2) = fmap (Constructor s1) $ join_Results $ map (\(x, y) -> common_tree x y) $ zip xs ys
    | otherwise = Error "constructor signatures mismatch"