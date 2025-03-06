#pragma once
#include "THost.h"

#define MAX_NUM_WORK_THREAD 2

enum class TNet_IO
{
    T_UseThread,
    T_StandardSelect,
    T_AsyncSelect,
    T_EventSelect,    
};

class TAcceptor : public TThread
{
    TNetwork* m_pNet;
public:
    virtual bool  RunThread();
    virtual void  Accept();
    TAcceptor(TNetwork* pNet):m_pNet(pNet)
    {
        CreateThread();
    }
};
class TNetwork : public TLockObj
{    
    std::list<THost*>   m_HostList;
    std::list<UPACKET>  m_RecvPool;    
    std::list<UPACKET>  m_SendPool;
public:
    SOCKET  m_Sock;
    bool    m_bRun = false;
    HANDLE m_hWorkerThread[MAX_NUM_WORK_THREAD];
    HANDLE m_hIOCP;
public:
    bool   Init();
    bool   Release();
    void   AddRecvPacket(UPACKET& packet);
    virtual THost* AddHost(SOCKET clientSock, SOCKADDR_IN clientaddr);
public:
    virtual bool  CreateServer(int iPort);    
    virtual bool  PostProcess();
    virtual bool  Run();
    virtual void  Broadcasting(UPACKET& packet);   
    virtual bool  PacketProcess();
    virtual int   SendPacket(SOCKET sock, const char* msg, WORD type);
    virtual int   SendPacket(SOCKADDR_IN addr, const char* msg, WORD type);
    virtual int   SendPacket(THost* host, const char* msg, WORD type);
    virtual bool  Check(THost& host, int iCode);
    virtual int   Send(THost*, UPACKET&);
public:
    virtual THost* FindHost(SOCKET sock);
    virtual THost* FindHost(SOCKADDR_IN addr);
    virtual SOCKET Accept();
private:

public:
    TNetwork();
};




