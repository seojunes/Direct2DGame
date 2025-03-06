#pragma once
#include "TThread.h"

#define MAX_RECV_SIZE 2048

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
class THost : public TObjectPool<THost>
{
public:  
    TNetwork*   m_tNet = nullptr;  
    SOCKET      sock;
    SOCKADDR_IN addr;
    char        m_csName[32];    
    int         m_iRecvBytes = 0;
    bool        m_bConnect = false;
    UPACKET     m_tPacket; 
    WSABUF      m_wsaRecvBuffers;
    WSABUF      m_wsaSendBuffers;    
private:
    UPACKET*    m_pPacket =nullptr;
    char		m_RecvBuffer[MAX_RECV_SIZE];
    int			m_iStartPos;
    int			m_iWritePos;
    int			m_iReadPos;
public:
    void        Put(char* redvBuffer, int iSize);
    bool        AsyncRecvTCP();
    void        Dispatch(DWORD dwTransfer, OVERLAPPED2* ov);
    virtual void        PacketProcess(UPACKET& packet);
public:
    THost();
    virtual ~THost() {}
};

