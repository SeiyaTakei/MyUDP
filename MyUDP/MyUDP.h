#pragma once
//#include <Windows.h>
#include <process.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define UDP_LENGTH_MAX 2048

class MyUDP {
private:
	//ソケット関連
	
	//送信用ソケット
	struct sockaddr_in sockAddr_send;
	SOCKET sock_send;
	unsigned short portnum_send;
	int sendLength;
	//受信ソケット
	struct sockaddr_in sockAddr_receive;
	SOCKET sock_receive;
	unsigned short portnum_receive;
	int receiveLength;

	//スレッド処理関連
	HANDLE sendThread;
	HANDLE receiveThread;

public:
	MyUDP();
	~MyUDP();
	void init(unsigned short port_send, int send_length, unsigned short port_receive, int receive_length);
	void stop();
	static unsigned __stdcall SendThread(void *ptr);
	static unsigned __stdcall ReceiveThread(void *ptr);
	void updateSend();
	void updateReceive();

	bool isInitialized;
	char sendData[UDP_LENGTH_MAX];
	char receiveBuff[UDP_LENGTH_MAX];
};