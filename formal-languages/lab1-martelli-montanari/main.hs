import Data.Char(digitToInt)
import UnitTesting
import qualified  Tokenizer as Tkn

data Result a = Error String | Success a deriving (Show)

instance Functor Result where
    fmap f (Success x) = Success (f x)
    fmap f (Error x) = Error x

data ConstructorSignature = ConstructorSignature String Int deriving (Show)
data VaraiblePrototype = VaraiblePrototype Char deriving (Show)

--                           Input      Output
read_ConstructorSignature :: [Tkn.Token] -> Result [ConstructorSignature]
read_ConstructorSignature [] = Success []
read_ConstructorSignature (Tkn.Name "constructors":Tkn.EqualSign:xs) = read_constructors xs
    where
        read_constructors :: [Tkn.Token] -> Result [ConstructorSignature]
        read_constructors [] = Success []
        read_constructors (Tkn.Name name : Tkn.ParL : Tkn.Val num : Tkn.ParR : Tkn.Comma : xs) = 
            fmap ((ConstructorSignature name num) :) $ read_constructors xs
        read_constructors (Tkn.Name name : Tkn.ParL : Tkn.Val num : Tkn.ParR : xs) = 
            Success [ConstructorSignature name num]
        read_constructors _ = Error "fucked up signature"
read_ConstructorSignature _ = Error "bullshit tokens"

-- read_ConstructorSignature $ Tkn.tokenize "constructors = f(a)"