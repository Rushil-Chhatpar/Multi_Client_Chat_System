#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

class BaseSocket;

class ClientConnection
{
public:
	ClientConnection(BaseSocket* socket, std::string name, sockaddr addr);
	~ClientConnection();

	BaseSocket* GetSocket()					{ return m_Socket; }
	std::string GetName()					{ return m_Name; }

protected:
	BaseSocket* m_Socket;
	std::string m_Name;
	sockaddr m_Addr;
};