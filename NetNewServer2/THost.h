#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>
#include <list>
#include "TProtocol.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
enum TResult {
    TNet_FALSE = 0,
    TNet_EWOULDBLOCK,
    TNet_TRUE,
};
class TNetwork;
class THost
{
public:
    SOCKET      sock;
    SOCKADDR_IN addr;
    char        m_csName[32];    
    int         m_iRecvBytes = 0;
    bool        m_bConnect;
    UPACKET     m_tPacket;
    THost();
    bool        Run(TNetwork& net);
    TResult     Check(int iCode);
};

