#pragma once
#include "TSelectModel.h"


class TNetwork
{
public:
    std::shared_ptr<TSelectModel>  m_pModel = nullptr;
protected:
    bool    m_bRun = false;
public:
    SOCKET  m_Sock;
    std::list<THost> m_HostList;
    std::list<UPACKET>  m_RecvPool;
    std::list<UPACKET>  m_SendPool;
public:
    bool   Init();
    bool   Release();
public:
    virtual bool  CreateServer(int iPort) = 0;
    virtual bool  RecvRun() = 0;
    virtual bool  PostProcess() = 0;
    virtual int   Send(THost&, UPACKET&) = 0;
public:
    virtual bool  Run();
    virtual void  Broadcasting(UPACKET& packet);
    virtual bool  PacketProcess();
    virtual int   SendPacket(SOCKET sock, const char* msg, WORD type);
    virtual int   SendPacket(SOCKADDR_IN addr, const char* msg, WORD type);
    virtual bool  Check(THost& host, int iCode);
public:
    virtual THost* FindHost(SOCKET sock);
    virtual THost* FindHost(SOCKADDR_IN addr);
    virtual bool   AddHost(SOCKET clientSock, SOCKADDR_IN clientaddr);
public:
    /// TCP Àü¿ë
    virtual SOCKET Accept();
    virtual bool CheckAccept(int iCode);
public:
    TNetwork(std::shared_ptr<TSelectModel> pModel);
};


