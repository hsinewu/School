main = do
	putStrLn "(Warning: You may NOT want to type in words too long.)"
	putStrLn "Type a word/sentence"
	a <- getLine
	putStrLn "Type another word/sentence"
	b <- getLine
	putStrLn $ "Edit distance is " ++ show (distance a b)
	where
		distance [] [] = 0
		distance a [] = length a
		distance [] b = length b
		distance a@(a0:a') b@(b0:b') =
			minimum [distance a' b + 1, distance a b' + 1, distance a' b' + if a0 == b0 then 0 else 1]