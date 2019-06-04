module Euclidian (main) where

import Data.List (partition)

-- | Based on http://unthingable.eat-up.org/posts/2014/Feb/23/euclidean-rhythm-generator-in-haskell/

euclidean :: Int -> Int -> [Bool]
euclidean k n = concat $ efold [ [ i <= k ] | i <- [1..n] ]

efold :: Eq a => [[a]] -> [[a]]
efold xs
    | length xs <= 3 || null a = xs
    | otherwise = efold $ ezip a b
    where (a, b) = partition (/= last xs) xs

ezip :: [[a]] -> [[a]] -> [[a]]
ezip x [] = x
ezip [] x = x
ezip (x:xs) (y:ys) = (x ++ y) : ezip xs ys

display :: [Bool] -> String
display = map (\b -> if b then 'X' else '.')

main :: IO ()
main = do
    putStrLn $ display $ euclidean 1 2
    putStrLn $ display $ euclidean 1 3
    putStrLn $ display $ euclidean 4 12
    putStrLn $ display $ euclidean 2 3
    putStrLn $ display $ euclidean 2 5
    putStrLn $ display $ euclidean 3 4
    putStrLn $ display $ euclidean 3 5
    putStrLn $ display $ euclidean 3 8
    putStrLn $ display $ euclidean 7 12
    mapM_ (putStrLn . display) [ euclidean k n | n <- [1..200], k <- [0..n] ]

