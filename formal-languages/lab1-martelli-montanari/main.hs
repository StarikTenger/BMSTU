import Data.Char(digitToInt)
import UnitTesting
import qualified  Tokenizer as Tkn

data Result a = Error String | Success a deriving (Show)

instance Functor Result where
    fmap f (Success x) = Success (f x)
    fmap f (Error x) = Error x

data ConstructorSignature = ConstructorSignature String Int deriving (Show)
data VaraibleSignature = VaraibleSignature String deriving (Show)

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