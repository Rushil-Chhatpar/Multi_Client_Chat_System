
#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include <mutex>
#include <thread>




using namespace std;

void CleanupWSADLL();

// globals :(
int port = 55555;
SOCKET listeningSocket;
SOCKET clientSockets[2];

// thread functions
void STDListeningThread();

int main()
{


    WSADATA wsaData;
    int wsaError;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaError = WSAStartup(wVersionRequested, &wsaData);
    if (wsaError != 0)
    {
        cout << "The Winsock dll not found :(\n";
        return 0;
    }
    else
    {
        cout << "The Winsock dll found :)\n";
        cout << "Status: " << wsaData.szSystemStatus << "\n\n";
    }

    // Create socket

    listeningSocket = INVALID_SOCKET;
    listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listeningSocket == INVALID_SOCKET)
    {
        cout << "Error creating socket: " << WSAGetLastError() << "\n";
        CleanupWSADLL();
        return;
    }
    else
    {
        cout << "Socket created successfully\n\n";
    }


    //Bind socket

    sockaddr_in listeningService;
    listeningService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &listeningService.sin_addr.s_addr); // OR &service.sin_addr.S_un.S_addr
    // Loopback address. https://www.geeksforgeeks.org/what-is-a-loopback-address/
    listeningService.sin_port = htons(port);

    if (bind(listeningSocket, (SOCKADDR*)&listeningService, sizeof(listeningService)) == SOCKET_ERROR)
    {
        cout << "Server bind failed: " << WSAGetLastError() << "\n";
        closesocket(listeningSocket);
        CleanupWSADLL();
        return;
    }
    else
    {
        cout << "Socket bind successful!\n\n";
    }


    // Listen on socket

    if (listen(listeningSocket, 1) == SOCKET_ERROR)
    {
        cout << "Server listen failed: " << WSAGetLastError() << "\n";
    }
    else
    {
        cout << "Server listening...\n\n";
    }









	return 0;
}

void CleanupWSADLL()
{
    // Shut down WSA DLL
    int cleanupError = WSACleanup();
    cout << "\n\n\n\n\n\nCleanup result: " << cleanupError << "\n\n";
}

void STDListeningThread()
{
    //// Create socket

    //listeningSocket = INVALID_SOCKET;
    //listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //if (listeningSocket == INVALID_SOCKET)
    //{
    //    cout << "Error creating socket: " << WSAGetLastError() << "\n";
    //    CleanupWSADLL();
    //    return;
    //}
    //else
    //{
    //    cout << "Socket created successfully\n\n";
    //}

    //
    ////Bind socket

    //sockaddr_in listeningService;
    //listeningService.sin_family = AF_INET;
    //InetPton(AF_INET, _T("127.0.0.1"), &listeningService.sin_addr.s_addr); // OR &service.sin_addr.S_un.S_addr
    //// Loopback address. https://www.geeksforgeeks.org/what-is-a-loopback-address/
    //listeningService.sin_port = htons(port);

    //if (bind(listeningSocket, (SOCKADDR*)&listeningService, sizeof(listeningService)) == SOCKET_ERROR)
    //{
    //    cout << "Server bind failed: " << WSAGetLastError() << "\n";
    //    closesocket(listeningSocket);
    //    CleanupWSADLL();
    //    return;
    //}
    //else
    //{
    //    cout << "Socket bind successful!\n\n";
    //}


    //// Listen on socket

    //if (listen(listeningSocket, 1) == SOCKET_ERROR)
    //{
    //    cout << "Server listen failed: " << WSAGetLastError() << "\n";
    //}
    //else
    //{
    //    cout << "Server listening...\n\n";
    //}


}
