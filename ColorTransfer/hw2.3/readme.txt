先輸入檔名，再輸入n便會產生OUTPUT.bmp，內容為由來源圖檔並以EMD嵌入亂數。
在main.cpp的第92行以assert的方式驗證解出來的數字與最初的相同，如果程式執行時擲出例外即為失敗。
轉換時是以符合L<=64的K值中取最佳的K值來做。
轉換完後會輸出MSE和PSNR至標準輸出流。

若有問題或bug可mail我。