#include "ClientConnection.h"
#include "BaseSocket.h"

ClientConnection::ClientConnection(BaseSocket* socket, std::string name, sockaddr addr)
	: m_Socket(socket)
	, m_Name(name)
	, m_Addr(addr)
{

}

ClientConnection::~ClientConnection()
{

}
