#include "SocketManager.h"

SocketManagerTCP::SocketManagerTCP()
{
	
}
void SocketManagerTCP::CreateSocket() {
	// ------------------------------------------------------
// Initialize Winsock
	this->iResult0 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	this->iResult1 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	this->iResult2 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (this->iResult0 != NO_ERROR && this->iResult1 != NO_ERROR && this->iResult2 != NO_ERROR)
	{
		wprintf(L"WSAStartup failed with error %d\n", this->iResult0);
		//return 1;
	}
	//puts("Initialized Winsock...\n");
	// ------------------------------------------------------
	ZeroMemory(&hints, sizeof(hints));
	this->hints.ai_family = AF_INET;
	this->hints.ai_socktype = SOCK_STREAM;
	this->hints.ai_protocol = IPPROTO_TCP;
	this->hints.ai_flags = AI_PASSIVE;
	// ------------------------------------------------------
	// Resolve the server address and port
	this->iResult0 = getaddrinfo(NULL, DEFAULT_PORT, &this->hints, &this->result);
	this->iResult1 = getaddrinfo(NULL, DEFAULT_PORT, &this->hints, &this->result);
	this->iResult2 = getaddrinfo(NULL, DEFAULT_PORT, &this->hints, &this->result);
	if (this->iResult0 != 0) { //&& this->iResult1 != 0 && this->iResult2 != 0 && this->title != 0
		printf("getaddrinfo failed with error: %d\n", this->iResult0);
		WSACleanup();
	}
	// Create a SOCKET for connecting to server
	this->ListenSocket = socket(this->result->ai_family, this->result->ai_socktype, this->result->ai_protocol);
	if (this->ListenSocket == INVALID_SOCKET)
	{
		wprintf(L"socket failed with error %d\n", WSAGetLastError());
		//return 1;
	}
	//puts("Listen Socket Created...\n");
	// ------------------------------------------------------
	// Setup the TCP listening socket
	this->iResult0 = bind(this->ListenSocket, this->result->ai_addr, (int)this->result->ai_addrlen);
	this->iResult1 = bind(this->ListenSocket, this->result->ai_addr, (int)this->result->ai_addrlen);
	this->iResult2 = bind(this->ListenSocket, this->result->ai_addr, (int)this->result->ai_addrlen);

	if (this->iResult0 != 0)
	{
		wprintf(L"bind failed with error %d\n", WSAGetLastError());
		// clean up
		freeaddrinfo(this->result);
		closesocket(this->ListenSocket);
		WSACleanup();
	}
	//puts("Bind Successful...\n");

	freeaddrinfo(this->result);

	// ------------------------------------------------------
	this->iResult0 = listen(this->ListenSocket, SOMAXCONN);
	this->iResult1 = listen(this->ListenSocket, SOMAXCONN);
	this->iResult2 = listen(this->ListenSocket, SOMAXCONN);
	if (this->iResult0 == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(this->ListenSocket);
		WSACleanup();
		//return 1;
	}
	//puts("listening ...\n");

	// ------------------------------------------------------
	// Accept a client socket
	this->ClientSocket = accept(this->ListenSocket, NULL, NULL);
	if (this->ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(this->ListenSocket);
		WSACleanup();
		//return 1;
	}
	//puts("Accepted ...\n");

	// No longer need server socket
	closesocket(this->ListenSocket);
}



double SocketManagerTCP::Read() {
	double result;
	// Load the PC socket
	this->CreateSocket();
	
	// begin loop
	for (int i = 0; i < 3; i++) 
	{
		// initialize the memory to 0
		ZeroMemory(this->recvbuf0, this->recvbuflen0);
		ZeroMemory(this->msgSizeBuf, this->msgSizeBufLen);
		// recieve the size of message to be recieved.
		this->msgResult = recv(this->ClientSocket, this->msgSizeBuf, this->msgSizeBufLen, 0);
		int msgSize = stoi(this->msgSizeBuf);
		cout << "msgSize: " << msgSize << endl;
		// read the message based upon the size sent before
		// TCP header size seems to be around 18 bytes, remove this. 
		this->iResult0 = recv(this->ClientSocket, this->recvbuf0, (msgSize - 18), 0);
		// check if message read properly
		if (this->iResult0 > 0) {
			cout << "Read Result0: " << this->recvbuf0 << endl;
			result = stod(this->recvbuf0);
		}
		else { // close if failed and print error message
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(this->ClientSocket);
			WSACleanup();
		}
		// repeat however times needed
	}
	

	// shutdown the connection since we're done
	this->iResult0 = shutdown(this->ClientSocket, SD_SEND);
	if (this->iResult0 == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(this->ClientSocket);
		WSACleanup();
	}
	// cleanup
	closesocket(this->ClientSocket);
	WSACleanup();

	return result;
}

vector<double> SocketManagerTCP::ReadAll() {
	vector<double> result;
	for (int i = 0; i < 3; i++) {
		this->Read();
	}
	return result;
}

int SocketManagerTCP::GetDuration()
{	
	int result;
	this->CreateSocket();
	do {
		ZeroMemory(this->recvbuf0, this->recvbuflen0);
		this->iResult0 = recv(this->ClientSocket, this->recvbuf0, this->recvbuflen0, 0);
		result = stoi(this->recvbuf0);
		if (this->iResult0 > 0) {
			cout << "Read Result: " << result << endl;
			return result;
		}
		else if (this->iResult0 == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(this->ClientSocket);
			WSACleanup();
		}
	} while (this->iResult0 > 0);
	// shutdown the connection since we're done
	this->iResult0 = shutdown(this->ClientSocket, SD_SEND);
	if (this->iResult0 == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(this->ClientSocket);
		WSACleanup();
	}
	// cleanup
	closesocket(this->ClientSocket);
	WSACleanup();
	return result;
}
string SocketManagerTCP::GetTitle() {
	string result;
	this->CreateSocket();
	do {
		ZeroMemory(this->recvbuf0, this->recvbuflen0);
		this->iResult0 = recv(this->ClientSocket, this->recvbuf0, this->recvbuflen0, 0);
		result = this->recvbuf0;
		if (this->iResult0 > 0) {
			cout << "Read Result: " << result << endl;
			return result;
		}
		else if (this->iResult0 == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(this->ClientSocket);
			WSACleanup();
		}
	} while (this->iResult0 > 0);
	// shutdown the connection since we're done
	this->iResult0 = shutdown(this->ClientSocket, SD_SEND);
	if (this->iResult0 == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(this->ClientSocket);
		WSACleanup();
	}
	// cleanup
	closesocket(this->ClientSocket);
	WSACleanup();
	return result;
}