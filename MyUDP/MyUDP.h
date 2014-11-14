#pragma once
#include <process.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")


class MyUDP {
private:
	//�\�P�b�g�֘A
	
	//���M�p�\�P�b�g
	struct sockaddr_in sockAddr_send;
	SOCKET sock_send;
	unsigned short portnum_send;
	int sendLength;
	int wait_send;
	//��M�\�P�b�g
	struct sockaddr_in sockAddr_receive;
	SOCKET sock_receive;
	unsigned short portnum_receive;
	int receiveLength;
	int wait_receive;

	//�X���b�h�����֘A
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