#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>
#include <string>
#include <thread>   // std::thread
#include <conio.h>
#include <process.h> // _beginthreadex
#include "TProtocol.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
enum TResult{
	TNet_FALSE = 0,
	TNet_EWOULDBLOCK,
	TNet_TRUE,
};
class TNetwork
{
public:
	SOCKET  m_Sock;
	bool    m_bRun;
	std::string m_szRecvData;	
	int     m_iSendBytes = 0;
	int     m_iRecvBytes = 0;
public:
	void    Reset();
	bool    Init();
	bool    Release();
	bool    Run();
	bool	Connect(std::string ip, UINT iPort);
	bool	DisConnect();
	bool	RecvWork();
	bool	SendWork(std::string SendBuf);
	TResult	Check(int iCode);
	int     SendPacket(SOCKET sock,
						const char* msg,
						WORD type);
};

