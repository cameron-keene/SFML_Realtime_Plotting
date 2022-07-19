#pragma once
// -------------------------------------- For Socket --------------------------------------
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_PORT "1245"
#define DEFAULT_BUFLEN (37-18)

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <string>
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
class SocketManagerTCP
{


public:
	// int title = 0;
	// int trial_duration = 0;
	int msgResult = 0;
	int iResult0 = 0;
	int iResult1 = 0;
	int iResult2 = 0;
	WSADATA wsaData;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	
	struct addrinfo* result = NULL;
	struct addrinfo hints;


	// by default use port 1243
	unsigned short DATA_PORT = 1243;
	unsigned short TITLE_PORT = 1245;
	
	int iSendResult;

	char recvbuf0[DEFAULT_BUFLEN];
	int recvbuflen0 = DEFAULT_BUFLEN;
	
	char msgSizeBuf[14];
	int msgSizeBufLen = 14;
	

	struct sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	SocketManagerTCP();
	//string GetTitle();
	double Read();
	vector<double> ReadAll();
	//int GetDuration();
	void CreateSocket();
	string GetTitle();
	int GetDuration();
};