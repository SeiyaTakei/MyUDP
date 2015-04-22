//**********************************
// UDP Control Program
// 
// by Seiya Takei
//**********************************

#pragma once
#define _WINSOCKAPI_
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <process.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

class MyUDP {
private:
	//送受信ソケット
	SOCKET sock;
	//送信用
	struct sockaddr_in sockAddr_send;
	unsigned short portnum_send;
	int sendLength;
	int wait_send;
	//受信用
	struct sockaddr_in sockAddr_receive;
	unsigned short portnum_receive;
	int receiveLength;
	int wait_receive;

	//時間制御
	clock_t send_count, send_count_start;
	clock_t receive_count, receive_count_start;

	char *sendData;
	char *receiveData;

public:
	MyUDP();
	~MyUDP();
	void init(unsigned short port_send, unsigned short port_receive);
	void update();
	void close();

	void setIPAddress(char *myIP, char *targetIP);
	void setSendData(int send_length, char *sd);
	void setReceiveData(int receive_length, char *rd);
	void setSendFPS(int fps);
	void setReceiveFPS(int fps);
	void updateSend();
	void updateReceive();
	bool isInitialized;
	
};