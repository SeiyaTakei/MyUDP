#pragma once
#include "MyUDP.h"
#include <iostream>
#include <time.h>
using namespace std;

MyUDP::MyUDP() : isInitialized(false), wait_send(0), wait_receive(0) {
}

MyUDP::~MyUDP() {
}

void MyUDP::init(unsigned short port_send, int send_length, unsigned short port_receive, int receive_length) {
	portnum_send = port_send;
	portnum_receive = port_receive;
	sendLength = send_length;
	receiveLength = receive_length;
	memset(sendData, 0, sizeof(sendData));
	memset(receiveBuff, 0, sizeof(receiveBuff));

	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 0), &wsadata)) {
		cout << "WSAStartup Faild." << endl;
		return;
	}

#pragma region SendSocketSetup
	if (portnum_send != 0) {
		sock_send = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock_send == INVALID_SOCKET) {
			cout << stderr << "Send Socket Error" << endl;
			WSACleanup();
			return;
		}
		cout << stderr << "Send Socket Created" << endl;
		memset(&sockAddr_send, 0, sizeof(sockAddr_send));
		sockAddr_send.sin_addr.s_addr = inet_addr("127.0.0.1");
		sockAddr_send.sin_port = htons(portnum_send);
		sockAddr_send.sin_family = AF_INET;

		/*if (bind(sock_send, (struct sockaddr *)&sockAddrIn, sizeof(sockAddrIn)) == SOCKET_ERROR) {
			closesocket(sock_send);
			WSACleanup();
			return;
		}*/
	}
#pragma endregion

#pragma region ReceiveSocketSetup
	if (portnum_receive != 0) {
		sock_receive = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock_receive == INVALID_SOCKET) {
			cout << stderr << "Receive Socket Error" << endl;
			WSACleanup();
			return;
		}
		cout << stderr << "Receive Socket Created" << endl;
		memset(&sockAddr_receive, 0, sizeof(sockAddr_receive));
		sockAddr_receive.sin_addr.s_addr = inet_addr("127.0.0.1");
		sockAddr_receive.sin_port = htons(portnum_receive);
		sockAddr_receive.sin_family = AF_INET;

		if (bind(sock_receive, (struct sockaddr *)&sockAddr_receive, sizeof(sockAddr_receive)) == SOCKET_ERROR) {
			closesocket(sock_receive);
			WSACleanup();
			return;
		}
	}
#pragma endregion

	isInitialized = true;
}

void MyUDP::run() {
	if (isInitialized) {
		if (portnum_send != 0) {
			unsigned threadID1;
			sendThread = (HANDLE)_beginthreadex(NULL, NULL, MyUDP::SendThread, this, 0, &threadID1);
		}
		if (portnum_receive != 0) {
			unsigned threadID2;
			receiveThread = (HANDLE)_beginthreadex(NULL, NULL, MyUDP::ReceiveThread, this, 0, &threadID2);
		}
	}
}

void MyUDP::stop() {
	if (isInitialized) {
		if (portnum_send != 0) {
			CloseHandle(sendThread);
			closesocket(sock_send);
		}
		if (portnum_receive != 0) {
			CloseHandle(receiveThread);
			closesocket(sock_receive);
		}
	}
	WSACleanup();
}

unsigned __stdcall MyUDP::SendThread(void *ptr) {
	reinterpret_cast<MyUDP*>(ptr)->updateSend();
	return 0;
}

unsigned __stdcall MyUDP::ReceiveThread(void *ptr) {
	reinterpret_cast<MyUDP*>(ptr)->updateReceive();
	return 0;
}

void MyUDP::updateSend() {
	int num;
	clock_t countStart, countEnd;
	int waiting;
	//countBegin = clock();
	countEnd = clock();
	while (true) {
		countStart = clock();
		num = sendto(sock_send, sendData, sendLength, 0, (const struct sockaddr *)&sockAddr_send, sizeof(sockAddr_send));
		
		while (wait_send > (countEnd - countStart)) {
			countEnd = clock();
		}
		//Sleep(wait_send - (countEnd - countStart));
	}
}

void MyUDP::updateReceive() {
	int num;
	while (true) {
		num = recv(sock_receive, receiveBuff, receiveLength, 0);
	}
}

void MyUDP::setSendFPS(int fps) {
	wait_send =  (int)(1000 * 1.0/(double)fps);
}

void MyUDP::setReceiveFPS(int fps) {
	wait_receive = (int)(1000 * 1.0 / (double)fps);
}