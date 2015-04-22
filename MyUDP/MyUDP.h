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
	//����M�\�P�b�g
	SOCKET sock;
	//���M�p
	struct sockaddr_in sockAddr_send;
	unsigned short portnum_send;
	int sendLength;
	int wait_send;
	//��M�p
	struct sockaddr_in sockAddr_receive;
	unsigned short portnum_receive;
	int receiveLength;
	int wait_receive;

	//���Ԑ���
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