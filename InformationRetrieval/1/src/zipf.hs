import qualified Data.Set as S
import qualified Data.List as L

zipf text =
	descendingKeeps
	where
		groups = (L.group . L.sort) (words text)
		keeps = map (\xs@(x:_) -> (x,length xs)) groups
		descendingKeeps = L.sortBy (\b a -> compare (snd a) (snd b)) keeps

main = do
	putStrLn "Enter target file name"
	f <- getLine
	content <- readFile f
	putStrLn "Rank \tWord\tCounts\tWeight"
	let
		top10 = take 10 $ zipf content
		f = \rank (w,freq) -> L.intercalate "\t" [(show rank) ++ ":", w, show freq, show (freq * rank)]
		table = zipWith f [1..10] top10
		in putStr $ unlines table