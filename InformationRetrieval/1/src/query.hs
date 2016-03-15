import Data.List(intersect,union,foldl')
import Data.Char(toLower,isAlphaNum)
import Data.Time (diffUTCTime, getCurrentTime)
import qualified Data.Map as Map
import Control.DeepSeq

newsFile = "en09062011.news"
--newsFile = "debug.news"
letterf = map toLower
--wordf = words
wordf =	(concatMap wordsExpand).words
	where
		alphanumfy = \x->if isAlphaNum x then x else ' '
		wordsExpand = \w-> [w] ++ words (map alphanumfy w)

buildMap contents =
	grab (lines contents)
	where
		grab (title:text:url:doc:dateTime:source:category:newLine:xs) = let
			mapify = foldl' (\ac w-> Map.insert w [read doc::Int] ac) Map.empty
			--keeps = Map.unions $ map (mapify.wordf.letterf) [title,text,url,dateTime,source,category,doc]
			keeps = (mapify.wordf.letterf) $ unlines [title,text,url,dateTime,source,category,doc] 
			in Map.unionWith (++) keeps (grab xs)
		grab _ = Map.empty

query contents iiMap sql =
	search sql
	where
		search ('~':sql') = seqSearch sql'
		search ('!':sql') = iiSearch sql'
		search _ = "Query must begin with char ~(sequential) or !(inverted indexed), following wtih desired pattern"
		iiSearch tar = show (eval oprands)
			where
				grab w = maybe [] id (Map.lookup w iiMap)
				oprands = words (map toLower tar)
				eval xs = _eval xs []
					where
					_eval [] ys = head ys
					_eval (x:xs) ys
						| x == "&" = let y2:y1:ys' = ys in _eval xs (intersect y1 y2:ys') 
						| x == "|" = let y2:y1:ys' = ys in _eval xs (union y1 y2:ys') 
						| otherwise = _eval xs (grab x:ys)
		seqSearch tar = let
			sequentialSearch [] = []
			sequentialSearch xs = 
				search x ++ sequentialSearch xs'
				where
					predicate = eval (words tar)
					(|||) f g w = any ($w) [f, g]
					(&&&) f g w = all ($w) [f, g]
					eval xs = _eval xs []
						where
						_eval [] ys = head ys
						_eval (x:xs) ys
							| x == "&" = let y2:y1:ys' = ys in _eval xs (y1 &&& y2:ys') 
							| x == "|" = let y2:y1:ys' = ys in _eval xs (y1 ||| y2:ys') 
							| otherwise = _eval xs (elem x:ys)
					search post =
						let ws = words $ concat post --trim doc yet
							in if predicate ws then [read (post!!3)::Int] else []
					(x, xs') = splitAt 8 xs
			in show $ sequentialSearch (lines contents)

main = do
  before1 <- getCurrentTime
  putStrLn "Preprocessing..."
  contents <- readFile newsFile
  let
    lcontents = map toLower contents
    invertedIndice = buildMap lcontents
    diffTimeMs t1 t2 = round (realToFrac (t2 `diffUTCTime` t1) * 1000) :: Int
  deepseq invertedIndice (putStrLn "done.")
  after1 <- getCurrentTime
  putStrLn $ "preprocessing time: " ++ show (diffTimeMs before1 after1) ++ " ms"
  let
    looper = do
      putStrLn "Enter your query> "
      sql <- getLine
      before2 <- getCurrentTime
      let outcome = query lcontents invertedIndice (map toLower sql)
      deepseq outcome (putStr "")
      after2 <- getCurrentTime
      putStrLn outcome
      putStrLn $ "query time: " ++ show (diffTimeMs before2 after2) ++ " ms"
      looper
    in looper
