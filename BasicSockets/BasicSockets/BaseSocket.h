// I followed the following link on how to set up a static library
// https://learn.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-static-library-cpp?view=msvc-170

#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

class BaseSocket
{
public:

	enum OPERATION
	{
		OP_READ,
		OP_WRITE,
	};

	enum MODE
	{
		MODE_BLOCKING,
		MODE_NONBLOCKING,
	};

	BaseSocket(int addr_Family, int type, int protocol);
	BaseSocket(SOCKET sock);

	~BaseSocket();

	void Bind(sockaddr_in service);

	void Listen(int backlog);

	SOCKET Accept(sockaddr& addr, int& addrLength);

	void Connect(sockaddr_in service);

	int Select(SOCKET sock, int operation);

	int SetMode(int mode);

	int Send(const char* buffer, int length);

	int Receive(std::string& message);

	SOCKET GetSocket() { return m_Socket; }

protected:
	SOCKET m_Socket;

private:
	std::string CharArrayToString(char buffer[]);
};