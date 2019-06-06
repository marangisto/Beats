module Euclidian (main) where

import Data.List (partition)
import Control.Monad (forM_)

-- | Based on http://unthingable.eat-up.org/posts/2014/Feb/23/euclidean-rhythm-generator-in-haskell/
euclidean1 :: Int -> Int -> [Bool]
euclidean1 k n = concat $ efold [ [ i <= k ] | i <- [1..n] ]

efold :: Eq a => [[a]] -> [[a]]
efold xs
    | length xs <= 3 || null a = xs
    | otherwise = efold $ ezip a b
    where (a, b) = partition (/= last xs) xs

ezip :: [[a]] -> [[a]] -> [[a]]
ezip x [] = x
ezip [] x = x
ezip (x:xs) (y:ys) = (x ++ y) : ezip xs ys

-- Based on http://www.maths.usyd.edu.au/u/joachimw/talk2.pdf
euclidean2' :: Int -> Int -> [Int]
euclidean2' k n
    | a == 0 = replicate k b
    | otherwise = concatMap f $ euclidean2' a k 
    where (b, a) = n `divMod` k
          f x = replicate (x - 1) b ++ [b + 1]

euclidean2 :: Int -> Int -> [Bool]
euclidean2 0 n = replicate n False
euclidean2 k n = concatMap f $ euclidean2' k n
    where f x = True : replicate (x-1) False

display :: [Bool] -> String
display = map (\b -> if b then 'X' else '.')

main :: IO ()
main = do
    mapM_ (putStrLn . display) [ euclidean2 k n | n <- [1..100], k <- [0..n] ]
    forM_ [ (1,2), (1,3), (4,12), (2,3), (2,5), (3,4), (3,5), (3,8), (7,12), (7,16) ] $ \e@(k, n) -> do
        putStrLn $ unwords [ show e, ":", display (euclidean1 k n), "    ", display (euclidean2 k n) ]

