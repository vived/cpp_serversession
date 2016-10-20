#pragma once

/*
	名称:ServerSession.h
	库介绍：此库由客户端使用，一个连接对应一个ServerSession，当ServerSession有效后
			直接进行接收或者发送数据
	版本：1.0
	作者:zhangfang 20140220
*/

#ifdef SERVERSESSION_CPP
	#define DllImportOrExport __declspec(dllexport)
#else
	#define DllImportOrExport __declspec(dllimport)
#endif

const int EXE_SUCCESS = 0;

const int TIME_OUT = -255;
const int NOT_SUPPORT_PLATFORM = -256;
const int EXE_FAILED	=	-257;

class CServerSessionImp;


class DllImportOrExport CServerSession
{		
public:
	CServerSession(const char *strServerIp, const int &iServerPort);
	~CServerSession();
public:
	/*使用本地固定IP和端口*/
	int BindLocalIp(const char *strLocalIp, const int &iLocalPort);
	/*手动连接 ，0秒 为阻塞连接*/
	int Connect(const unsigned short &usWaitsecs = 0);
	/*发送数据， 0秒为阻塞发送，返回已发送字节数，iErrorCode指示是否出错*/
	int Send(const char* pSendBuf, size_t szBufSize,  int &iErrorCode, const unsigned short &usWaitsecs = 0);
	/*接收数据，0秒为阻塞接收，返回已接收字节数，iErrorCode指示是否出错*/
	int Recv(char *pRecvBuf, size_t szMaxRecvSize, int &iErrorCode, const unsigned short &usWaitsecs = 0);
	/*设置最大缓冲区*/
	int SetMaxRecvBufSize(size_t szMaxSize);
	int CloseSession();
	bool IsOpen(); //判断Session是否可用
protected:
private:
	CServerSessionImp  *m_pSessionImp;	
};