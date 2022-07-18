#pragma once
// -------------------------------------- For Socket --------------------------------------
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define SERVERIP "192.168.10.144"
#define PORT 1247

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <tchar.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class TestManager {
public:
	string testType;
	int iResult0 = 0;
	const char* sendbuf;
	string ip_address;
	SOCKET outGoingSocket;
	WSADATA wsaData;
	struct sockaddr_in SendAddr0;

	// by default use port 1243
	unsigned short DATA_PORT = 1247;

	TestManager(string _testType);
	void RunTest();

};