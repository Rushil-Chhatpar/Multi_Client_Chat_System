// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <tchar.h>
#include "BaseSocket.h"
#include "ClientConnection.h"
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
using namespace std;


void InitializeClient(BaseSocket* acceptSocket, sockaddr addr, int addrlen);
void ReceiveFromClient(ClientConnection* client);


void SetupWSADLL();
void CleanupWSADLL();

mutex Mut;
vector<ClientConnection*> Clients;

int main()
{
    SetupWSADLL();
    int port = 55555;

    BaseSocket* serverSocket = new BaseSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr); // OR &service.sin_addr.S_un.S_addr
    service.sin_port = htons(port);

    serverSocket->Bind(service);

    serverSocket->SetMode(BaseSocket::MODE_NONBLOCKING);

    serverSocket->Listen(1);

    serverSocket->SetMode(BaseSocket::MODE_BLOCKING);

    do
    {
        sockaddr addr;
        int addrlen;
        BaseSocket* acceptSocket = new BaseSocket(serverSocket->Accept(addr, addrlen));
        thread initClient(InitializeClient, acceptSocket, addr, addrlen);
        initClient.detach();
    } while (true);

        


    CleanupWSADLL();
}

void InitializeClient(BaseSocket* acceptSocket, sockaddr addr, int addrlen)
{
    Mut.lock();
    string name;
    acceptSocket->Receive(name);
    ClientConnection* clientConnection = new ClientConnection(acceptSocket, name, addr);
    Clients.push_back(clientConnection);
    thread receiveFromClient(ReceiveFromClient, clientConnection);
    receiveFromClient.detach();
    Mut.unlock();
}

void ReceiveFromClient(ClientConnection* client)
{
    do
    {
        string message = "";
        int result = client->GetSocket()->Receive(message);
        if (result == SOCKET_ERROR)
        {
            // close the client
            Mut.lock();
            int index = 0;
            for (int i = 0; i < Clients.size(); i++)
            {
                if (Clients[i] == client)
                {
                    index = i;
                    break;
                }
            }
            Clients.erase(Clients.begin() + index);
            delete client;
            Mut.unlock();
            break;
        }
        string sMessage = client->GetName() + ": " + message;
        Mut.lock();
        for (int i = 0; i < Clients.size(); i++)
        {
            if (client == Clients[i])
                continue;
            const char* sendMessage = sMessage.c_str();
            Clients[i]->GetSocket()->Send(sendMessage, (int)strlen(sendMessage) + 1);
        }
        Mut.unlock();
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

