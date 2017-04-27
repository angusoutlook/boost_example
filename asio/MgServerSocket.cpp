#include "MgServerSocket.h"

const int gReceiveTimeOut = 5;

CMgServerSocket::CMgServerSocket(void)
{
	m_bSockListenValid = false;
	m_bSockAcceptValid = false;
	m_bBind = false;
	m_bAccepted = false;
	m_nLastError = 0;
}

CMgServerSocket::~CMgServerSocket(void)
{
	ClostSockListen();
}

bool CMgServerSocket::CreateSockListen()
{
	m_SockListen = socket( AF_INET, SOCK_STREAM, 0 );
	if ( m_SockListen == -1 ) 
	{
		m_nLastError = 0x01;
		return false;
	}
	else 
	{
		m_bSockListenValid = true;
		return true;
	}
}

bool CMgServerSocket::Bind( void )
{
	m_addrServer.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons( PORT );

	if (SOCKET_ERROR == bind( m_SockListen, ( struct sockaddr* ) & m_addrServer, sizeof( struct sockaddr ) ) )
	{
		//printf("Error at socket(): %ld\n", WSAGetLastError());
		return false;
	}
	else
	{
		m_bBind = true;
		return true;
	}
}

bool CMgServerSocket::Listen(int nBacklog)
{
	if ( SOCKET_ERROR == listen( m_SockListen, nBacklog ) )
	{
		//printf("Error at socket(): %ld\n", WSAGetLastError());
		return false;
	}
	return true;
}

int CMgServerSocket::Accept()
{
	int len;
	len = sizeof( struct sockaddr );
	m_SockAccept = accept( m_SockListen, ( struct sockaddr* ) & m_addrServer, &len );
	if ( m_SockAccept == INVALID_SOCKET )
	{
		//printf("Error at socket(): %ld\n", WSAGetLastError());
		return 0;
	}
	else
	{
		m_bAccepted = true;
		return m_SockAccept;
	}
}

//return bytes
int CMgServerSocket::Read(int sock, const void* buf, int buflen)
{
	if ( buflen <= 0 || buf == NULL ) 
	{
		m_nLastError = 0x0C;
		return -1;
	}

	int nret = recv(sock, buf, buflen, 0);
	if (nret < 0) 
	{
		m_nLastError = 0x0D;
		CloseSockAccept();
		return -1;
	}
	else if ( nret == 0 ) 
	{
		m_nLastError = 0x0E;
		CloseSockAccept();
		return 0;
	}
	return nret;
}

bool CMgServerSocket::Send(int sock, const void* buf, int buflen)
{
	int pos = 0;
	int nRet = 0;
	char* movehead = (char*)buf;

	while (pos < buflen) 
	{

		nRet = send(sock, (void*)(movehead + pos), buflen - pos, 0 /*flags*/ );
		if (nRet == 0) 
		{
			m_nLastError = 0x08;
			std::cout<<"0x08: send find connection closed;"<<std::endl;
			CloseSockAccept();
			return false;
		}
		else if (nRet < 0) 
		{
			m_nLastError = 0x09;
			std::cout<<"0x09: send find network error;"<<std::endl;
			CloseSockAccept();
			return false;
		}
		else 
		{
			pos += nRet;
			if ( pos != buflen ) 
			{
				continue;
			}
			else 
			{
				return true;
			}
		}
	}
	m_nLastError = 0x0A;
	return false;
}

void CMgServerSocket::CloseSockAccept()
{
	if ( m_bSockAcceptValid ) 
	{
// 		if ( m_bAccepted )
			shutdown( m_SockAccept, 0 );
		close( m_SockAccept );
		m_bSockAcceptValid = false;
		m_bAccepted = false;
	}
}

void CMgServerSocket::ClostSockListen()
{
	if ( m_bSockListenValid ) 
	{
		if ( m_bBind )
		{
			shutdown( m_SockListen, 0 );
			m_bBind = false;
		}
		close( m_SockListen );
	}
	m_bSockListenValid = false;
}

