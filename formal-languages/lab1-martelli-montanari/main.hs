import Data.Char(digitToInt)
import UnitTesting
import qualified  Tokenizer as Tkn

data Result a = Error String | Success a deriving (Show)

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
    ((if find cnsgn cns
        then fmap2 Constructor (Success cnsgn) (fst terms) -- try fmap, not fmap2
        else Error "Invalid signature"),
    (snd terms))
    where
        -- TODO: read_Term instead of read_Varaible
        read_terms :: [Tkn.Token] -> (Result [Term], [Tkn.Token])
        read_terms (Tkn.ParR:ts) = (Success [], ts)
        read_terms (Tkn.Comma:ts) = read_terms ts
        read_terms tokens = (fmap2 (:) (fst res) $ fst terms_read, snd terms_read)
            where
                res = _read_Term tokens cns vrs -- read Term
                terms_read = read_terms $ snd res
        terms = read_terms xs 
        cnsgn = ConstructorSignature name $ length $ take_Success $ fst terms
read_Constructor xs _ _ = (Error "expected Tkn.Name", xs)

--           Tokens         Possible constructors     Possible varaibles     Term          Rest tokens
_read_Term :: [Tkn.Token] -> [ConstructorSignature] -> [VaraibleSignature] -> (Result Term, [Tkn.Token])
_read_Term (Tkn.Name name : Tkn.ParL : xs) = read_Constructor (Tkn.Name name : Tkn.ParL : xs)
_read_Term xs = read_Varaible xs

read_Term ts cns vrs = fst $ _read_Term ts cns vrs

constructors = take_Success $ read_ConstructorSignature $ Tkn.tokenize "constructors = f(2), g(1)"
varaibles = take_Success $ read_VaraibleSignature $ Tkn.tokenize "varaibles = x, y"
example = read_Term (Tkn.tokenize "f(f(x, g(f(x, y))), g(x))") constructors varaibles