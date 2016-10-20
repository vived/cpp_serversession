// ServerSession.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ServerSession.h"
#include "ServerSessionImp.h"

CServerSession::CServerSession( const char *strServerIp, const int &iServerPort )
{
	m_pSessionImp = new CServerSessionImp(strServerIp, iServerPort);
}

int CServerSession::Connect( const unsigned short &usWaitsecs /*= 0*/ )
{
	return (NULL == m_pSessionImp) ? EXE_FAILED : m_pSessionImp->Connect(usWaitsecs);
}

int CServerSession::Send( const char* pSendBuf, size_t szBufSize, int &iErrorCode, const unsigned short &usWaitsecs /*= 0*/ )
{
	return (NULL == m_pSessionImp) ? EXE_FAILED : m_pSessionImp->Send(pSendBuf, szBufSize, iErrorCode, usWaitsecs);
}

int CServerSession::Recv( char *pRecvBuf, size_t szMaxRecvSize, int &iErrorCode, const unsigned short &usWaitsecs /*= 0*/ )
{
	return (NULL == m_pSessionImp) ? EXE_FAILED : m_pSessionImp->Recv(pRecvBuf, szMaxRecvSize, iErrorCode, usWaitsecs);
}

CServerSession::~CServerSession()
{
	m_pSessionImp->CloseSession();
	delete m_pSessionImp;
	m_pSessionImp = NULL;
}

int CServerSession::SetMaxRecvBufSize( size_t szMaxSize )
{
	return (NULL == m_pSessionImp) ? EXE_FAILED : m_pSessionImp->SetMaxRecvBufSize(szMaxSize);
}

int CServerSession::CloseSession()
{
	return (NULL == m_pSessionImp) ? EXE_FAILED : m_pSessionImp->CloseSession();
}

int CServerSession::BindLocalIp( const char *strLocalIp, const int &iLocalPort )
{
	return (NULL == m_pSessionImp) ? EXE_FAILED : m_pSessionImp->BindLocalIp(strLocalIp, iLocalPort);
}

bool CServerSession::IsOpen()
{
	return (NULL == m_pSessionImp) ? false : m_pSessionImp->IsOpen();
}
