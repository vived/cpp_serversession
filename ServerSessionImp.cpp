#include "StdAfx.h"
#include "ServerSessionImp.h"
#include "ServerSession.h"

#include "boost/asio.hpp"
#include "boost/asio/deadline_timer.hpp"

using namespace boost::asio;
using namespace boost::asio::ip;

CServerSessionImp::CServerSessionImp(std::string strServerIp, const int &iServerPort )
	: m_strServerIp(strServerIp), m_iServerPort(iServerPort)
{
	m_pIoService = new boost::asio::io_service();
	m_pSock = new boost::asio::ip::tcp::socket(*m_pIoService);
}

CServerSessionImp::~CServerSessionImp(void)
{
	if (m_pSock->is_open())
	{
		m_pSock->close();		
	}
	delete m_pSock;
	m_pSock = NULL;
	delete m_pIoService;
	m_pIoService = NULL;
}

int CServerSessionImp::BindLocalIp( const std::string &strLocalIp, const int &iLocalPort )
{
	try
	{
		if (!strLocalIp.empty())
		{
			boost::system::error_code ec;
			ec.clear();
			boost::asio::ip::tcp::endpoint endLocal(boost::asio::ip::address_v4::from_string(strLocalIp), iLocalPort); 		
			m_pSock->open(boost::asio::ip::tcp::v4());
			boost::asio::socket_base::reuse_address option(true);
			m_pSock->set_option(option);
			m_pSock->bind(endLocal, ec);
			if (ec)
			{
				return ec.value();
			}
		}	
	}
	catch (...)
	{
		return EXE_FAILED;
	}	
	return EXE_SUCCESS;
}

int CServerSessionImp::Connect( const unsigned short &usWaitsecs /*= 0*/ )
{
	boost::system::error_code ec;
	if (!m_pSock->is_open())
	{
		m_pSock->open(boost::asio::ip::tcp::v4());	
		boost::asio::socket_base::reuse_address option(true);
		m_pSock->set_option(option);
	}		

	tcp::endpoint endPoint(boost::asio::ip::address_v4::from_string(m_strServerIp), m_iServerPort);
	if (0 == usWaitsecs)
	{	
		m_pSock->connect(endPoint, ec);
		if (ec)
		{
			m_pSock->close();
			return ec.value();
		}
	}
	else
	{
#ifdef WIN32

		m_pSock->non_blocking(true, ec);
		boost::asio::detail::socket_ops::connect(m_pSock->native_handle(), endPoint.data(), endPoint.size(), ec);

		if (ec.value() != 0 && ec.value() != boost::asio::error::would_block)
		{
			m_pSock->non_blocking(false, ec);
			m_pSock->close();
			return ec.value();
		}
		fd_set fdWrite;
		FD_ZERO(&fdWrite);
		FD_SET(m_pSock->native(), &fdWrite);
		timeval tv = {usWaitsecs};
		if (select(0, NULL, &fdWrite, NULL, &tv) <=0
			|| !FD_ISSET(m_pSock->native(), &fdWrite))
		{
			m_pSock->non_blocking(false, ec);
			m_pSock->close();
			return TIME_OUT;
		}
		m_pSock->non_blocking(false, ec);
#else
		return NOT_SUPPORT_PLATFORM;
#endif
	}
	return EXE_SUCCESS;
}

int CServerSessionImp::Send( const char* pSendBuf, size_t szBufSize, int &iErrorCode, const unsigned short &usWaitsecs /*= 0*/ )
{
	iErrorCode = 0;
	boost::system::error_code ec;
	int szSend = 0;
	if (0 == usWaitsecs)
	{
		szSend = m_pSock->write_some(boost::asio::buffer(pSendBuf, szBufSize), ec);
		iErrorCode = ec.value();		
	}
	else
	{
		if (!m_pSock->is_open())
		{
			m_pSock->open(boost::asio::ip::tcp::v4());
		}		

#ifdef WIN32
		m_pSock->non_blocking(true, ec);

		szSend = send(m_pSock->native_handle(), pSendBuf, szBufSize, 0);
		if (szSend == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK)
		{
			fd_set fdWrite;
			FD_ZERO(&fdWrite);
			FD_SET(m_pSock->native(), &fdWrite);
			timeval tv = {usWaitsecs};
			if (select(0, NULL, &fdWrite, NULL, &tv) <=0
				|| !FD_ISSET(m_pSock->native(), &fdWrite))
			{
				iErrorCode = TIME_OUT;
			}
			else
			{
				szSend = send(m_pSock->native_handle(), pSendBuf, szBufSize, 0);
				iErrorCode = WSAGetLastError();
			}	
		}
		else
		{
			iErrorCode = WSAGetLastError();
		}

		m_pSock->non_blocking(false,ec);	

#else
		return NOT_SUPPORT_PLATFORM;
#endif
	}
	return szSend;
}

int CServerSessionImp::Recv( char *pRecvBuf, size_t szMaxRecvSize, int &iErrorCode, const unsigned short &usWaitsecs /*= 0*/ )
{
	using namespace boost::asio::detail::socket_ops;
	iErrorCode = 0;
	boost::system::error_code ec;
	int szRecv = 0;
	if (0 == usWaitsecs)
	{
		szRecv = m_pSock->read_some(boost::asio::buffer(pRecvBuf, szMaxRecvSize), ec);
		iErrorCode = ec.value();		
	}
	else
	{
		if (!m_pSock->is_open())
		{
			m_pSock->open(boost::asio::ip::tcp::v4());
		}		

#ifdef WIN32
		m_pSock->non_blocking(true, ec);

		szRecv = recv(m_pSock->native_handle(), pRecvBuf, szMaxRecvSize, 0);
		if (0 == szRecv)
		{
			iErrorCode = WSAGetLastError();
		}
		else if (szRecv < 0 && WSAGetLastError() == WSAEWOULDBLOCK)
		{
			fd_set fdRead;
			FD_ZERO(&fdRead);
			FD_SET(m_pSock->native(), &fdRead);
			timeval tv = {usWaitsecs};
			if (select(0, &fdRead, NULL, NULL, &tv) <= 0
				|| !FD_ISSET(m_pSock->native(), &fdRead))
			{
				iErrorCode = TIME_OUT;
			}
			else
			{
				szRecv = recv(m_pSock->native_handle(), pRecvBuf, szMaxRecvSize, 0);
				iErrorCode = WSAGetLastError();	
			}
		}
		m_pSock->non_blocking(false, ec);
#else
		return NOT_SUPPORT_PLATFORM;
#endif

	}
	return szRecv;
}

int CServerSessionImp::SetMaxRecvBufSize( size_t szMaxSize )
{
	boost::asio::socket_base::receive_buffer_size rc(szMaxSize);
	boost::system::error_code ec;
	m_pSock->set_option(rc, ec);
	if (ec)
	{
		return ec.value();
	}	
	return EXE_SUCCESS;
}

int CServerSessionImp::CloseSession()
{
	boost::system::error_code ec;
	ec.clear();
	if (m_pSock->is_open())
	{
		m_pSock->close(ec);
	}
	return ec.value();
}

bool CServerSessionImp::IsOpen()
{
	return m_pSock->is_open();
}
