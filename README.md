## Etherimg ##
Ethernetを用いて画像（OpenCVのMat形式）を送信・受信を行うソフトウェア  
  
### コンパイル方法 ###
cd Etherimg/  
make  
  
### 使い方 ###
・受信側  
cd Etherimg/src  
sudo ./recv eth0  
(eth0部分は適当なネットワークインターフェースに変更してください）  
  
・送信側
cd Etherimg/src  
sudo ./send eth0 eth0 0 640 480
  
コマンドライン引数の意味:  
第１引数　ネットワークインターフェース  
第２引数　カメラ番号  
第３,４引数　画像の高さ,幅  
  
##Licence
MIT
