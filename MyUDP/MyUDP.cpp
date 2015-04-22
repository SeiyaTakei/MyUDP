#pragma once
#include "MyUDP.h"
#include <iostream>

using namespace std;

MyUDP::MyUDP() : isInitialized(false), wait_send(0), wait_receive(0), sendData(NULL), receiveData(NULL) {
}

MyUDP::~MyUDP() {
}

void MyUDP::init(unsigned short port_send, unsigned short port_receive) {
	portnum_send = port_send;
	portnum_receive = port_receive;

	send_count_start = clock();
	receive_count_start = clock();

	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 0), &wsadata)) {
		cout << "WSAStartup Faild." << endl;
		return;
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << stderr << " : Socket Error" << endl;
		WSACleanup();
		return;
	}
	cout << stderr << " : Socket Created" << endl;

	//デフォルトではSend, Receiveともに"127.0.0.1"(ローカルループバック)に設定
	memset(&sockAddr_receive, 0, sizeof(sockAddr_receive));
	sockAddr_receive.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr_receive.sin_port = htons(portnum_receive);
	sockAddr_receive.sin_family = AF_INET;
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);
	if (bind(sock, (struct sockaddr *)&sockAddr_receive, sizeof(sockAddr_receive)) == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();
		return;
	}

#pragma region SendSocketSetup
	memset(&sockAddr_send, 0, sizeof(sockAddr_send));
	sockAddr_send.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr_send.sin_port = htons(portnum_send);
	sockAddr_send.sin_family = AF_INET;
#pragma endregion

	isInitialized = true;
}

void MyUDP::setSendData(int send_length, char *sd) {
	sendLength = send_length;
	sendData = sd;
}

void MyUDP::setReceiveData(int receive_length, char *rd) {
	receiveLength = receive_length;
	receiveData = rd;
}

void MyUDP::setIPAddress(char* myIP, char* targetIP) {
	sockAddr_send.sin_addr.s_addr = inet_addr(targetIP);
	sockAddr_receive.sin_addr.s_addr = inet_addr(myIP);
	//Socketの作り直し
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << stderr << " : IPAddress configuration failed" << endl;
		isInitialized = false;
		WSACleanup();
		return;
	}
	if (bind(sock, (struct sockaddr *)&sockAddr_receive, sizeof(sockAddr_receive)) == SOCKET_ERROR) {
		closesocket(sock);
		cout << stderr << " : IPAddress configuration failed" << endl;
		WSACleanup();
		isInitialized = false;
		return;
	}
	cout << stderr << " : IPAddress configuration succeeded" << endl;
}

void MyUDP::update() {
	updateSend();
	updateReceive();
}

void MyUDP::close() {
	if (isInitialized) {
		closesocket(sock);
	}
	WSACleanup();
}


void MyUDP::updateSend() {
	if (isInitialized) {
		if (portnum_send != 0) {
			send_count = clock();
			if (send_count - send_count_start > wait_send) {
				sendto(sock, sendData, sendLength, 0, (const struct sockaddr *)&sockAddr_send, sizeof(sockAddr_send));
				send_count_start = clock();
			}
		}
	}
}

void MyUDP::updateReceive() {
	int num;
	if (isInitialized) {
		if (portnum_receive != 0) {
			receive_count = clock();
			if (receive_count - receive_count_start > wait_receive) {
				num = recv(sock, receiveData, receiveLength, 0);
				receive_count_start = clock();
			}
		}
	}
}

void MyUDP::setSendFPS(int fps) {
	if (fps <= 0) {
		wait_send = 0;
		return;
	}
	wait_send = (int)(1000 * 1.0 / (double)fps);
}

void MyUDP::setReceiveFPS(int fps) {
	if (fps <= 0) {
		wait_receive = 0;
		return;
	}
	wait_receive = (int)(1000 * 1.0 / (double)fps);
}