// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <tchar.h>
#include "BaseSocket.h"
#include <thread>
#include <mutex>
#include <string>
#include <iostream>

using namespace std;


void SendingThread(BaseSocket* socket);
void ReceivingThread(BaseSocket* socket);

string Name = "";


void SetupWSADLL();
void CleanupWSADLL();

int main()
{
    SetupWSADLL();
    int port = 55555;

    BaseSocket* clientSocket = new BaseSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr); // OR &service.sin_addr.S_un.S_addr
    service.sin_port = htons(port);

    clientSocket->Connect(service);

    clientSocket->SetMode(BaseSocket::MODE_BLOCKING);

    cout << "Enter your name:\n";
    getline(cin, Name);

    const char* message = Name.c_str();
    clientSocket->Send(message, (int)strlen(message));


    thread receiving(ReceivingThread, clientSocket);
    receiving.detach();
    thread sending(SendingThread, clientSocket);
    sending.join();




    CleanupWSADLL();


    system("pause");
}

void SendingThread(BaseSocket* socket)
{
    do
    {
        string message;
        cout << Name + ": ";
        getline(cin, message);
        const char* sendMessage = message.c_str();
        int result = socket->Send(sendMessage, (int)strlen(sendMessage));
        if (result == SOCKET_ERROR)
            return;
    } while (true);
}

void ReceivingThread(BaseSocket* socket)
{
    do
    {
        string message = "";
        int result = socket->Receive(message);
        if (result == SOCKET_ERROR)
            return;
        cout << "\n" + message + "\n";
    } while (true);
}

void SetupWSADLL()
{
    WSADATA wsaData;
    int wsaError;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaError = WSAStartup(wVersionRequested, &wsaData);
    if (wsaError != 0)
    {
        cout << "The Winsock dll not found :(\n";
        return;
    }
    else
    {
        cout << "The Winsock dll found :)\n";
        cout << "Status: " << wsaData.szSystemStatus << "\n\n";
    }
}

void CleanupWSADLL()
{
    int cleanupError = WSACleanup();
    cout << "\n\n\n\n\n\nCleanup result: " << cleanupError << "\n\n";
}
