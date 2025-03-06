#include "TNetwork.h"
DWORD WINAPI ThreadWorkerProc(LPVOID lpParameter)
{
    TNetwork* pNet = (TNetwork*)lpParameter;
    HANDLE hIOCP = pNet->m_hIOCP;
    while (1)
    {
        DWORD dwTransfer;
        ULONG_PTR CompletionKey;
        OVERLAPPED* Overlapped;
        BOOL bRet = ::GetQueuedCompletionStatus(hIOCP,
            &dwTransfer,
            &CompletionKey,
            &Overlapped, 1000);
        if (bRet == TRUE)
        {
            THost* pHost = (THost*)CompletionKey;
            OVERLAPPED2* ov = (OVERLAPPED2*)Overlapped;
            pHost->Dispatch(dwTransfer, ov);
        }
        else
        {
            THost* pHost = (THost*)CompletionKey;
            if (::WSAGetLastError() == WAIT_TIMEOUT)
            {
                std::cout << ".";
            }
            else if ( dwTransfer == 0)
            {                
                pHost->m_bConnect = false;
            }
            else
            {
                if (Overlapped != nullptr)
                {
                    pHost->m_bConnect = false;
                }
                std::cout << GetLastError() << std::endl;                
            }
        }
    }
    return 0;
}
void  TAcceptor::Accept()
{
    SOCKADDR_IN clientaddr;
    int addlen = sizeof(clientaddr);
    SOCKET clientSock = accept(m_pNet->m_Sock, (SOCKADDR*)&clientaddr, &addlen);
    if (clientSock == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            exit(1);
        }
    }
    else
    {
        TSync lock(this);
        THost* pHost = m_pNet->AddHost(clientSock, clientaddr);
    }
}
bool  TAcceptor::RunThread()
{
    while (m_bRun)
    {
        Accept();
    }
    return true;
}
bool    TNetwork::CreateServer(int iPort)
{
    m_Sock = socket(AF_INET, SOCK_STREAM, 0);// IPPROTO_TCP);
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// 전화번호
    sa.sin_port = htons(iPort); // 받는 사람    
    int iRet = bind(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return false;
    iRet = listen(m_Sock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return false;

    // 넌블록킹 소켓전환
    u_long on = 1;
    ioctlsocket(m_Sock, FIONBIO, &on);
    m_bRun = true;
    return true;
}
SOCKET  TNetwork::Accept()
{
    SOCKADDR_IN clientaddr;
    int addlen = sizeof(clientaddr);
    SOCKET clientSock = accept(m_Sock, (SOCKADDR*)&clientaddr,   &addlen);
    if (clientSock == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            exit(1);
        }
    }
    else
    {
        AddHost(clientSock, clientaddr);      
    }
    return clientSock;
}
THost*  TNetwork::AddHost(SOCKET clientSock, SOCKADDR_IN clientaddr)
{
    std::cout << "접속(IP):" << inet_ntoa(clientaddr.sin_addr)
        << "포트번호" << ntohs(clientaddr.sin_port)
        << std::endl;
    THost* pHost = new THost;
    pHost->m_tNet = this;
    pHost->addr = clientaddr;
    pHost->sock = clientSock;
    pHost->m_bConnect = true;
    m_HostList.push_back(pHost);

    ::CreateIoCompletionPort((HANDLE)clientSock, m_hIOCP, (ULONG_PTR)pHost, 0);
    SendPacket(pHost, nullptr, PACKET_JOIN_ACK);
    SendPacket(pHost, nullptr, PACKET_CHAT_NAME_SC_REQ);
    pHost->AsyncRecvTCP();
    return pHost;
}
bool    TNetwork::Init()
{
    // 윈속 초기화( 버전선택)
    WSADATA wsa;
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;

    m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    for (int iThread = 0; iThread < MAX_NUM_WORK_THREAD; iThread++)
    {
        DWORD threadID;
        m_hWorkerThread[iThread] = CreateThread(0, 0, ThreadWorkerProc, this, 0, &threadID);
    }
   
    return true;
}
bool    TNetwork::Run()
{
    std::cout << "서버가 시작되었습니다." << std::endl;
    while (m_bRun)
    {
        Accept();
        PacketProcess();
        PostProcess();
    }
    return true;
}
bool    TNetwork::Release()
{
    closesocket(m_Sock);
    WSACleanup();
    return true;
}
void    TNetwork::Broadcasting(UPACKET& packet)
{
    for (auto sendHost = m_HostList.begin(); sendHost != m_HostList.end(); sendHost++)
    {
        THost* host = *sendHost;
        if (host->m_bConnect == false) continue;
        char* pMsg = (char*)&packet;
        Send(host, packet);
    }
}
void    TNetwork::AddRecvPacket(UPACKET& packet)
{
    {
        TSync lock(this);
        m_RecvPool.emplace_back(packet);
    }
}
bool    TNetwork::PacketProcess()
{    
    {
        TSync lock(this);
        for (auto recvHost = m_RecvPool.begin(); recvHost != m_RecvPool.end(); recvHost++)
        {
            UPACKET& packet = *recvHost;

            Broadcasting(packet);
        }
        m_RecvPool.clear();
    }
    return true;
}
bool    TNetwork::PostProcess()
{    
    // 종료처리
    for (auto iter = m_HostList.begin();
        iter != m_HostList.end(); )
    {
        THost* host = *iter;
        if (host->m_bConnect == false)
        {
            USER_NAME Data;
            memcpy(Data.name, host->m_csName, sizeof(char) * 32);

            std::cout << "접속종료(IP):" <<
                inet_ntoa(host->addr.sin_addr)
                << "포트번호" << ntohs(host->addr.sin_port)
                << std::endl;
            closesocket(host->sock);

            {
                iter = m_HostList.erase(iter);
            }
            delete host;
            UPACKET sendpacket;
            ZeroMemory(&sendpacket, sizeof(sendpacket));
            sendpacket.ph.len = PACKET_HEADER_SIZE + sizeof(USER_NAME);
            sendpacket.ph.type = PACKET_DRUP_USER;
            memcpy(sendpacket.msg, (char*)&Data, sizeof(USER_NAME));
            AddRecvPacket(sendpacket);
        }
        else
        {
            iter++;
        }
    }
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
    for (auto host : m_HostList)
    {
        if (host->addr.sin_addr.S_un.S_addr == addr.sin_addr.S_un.S_addr)
        {
            return host;
        }
    }
    return nullptr;
}
int     TNetwork::Send(THost* host, UPACKET& packet)
{
    char* pMsg = (char*)&packet;
    int iSendSize = send(host->sock, pMsg, packet.ph.len, 0);
    return iSendSize;
}
int   TNetwork::SendPacket(SOCKET sock,  const char* msg,   WORD type)
{   
    return 0;
}
int   TNetwork::SendPacket(SOCKADDR_IN addr, const char* msg, WORD type)
{
    return 0;
};
int     TNetwork::SendPacket(THost* host, const char* msg, WORD type)
{
    UINT iMsgSize = 0;
    if (msg != nullptr)
    {
        iMsgSize = strlen(msg);
    }
    UPACKET sendpacket;
    ZeroMemory(&sendpacket, sizeof(sendpacket));
    sendpacket.ph.len = PACKET_HEADER_SIZE + iMsgSize;
    sendpacket.ph.type = type;
    if (iMsgSize > 0)
    {
        memcpy(sendpacket.msg, msg, iMsgSize);
    }
    char* pMsg = (char*)&sendpacket;

    OVERLAPPED2* ov = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);

    host->m_wsaSendBuffers.buf = pMsg;
    host->m_wsaSendBuffers.len = PACKET_MAX_PACKET_SIZE;
    DWORD dwSendByte;
    int iSendbyte = WSASend(host->sock,
        &host->m_wsaSendBuffers,
        1,
        &dwSendByte,
        0,
        &ov->m_ov,
        nullptr);
    if (iSendbyte == SOCKET_ERROR)
    {
        if (GetLastError() != ERROR_IO_PENDING)
        {
            host->m_bConnect = false;
            return false;
        }
    }
    return true;
}
TNetwork::TNetwork()
{

}