#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream> /*cout, endl*/
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

#include <string.h>

using namespace std::chrono;
using namespace std;

static const float VIEW_HEIGHT = 1080.0f;
static const float VIEW_WIDTH = 1920.0f;

#define SAMPLE_TIMESAMPLE_TIME = 15ms

// -------------------------------------- For Socket --------------------------------------
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

// -------------------------------------- For Socket --------------------------------------
int main()
{

    // -------------------------------------- For Socket --------------------------------------
	int title = 0;
	int iResult0 = 0;
	int iResult1 = 0;
	int iResult2 = 0;
	WSADATA wsaData;

	SOCKET RecvSocket0;
	struct sockaddr_in RecvAddr0;

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

	// ------------------------------------------------------
	// Initialize Winsock
	title = WSAStartup(MAKEWORD(2, 2), &wsaData);
	iResult0 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	iResult1 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	iResult2 = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult0 != NO_ERROR)
	{
		wprintf(L"WSAStartup failed with error %d\n", iResult0);
		return 1;
	}
	puts("Initialized Winsock...\n");
	// ------------------------------------------------------
	// Create a reciever socket to receive datagrams
	RecvSocket0 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (RecvSocket0 == INVALID_SOCKET)
	{
		wprintf(L"socket failed with error %d\n", WSAGetLastError());
		return 1;
	}
	puts("Receiver Socket Created...\n");
	// ------------------------------------------------------
	// Bind the socket to any address and the specified port
	RecvAddr0.sin_family = AF_INET;
	RecvAddr0.sin_port = htons(TITLE_PORT);
	RecvAddr0.sin_addr.s_addr = htonl(INADDR_ANY);

	title = bind(RecvSocket0, (SOCKADDR*)&RecvAddr0, sizeof(RecvAddr0));
	iResult0 = bind(RecvSocket0, (SOCKADDR*)&RecvAddr0, sizeof(RecvAddr0));
	iResult1 = bind(RecvSocket0, (SOCKADDR*)&RecvAddr0, sizeof(RecvAddr0));
	iResult2 = bind(RecvSocket0, (SOCKADDR*)&RecvAddr0, sizeof(RecvAddr0));

	if (iResult0 != 0 && iResult1 != 0 && iResult2 != 0 && title != 0)
	{
		wprintf(L"bind failed with error %d\n", WSAGetLastError());
	}
	puts("Bind Successful...\n");

	// get the title
	memset(RecvBuf, 0, BufLen);
	title = recvfrom(RecvSocket0, RecvBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
	auto expTitle = RecvBuf;
	if (title == SOCKET_ERROR)
	{
		wprintf(L"Recvfrom failed with error %d\n", WSAGetLastError());
		return 1;
	}
	printf("Title:%s\n", RecvBuf);

    auto next = steady_clock::now();
    auto prev = next - 5ms;

    int random_num;
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "SFML works!");

    // create default view
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));

    // for x & y axis (Graph)
    sf::VertexArray x_axis(sf::Lines);
    x_axis.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f));
    x_axis.append(sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f));

    sf::VertexArray y_axis(sf::Lines);
    y_axis.append(sf::Vector2f(100.f, 100.f));
    y_axis.append(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f));

	// new method to draw chart.
	// SFML lines cannot have a thickness
	// need to draw rectangles instead
	sf::VertexArray quad_x(sf::Quads, 4);
	// Top Left
	quad_x[0].position = sf::Vector2f(100.f, VIEW_HEIGHT - 100.f);
	quad_x[0].color = sf::Color::White;
	// Bottom Left
	quad_x[1].position = sf::Vector2f(100.f, VIEW_HEIGHT - 100.f + 10.f);
	quad_x[1].color = sf::Color::White;
	// Bottom Right
	quad_x[2].position = sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f + 10.f);
	quad_x[2].color = sf::Color::White;
	// Top Right
	quad_x[3].position = sf::Vector2f(VIEW_WIDTH - 100.f, VIEW_HEIGHT - 100.f);
	quad_x[3].color = sf::Color::White;

	// y-axis:
	sf::VertexArray quad_y(sf::Quads, 4);
	// Top Left
	quad_y[0].position = sf::Vector2f(100.f, 100.f);
	quad_y[0].color = sf::Color::White;
	// Bottom Left 
	quad_y[1].position = sf::Vector2f(100.f, VIEW_HEIGHT - 100.f);
	quad_y[1].color = sf::Color::White;
	// Bottom Right 
	quad_y[2].position = sf::Vector2f(110.f, VIEW_HEIGHT - 100.f);
	quad_y[2].color = sf::Color::White;
	// Top Right
	quad_y[3].position = sf::Vector2f(110.f, 100.f);
	quad_y[3].color = sf::Color::White;


    // data stream
    std::vector<sf::Vertex> m_verticies;
    m_verticies.push_back(sf::Vertex(sf::Vector2f(100.f, VIEW_HEIGHT - 100.f), sf::Color::Red));


    // (0,0) = (100.f, height - 100.f)

    uint16_t offset = 2;
    int scale = 10;

    view.setCenter(VIEW_WIDTH / 2, VIEW_HEIGHT / 2);

    while (window.isOpen())
    {   
		// This is where we need to read info from the socket.
		// can have data loop in here. 
		// ------------------------------------------------------ read from socket ------------------------------------------------------
		// // ------------------------------------------------------
		// call recvfrom function to receive datagrams on bound socket
		//wprintf(L"Receiving datagrams...\n");
		// recieving piTime
		memset(RecvDataBuf0, 0, RecvBufLen0);
		iResult0 = recvfrom(RecvSocket0, RecvDataBuf0, RecvBufLen0, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
		double piTime = strtod(RecvDataBuf0, NULL);
		if (iResult0 == SOCKET_ERROR)
		{
			wprintf(L"Recvfrom failed with error %d\n", WSAGetLastError());
			return 1;
		}

		// recieving emgGAS
		memset(RecvDataBuf1, 0, RecvBufLen1);
		iResult1 = recvfrom(RecvSocket0, RecvDataBuf1, RecvBufLen1, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
		double emgGAS = strtod(RecvDataBuf1, NULL);
		if (iResult1 == SOCKET_ERROR)
		{
			wprintf(L"Recvfrom failed with error %d\n", WSAGetLastError());
			return 1;
		}

		// recieving emgTA
		memset(RecvDataBuf2, 0, RecvBufLen2);
		iResult2 = recvfrom(RecvSocket0, RecvDataBuf2, RecvBufLen2, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
		double emgTA = strtod(RecvDataBuf2, NULL);
		if (iResult2 == SOCKET_ERROR)
		{
			wprintf(L"Recvfrom failed with error %d\n", WSAGetLastError());
			return 1;
		}

		printf("piTime: %.15g - emgGAS: %.15g - emgTA: %.15g\n", piTime, emgGAS, emgTA);
		// ------------------------------------------------------ end read from socket  ------------------------------------------------------


		// old - random_num instead of using actual data
        random_num = std::rand() % ((int)VIEW_HEIGHT - 200) + 1;
		// new - taking old emg data and plotting it
		// data is on scale 0.000199 to 0.0258
		double emgGasScaled = emgGAS * 34000;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (m_verticies.size() == VIEW_WIDTH / scale) {
            // start scrolling
            // erase first item in vector
            m_verticies.erase(m_verticies.begin());
            // push_back new vertex/point
            m_verticies.push_back(sf::Vertex(sf::Vector2f(230.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled), sf::Color::Red));
            offset += scale;
            view.setCenter(-(VIEW_HEIGHT/2) -300 + offset, VIEW_HEIGHT/2);

            // update the chart position
            /*x_axis[0].position = sf::Vector2f(sf::Vector2f(100.f + offset, VIEW_HEIGHT - 100.f));
            x_axis[1].position = sf::Vector2f(sf::Vector2f(-VIEW_WIDTH + 220.f + offset, VIEW_HEIGHT - 100.f));
            y_axis[0].position = sf::Vector2f(sf::Vector2f(-VIEW_WIDTH + 220.f + offset , 100.f));
            y_axis[1].position = sf::Vector2f(sf::Vector2f(-VIEW_WIDTH + 220.f + offset , VIEW_HEIGHT - 100.f));*/

			// Top Right
			quad_x[0].position = sf::Vector2f(offset, VIEW_HEIGHT - 100.f);
			// Bottom Right
			quad_x[1].position = sf::Vector2f(offset, VIEW_HEIGHT - 100.f + 10.f);
			// Bottom Left
			quad_x[2].position = sf::Vector2f(-VIEW_WIDTH + 220.f + offset, VIEW_HEIGHT - 100.f + 10.f);
			// Top Left
			quad_x[3].position = sf::Vector2f(- VIEW_WIDTH + 220.f + offset, VIEW_HEIGHT - 100.f);

			// Top Left
			quad_y[0].position = sf::Vector2f(-VIEW_WIDTH + 220.f + offset, 100.f);
			// Bottom Left 
			quad_y[1].position = sf::Vector2f(-VIEW_WIDTH + 220.f + offset, VIEW_HEIGHT - 100.f);
			// Bottom Right 
			quad_y[2].position = sf::Vector2f(-VIEW_WIDTH + 230.f + offset, VIEW_HEIGHT - 100.f);
			// Top Right
			quad_y[3].position = sf::Vector2f(-VIEW_WIDTH + 230.f + offset, 100.f);

        }else{
            m_verticies.push_back(sf::Vertex(sf::Vector2f(100.f + offset, VIEW_HEIGHT - 100.f - emgGasScaled), sf::Color::Red));
            offset += scale;
        }

        window.clear();
        window.draw(quad_x);
        window.draw(quad_y);
        window.setView(view);
        
        // old method of drawing live data
        window.draw(m_verticies.data(), m_verticies.size(), sf::TriangleStrip);

        window.display();
        

        // do stuff
        auto now = steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now-prev).count() << std::endl;
        prev = now;

        // delay until time to iterate again
        next += 5ms;
        std::this_thread::sleep_until(next);

    }

    return 0;
}