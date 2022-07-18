#include "TestManager.h"

TestManager::TestManager(string _testType) {
	this->testType = _testType;
	this->sendbuf = "this is a test";
	this->ip_address = "192.168.10.144";
}

void TestManager::RunTest() {
    int iResult;
    WSADATA wsaData;

    SOCKET SendSocket = INVALID_SOCKET;
    sockaddr_in RecvAddr;

    unsigned short Port = 27015;

    //char SendBuf[1024];
    //int BufLen = 1024;

    char SendBuf[] = "run";
    int BufLen = sizeof(SendBuf);

    string mesg = "Testing Sending to Socket";

    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %d\n", iResult);
        //return 1;
    }

    //---------------------------------------------
    // Create a socket for sending data
    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (SendSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        //return 1;
    }
    //---------------------------------------------
    // Set up the RecvAddr structure with the IP address of
    // the receiver (in this example case "192.168.1.1")
    // and the specified port number.
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_port = htons(PORT);
    //RecvAddr.sin_addr.s_addr = inet_addr(SERVERIP);
    InetPton(AF_INET, _T(SERVERIP), &RecvAddr.sin_addr.s_addr);

    //---------------------------------------------
    // Send a datagram to the receiver
    wprintf(L"Sending a datagram to the receiver...\n");
    iResult = sendto(SendSocket,
        SendBuf, BufLen, 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
        closesocket(SendSocket);
        WSACleanup();
        //return 1;
    }
    //---------------------------------------------
       
    // need to wait for response that done sending data
    // then exit/close socket
    
    
    
    // When the application is finished sending, close the socket.
    wprintf(L"Finished sending. Closing socket.\n");
    iResult = closesocket(SendSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        //return 1;
    }
    //---------------------------------------------
    // Clean up and quit.
    wprintf(L"Exiting.\n");
    WSACleanup();
    //return 0;

}