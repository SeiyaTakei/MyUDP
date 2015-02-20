MyUDP
=====

自分用UDPミニマムプログラム  
IPの書き換えメソッドを使えば同一LAN内の他端末ともデータを送受信できる　
Receiveに関してはバッファにデータがなければTimeoutとなって通りすぎてしまうので注意　 

・つかいかた　
MyUDP *udp;　
udp = new MyUDP();　

udp->init(10000, 10001);//相手の10001ポートへ送信，自分の10000ポートへ受信　
//initは引数のポート番号に0を入れると送信(受信)を利用しない　
　
udp->setSendData(sendDataLength_byte, (char*)sendData);　
udp->setReceiveData(receiveDataLength_byte, (char*)receiveData);　
　
udp->setIPAddress(myIP, targetIP);//自分のIPアドレス(127.0.0.1はNG)，相手のIPアドレス　
　
udp->setSendFps(60);//ループを回す際に送信のfpsを設定　
　
udp->update()　
