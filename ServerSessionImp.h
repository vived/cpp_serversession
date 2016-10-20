#pragma once

#include "boost/asio.hpp"
#include "boost/asio/deadline_timer.hpp"

class CServerSessionImp
{
public:
	CServerSessionImp(std::string strServerIp, const int &iServerPort);
	virtual ~CServerSessionImp(void);
public:
	int BindLocalIp(const std::string &strLocalIp, const int &iLocalPort);
	int Connect(const unsigned short &usWaitsecs = 0);
	int Send(const char* pSendBuf, size_t szBufSize,  int &iErrorCode, const unsigned short &usWaitsecs = 0);
	int Recv(char *pRecvBuf, size_t szMaxRecvSize, int &iErrorCode, const unsigned short &usWaitsecs = 0);
	int SetMaxRecvBufSize(size_t szMaxSize);
	int CloseSession();
	bool IsOpen();
private:
	std::string m_strServerIp;
	std::string m_strLoacalIp;
	int m_iServerPort;
	int m_iLocalPort;
	boost::asio::io_service *m_pIoService;
	boost::asio::ip::tcp::socket *m_pSock;
};
