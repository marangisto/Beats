module Euclidian (main) where

import Data.List (partition)

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
    | a == 0 = replicate k (n `div` k)
    | otherwise =
        let xs = euclidean2' a k 
            f = floor $ (fromIntegral n) / (fromIntegral k)
            c = ceiling $ (fromIntegral n) / (fromIntegral k)
            h x = replicate (x - 1) f ++ [ c ]
         in concatMap h xs
    where a = n `mod` k

euclidean2 :: Int -> Int -> [Bool]
euclidean2 0 n = replicate n False
euclidean2 k n = concatMap f $ euclidean2' k n
    where f x = replicate (x-1) False ++ [True]

display :: [Bool] -> String
display = map (\b -> if b then 'X' else '.')

euclidean = euclidean2

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
    putStrLn $ display $ euclidean 7 16
    mapM_ (putStrLn . display) [ euclidean k n | n <- [1..100], k <- [0..n] ]

