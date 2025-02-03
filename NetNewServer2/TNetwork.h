#pragma once
#include "THost.h"

class TNetwork
{
    SOCKET  m_Sock;
    bool    m_bRun = false;
    std::list<THost> m_HostList;
public:
    std::list<UPACKET>  m_RecvPool;
    std::list<UPACKET>  m_SendPool;
public:
    bool   Init();
    bool   Release();
public:   
    void  Broadcasting(UPACKET& packet);
    int   SendPacket(SOCKET sock,const char* msg,WORD type);
    bool  CreateServer(int iPort);
    bool  Run();
    bool  AddHost(SOCKET clientSock, SOCKADDR_IN clientaddr);
    bool  Accept();
    bool  RecvRun();
    bool  PacketProcess();
    bool  PostProcess();
public:
    bool Check(THost& host, int iCode);
    bool CheckAccept(int iCode);
};

class TNetworkClient : public TNetwork {};
class TNetworkServer : public TNetwork {};

