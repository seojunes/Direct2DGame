#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>
#include <list>
#include <vector>
#include "TStreamPacket.h"
#include "TObjectPool.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
enum TResult {
    TNet_FALSE = 0,
    TNet_EWOULDBLOCK,
    TNet_TRUE,
};
class TNetwork;

struct OVERLAPPED2 : TObjectPool<OVERLAPPED2>
{
    enum { MODE_RECV = 0, MODE_SEND = 1, };
    OVERLAPPED m_ov;
    UINT  m_iFlag;
    OVERLAPPED2()
    {
        m_iFlag = MODE_RECV;
        ZeroMemory(&m_ov, sizeof(m_ov));
    }
    OVERLAPPED2(UINT flag)
    {
        m_iFlag = flag;
        ZeroMemory(&m_ov, sizeof(m_ov));
    }
};
class THost
{
public:    
    SOCKET      sock;
    SOCKADDR_IN addr;
    char        m_csName[32];    
    int         m_iRecvBytes = 0;
    bool        m_bConnect = false;
    UPACKET     m_tPacket; 
    // iocp
    WSABUF      m_wsaRecvBuffers;
    WSABUF      m_wsaSendBuffers;
    TNetwork*   m_tNet =nullptr;
    TStreamPacket  m_StreamPacket;
public:
    THost();
    bool        RunTCP(TNetwork& net);
    bool        RunUDP(TNetwork& net);
    TResult     Check(int iCode);
    bool        AsyncRecvTCP(TNetwork& tNet);
    void        Dispatch(DWORD dwTransfer, OVERLAPPED2* ov);
};

