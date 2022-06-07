#pragma once
// -------------------------------------- For Socket --------------------------------------
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
class SocketManager 
{
	
	
public:
	// -------------------------------------- For Socket --------------------------------------
	int title = 0;
	int iResult0 = 0;
	int iResult1 = 0;
	int iResult2 = 0;
	WSADATA wsaData;

	SOCKET RecvSocket0;
	struct sockaddr_in RecvAddr0;

	// by default use port 1243
	unsigned short DATA_PORT = 1243;
	unsigned short TITLE_PORT = 1245;

	char RecvBuf[38];
	int BufLen = 38;


	char RecvDataBuf0[38];
	int RecvBufLen0 = 38;
	char RecvDataBuf1[38];
	int RecvBufLen1 = 38;
	char RecvDataBuf2[38];
	int RecvBufLen2 = 38;

	struct sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	SocketManager();
	string GetTitle();
	vector<double> Read();
};