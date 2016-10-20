#pragma once

/*
	����:ServerSession.h
	����ܣ��˿��ɿͻ���ʹ�ã�һ�����Ӷ�Ӧһ��ServerSession����ServerSession��Ч��
			ֱ�ӽ��н��ջ��߷�������
	�汾��1.0
	����:zhangfang 20140220
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
	/*ʹ�ñ��ع̶�IP�Ͷ˿�*/
	int BindLocalIp(const char *strLocalIp, const int &iLocalPort);
	/*�ֶ����� ��0�� Ϊ��������*/
	int Connect(const unsigned short &usWaitsecs = 0);
	/*�������ݣ� 0��Ϊ�������ͣ������ѷ����ֽ�����iErrorCodeָʾ�Ƿ����*/
	int Send(const char* pSendBuf, size_t szBufSize,  int &iErrorCode, const unsigned short &usWaitsecs = 0);
	/*�������ݣ�0��Ϊ�������գ������ѽ����ֽ�����iErrorCodeָʾ�Ƿ����*/
	int Recv(char *pRecvBuf, size_t szMaxRecvSize, int &iErrorCode, const unsigned short &usWaitsecs = 0);
	/*������󻺳���*/
	int SetMaxRecvBufSize(size_t szMaxSize);
	int CloseSession();
	bool IsOpen(); //�ж�Session�Ƿ����
protected:
private:
	CServerSessionImp  *m_pSessionImp;	
};