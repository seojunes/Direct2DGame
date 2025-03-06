#include "TNetwork.h"
bool    TNetwork::Init()
{
    // 윈속 초기화( 버전선택)
    WSADATA wsa;
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    return true;
}
bool    TNetwork::Run()
{
    while (m_bRun)
    {
        if( m_pModel ) m_pModel->Run();
        else
        {
            Accept();
            RecvRun();
        }
        PacketProcess();
        PostProcess();
    }
    return true;
}
bool    TNetwork::Release()
{
    closesocket(m_Sock);
    // 윈속 소멸
    WSACleanup();
    return true;
}
void    TNetwork::Broadcasting(UPACKET& packet)
{
    for (auto sendHost = m_HostList.begin(); sendHost != m_HostList.end(); sendHost++)
    {
        THost& host = *sendHost;
        if (host.m_bConnect == false) continue;
        char* pMsg = (char*)&packet;
        Send(host, packet);
    }
}

bool    TNetwork::PacketProcess()
{
    for (auto recvHost = m_RecvPool.begin();
        recvHost != m_RecvPool.end();
        recvHost++)
    {
        UPACKET& packet = *recvHost;
        Broadcasting(packet);
    }
    m_RecvPool.clear();
    return true;
}

bool    TNetwork::Check(THost& host, int iCode)
{
    if (iCode == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            host.m_bConnect = false;
        }
    }
    else
    {
        return true;
    }
    return false;
}
THost*  TNetwork::FindHost(SOCKET sock)
{
    return nullptr;
}
THost*  TNetwork::FindHost(SOCKADDR_IN addr)
{
    for (auto& host : m_HostList)
    {
        if (host.addr.sin_addr.S_un.S_addr ==
            addr.sin_addr.S_un.S_addr)
        {
            return &host;
        }
    }
    return nullptr;
}
bool    TNetwork::AddHost(SOCKET clientSock, SOCKADDR_IN clientaddr)
{
    std::cout << "접속(IP):" << inet_ntoa(clientaddr.sin_addr)
        << "포트번호" << ntohs(clientaddr.sin_port)
        << std::endl;
    THost host;
    host.addr = clientaddr;
    host.sock = clientSock;
    host.m_bConnect = true;
    m_HostList.push_back(host);
    return true;
}


int   TNetwork::SendPacket(SOCKET sock,  const char* msg,   WORD type)
{   
    return 0;
}
int   TNetwork::SendPacket(SOCKADDR_IN addr, const char* msg, WORD type)
{
    return 0;
};
int   TNetwork::SendPacket(THost* host, const char* msg, WORD type)
{
    return 0;
}

SOCKET TNetwork::Accept()
{
    return 0;
}
bool TNetwork::CheckAccept(int iCode)
{    
    return false;
}

TNetwork::TNetwork(std::shared_ptr<TNetModel> pModel)
{
    m_pModel = pModel;   
}