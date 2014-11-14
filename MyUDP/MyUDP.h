#pragma once
#include <process.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")


class MyUDP {
private:
	//ソケット関連
	
	//送信用ソケット
	struct sockaddr_in sockAddr_send;
	SOCKET sock_send;
	unsigned short portnum_send;
	int sendLength;
	int wait_send;
	//受信ソケット
	struct sockaddr_in sockAddr_receive;
	SOCKET sock_receive;
	unsigned short portnum_receive;
	int receiveLength;
	int wait_receive;

	//スレッド処理関連
	HANDLE sendThread;
	HANDLE receiveThread;

public:
	MyUDP();
	~MyUDP();
	void init(unsigned short port_send, int send_length, char *sd, unsigned short port_receive, int receive_length, char *rb);
	void run();
	void stop();
	static unsigned __stdcall SendThread(void *ptr);
	static unsigned __stdcall ReceiveThread(void *ptr);
	void updateSend();
	void updateReceive();
	void setSendFPS(int fps);
	void setReceiveFPS(int fps);

	bool isInitialized;
	char *sendData;
	char *receiveBuff;
};