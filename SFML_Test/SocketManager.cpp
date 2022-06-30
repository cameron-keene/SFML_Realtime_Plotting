#include "SocketManager.h"

SocketManager::SocketManager()
{
	// ------------------------------------------------------
	// Initialize Winsock
	this->title = WSAStartup(MAKEWORD(2, 2), &wsaData);
	this->iResult0 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	this->iResult1 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	this->iResult2 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (this->iResult0 != NO_ERROR)
	{
		wprintf(L"WSAStartup failed with error %d\n", this->iResult0);
		//return 1;
	}
	puts("Initialized Winsock...\n");
	// ------------------------------------------------------
	// Create a reciever socket to receive datagrams
	this->RecvSocket0 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (this->RecvSocket0 == INVALID_SOCKET)
	{
		wprintf(L"socket failed with error %d\n", WSAGetLastError());
		//return 1;
	}
	puts("Receiver Socket Created...\n");
	// ------------------------------------------------------
	// Bind the socket to any address and the specified port
	this->RecvAddr0.sin_family = AF_INET;
	this->RecvAddr0.sin_port = htons(this->TITLE_PORT);
	this->RecvAddr0.sin_addr.s_addr = htonl(INADDR_ANY);

	this->title = bind(this->RecvSocket0, (SOCKADDR*)&this->RecvAddr0, sizeof(this->RecvAddr0));
	this->iResult0 = bind(this->RecvSocket0, (SOCKADDR*)&this->RecvAddr0, sizeof(this->RecvAddr0));
	this->iResult1 = bind(this->RecvSocket0, (SOCKADDR*)&this->RecvAddr0, sizeof(this->RecvAddr0));
	this->iResult2 = bind(this->RecvSocket0, (SOCKADDR*)&this->RecvAddr0, sizeof(this->RecvAddr0));

	if (this->iResult0 != 0 && this->iResult1 != 0 && this->iResult2 != 0 && this->title != 0)
	{
		wprintf(L"bind failed with error %d\n", WSAGetLastError());
	}
	puts("Bind Successful...\n");

	memset(this->RecvBuf, 0, this->BufLen);
	memset(this->RecvDataBuf0, 0, this->RecvBufLen0);
	memset(this->RecvDataBuf1, 0, this->RecvBufLen1);
	memset(this->RecvDataBuf2, 0, this->RecvBufLen2);

}

string SocketManager::GetTitle() {
	// get the title
	memset(this->RecvBuf, 0, this->BufLen);
	this->title = recvfrom(this->RecvSocket0, this->RecvBuf, this->BufLen, 0, (SOCKADDR*)&this->SenderAddr, &this->SenderAddrSize);
	auto expTitle = this->RecvBuf;
	if (this->title == SOCKET_ERROR)
	{
		wprintf(L"Recvfrom failed with error %d\n", WSAGetLastError());
		//return 1;
	}
	cout << "Title: " << expTitle << endl;
	return expTitle;
}
vector<double> SocketManager::Read() {
	vector<double> result;

	// recieving PiTime
	memset(this->RecvDataBuf0, 0, this->RecvBufLen0);
	this->iResult0 = recvfrom(this->RecvSocket0, this->RecvDataBuf0, this->RecvBufLen0, 0, (SOCKADDR*)&this->SenderAddr, &this->SenderAddrSize);
	double piTime = strtod(this->RecvDataBuf0, NULL);
	//cout << "piTime: " << piTime << endl;
	if (this->iResult0 == SOCKET_ERROR)
	{
		wprintf(L"Recvfrom0 failed with error %d\n", WSAGetLastError());
	}
	result.push_back(piTime);

	// recieving emgGAS
	memset(this->RecvDataBuf1, 0, this->RecvBufLen1);
	this->iResult1 = recvfrom(this->RecvSocket0, this->RecvDataBuf1, this->RecvBufLen1, 0, (SOCKADDR*)&this->SenderAddr, &this->SenderAddrSize);
	double emgGAS = strtod(this->RecvDataBuf1, NULL);
	if (this->iResult1 == SOCKET_ERROR)
	{
		wprintf(L"Recvfrom1 failed with error %d\n", WSAGetLastError());
	}
	result.push_back(emgGAS);

	// recieving emgTA
	memset(this->RecvDataBuf2, 0, this->RecvBufLen2);
	this->iResult2 = recvfrom(this->RecvSocket0, this->RecvDataBuf2, this->RecvBufLen2, 0, (SOCKADDR*)&this->SenderAddr, &this->SenderAddrSize);
	double emgTA = strtod(this->RecvDataBuf2, NULL);
	if (this->iResult2 == SOCKET_ERROR)
	{
		wprintf(L"Recvfrom2 failed with error %d\n", WSAGetLastError());
	}
	result.push_back(emgTA);

	//printf("piTime: %.15g - emgGAS: %.15g - emgTA: %.15g\n", piTime, emgGAS, emgTA);

	return result;
}

int SocketManager::GetDuration()
{
	memset(this->DurBuf, 0, this->DurBufLen);
	this->trial_duration = recvfrom(this->RecvSocket0, this->DurBuf, this->DurBufLen, 0, (SOCKADDR*)&this->SenderAddr, &this->SenderAddrSize);
	string Dur = this->DurBuf;
	if (this->title == SOCKET_ERROR)
	{
		wprintf(L"Recvfrom failed with error %d\n", WSAGetLastError());
		//return 1;
	}
	cout << "Value: " << Dur << endl;
	return stoi(Dur);
}
