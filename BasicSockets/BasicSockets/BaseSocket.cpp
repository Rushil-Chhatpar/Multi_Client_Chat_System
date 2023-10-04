#include "BaseSocket.h"

BaseSocket::BaseSocket(int addr_Family, int type, int protocol)
{
	m_Socket = socket(addr_Family, type, protocol);
}

BaseSocket::BaseSocket(SOCKET sock)
{
	m_Socket = sock;
}

BaseSocket::~BaseSocket()
{
	closesocket(m_Socket);
}

void BaseSocket::Bind(sockaddr_in service)
{
	if (bind(m_Socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		std::cout << "Server bind failed: " << WSAGetLastError() << "\n";
		std::cout << "\n\n\n\n\n\nCleanup result: " << WSACleanup() << "\n\n";
	}
	else
		std::cout << "Socket created successfully\n\n";
}

void BaseSocket::Listen(int backlog)
{
	if (listen(m_Socket, backlog) == SOCKET_ERROR)
		std::cout << "Server listen failed: " << WSAGetLastError() << "\n";
	else
		std::cout << "Server listening...\n\n";
}

SOCKET BaseSocket::Accept(sockaddr& addr, int& addrLength)
{
	addrLength = sizeof(addr);
	SOCKET acceptSocket = accept(m_Socket, &addr, &addrLength);
	if (acceptSocket == INVALID_SOCKET)
	{
		std::cout << "Accept connection failed: " << WSAGetLastError() << "\n";
		WSACleanup();
		return INVALID_SOCKET;
	}
	else
	{
		std::cout << "Accepted connection\n\n";
		return acceptSocket;
	}
}

void BaseSocket::Connect(sockaddr_in service)
{
	if (connect(m_Socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		std::cout << "Connection failed: " << WSAGetLastError() << "\n";
		WSACleanup();
		return;
	}
	else
	{
		std::cout << "Client connection: OK\n";
		std::cout << "Ready to send and receive data.\n\n";
	}
}

int BaseSocket::Select(SOCKET sock, int operation)
{
	int Result;

	//read
	if (operation == OP_READ)
	{
		do
		{
			FD_SET readSet;
			timeval timeVal;

			FD_ZERO(&readSet);
			FD_SET(sock, &readSet);

			timeVal.tv_sec = 1;

			Result = select(0, &readSet, NULL, NULL, &timeVal);

			if (Result > 0)
				return Result;

		} while (true);
	}

	// write
	else if (operation == OP_WRITE)
	{
		do
		{
			FD_SET writeSet;
			timeval timeVal;

			FD_ZERO(&writeSet);
			FD_SET(sock, &writeSet);

			Result = select(0, NULL, &writeSet, NULL, &timeVal);

			if (Result > 0)
				return Result;
		} while (true);
	}

	return 0;
}

int BaseSocket::SetMode(int mode)
{
	u_long arg = 0;
	if (mode == MODE_BLOCKING)
		arg = 0;
	else if (mode == MODE_NONBLOCKING)
		arg = 1;

	int Result;

	Result = ioctlsocket(m_Socket, FIONBIO, &arg);

	return Result;
}

int BaseSocket::Send(const char* buffer, int length)
{
	int byteCount;

	byteCount = send(m_Socket, buffer, length, 0);
	if (byteCount <= 0)
	{
		std::cout << "Message failed to send\n";
	}

	return byteCount;
}

int BaseSocket::Receive(std::string& message)
{
	int byteCount = 0;
	char buffer[512];

	byteCount = recv(m_Socket, buffer, 512, 0);
	if (byteCount == 0)
	{
		std::cout << "Connection failed\n";
	}
	else if (byteCount < 0)
	{
		std::cout << "Receive failed\n";
	}

	message = CharArrayToString(buffer);
	return byteCount;
}

std::string BaseSocket::CharArrayToString(char buffer[])
{
	int size = sizeof(buffer) / sizeof(char);
	std::string message = "";
	for (int i = 0; i < 512; i++)
	{
		if (buffer[i] == -52)
			break;
		message += buffer[i];
	}
	return message;
}
