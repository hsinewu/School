##### Revise
修正計時包含IO時間的問題
修正循序跟II搜尋正規化不一致的問題

##### query.exe
直接執行後稍待程式進行預處理，接著輸入的查詢格式必須是以~或!的字元開頭，各別代表的是ii查詢或循序查詢
ex1
!some
=> 以ii查詢some單詞出現的文章

支援布林運算(&|)，唯必須是以"後序式"表式(還請自行確保式子沒寫錯)
ex2
~some tea &
=> 以循序查詢"some AND tea"

ex3
!a b | c & b &
=> 以ii查詢 ((a OR b) AND c) AND b


##### editDistance.exe
輸入兩行字
ex
cat\n
car\n
=> distance is 1


##### zipf.exe
輸入想解析的文章檔名
ex
article.txt
=> 印出前10名的單詞名稱、數量、加權分數


###
btw關於編譯，我是在win7底下編譯的程式，應該大部份的人都是用這個，如果非常不幸需要自己編譯的話，我用的編譯器名稱叫ghc，google就有了，並且我沒有安裝任何額外套件，僅下載標準的編譯器即可編譯
$ ghc -O query.hs	//-O代表optimize
亦可不編譯直接執行
$ runghc query.hs