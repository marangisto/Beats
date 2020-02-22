module Euclidian (main) where

import Control.Monad (forM_)

----
--
--      Euclidean Rythm algorithm returning distances between beats
--      Based on http://www.maths.usyd.edu.au/u/joachimw/talk2.pdf
--      Valid for n > 0, 0 < k <= n
--
----

euclidean :: Int -> Int -> [Int]
euclidean k n
    | a == 0 = replicate k b
    | otherwise = concatMap f $ euclidean a k 
    where (b, a) = n `divMod` k
          f x = replicate (x - 1) b ++ [b + 1]

----
--
--      Expanded Euclidean Rythm indicating beats and rests
--      Valid for n > 0, 0 <= k <= n
--
----

data Step = Beat | Rest

euclidean' :: Int -> Int -> [Step]
euclidean' 0 n = replicate n Rest
euclidean' k n = concatMap f $ euclidean k n
    where f x = Beat : replicate (x-1) Rest

display :: [Step] -> String
display = map f
    where f Beat = 'X'
          f Rest = '-'

main :: IO ()
main = do
    mapM_ (putStrLn . display) [ euclidean' k n | n <- [1..100], k <- [0..n] ]
    forM_ [ (1,2), (1,3), (4,12), (2,3), (2,5), (3,4), (3,5), (3,8), (7,12), (7,16) ] $ \e@(k, n) -> do
        putStrLn $ unwords [ show e, ":", display $ euclidean' k n ]

