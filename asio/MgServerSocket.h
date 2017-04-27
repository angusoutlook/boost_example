#ifndef _MG_SERVER_SOCKET_H_
#define _MG_SERVER_SOCKET_H_

#include <iostream>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>


#define PORT 5150

class CMgServerSocket
{
public:
	CMgServerSocket();
	virtual ~CMgServerSocket();

	bool	CreateSockListen();
	bool	Bind();
	bool	Listen(int nBacklog);
	
	int	    Accept();
	int	    Read(int sock, void* buf, int buflen);
	bool	Send(int sock, const void* buf, int buflen);
	int	    GetSockListen() { return m_SockListen; }
	int	    GetSockAccept() { return m_SockAccept; }
	
	void	CloseSockAccept();
	void	ClostSockListen();

protected:
	int	    m_nLastError;
	bool	m_bBind;
	bool	m_bAccepted;
	bool	m_bSockListenValid;
	bool	m_bSockAcceptValid;

private:
	int	    m_SockListen;
	int	    m_SockAccept;
	
	struct 	sockaddr_in	m_addrServer;
};

#endif
