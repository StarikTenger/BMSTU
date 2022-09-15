module UnitTesting(
    test,
    runTests
) where

data Test a b = TestOk | TestFailed {input :: a, result :: b, expected :: b} deriving (Show)

test :: (Eq b) => (a -> b) -> a -> b -> Test a b
test f x r
    | res == r = TestOk
    | otherwise = TestFailed x res r
        where
            res = f x

runTests :: (Eq b) => (a -> b) -> [(a, b)] -> [Test a b]
runTests f samples = map (test_pair f) samples
    where
        test_pair f (x, r) = test f x r