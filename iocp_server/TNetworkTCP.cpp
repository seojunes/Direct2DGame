#include "TNetworkTCP.h"
bool TNetworkTCP::CheckAccept(int iCode)
{
    if (iCode == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            exit(1);
        }
    }
    else
    {
        return true;
    }
    return false;
}
SOCKET    TNetworkTCP::Accept()
{
    SOCKADDR_IN clientaddr;
    int addlen = sizeof(clientaddr);
    SOCKET clientSock = accept(m_Sock, (SOCKADDR*)&clientaddr,
        &addlen);
    if (CheckAccept(clientSock))
    {
        AddHost(clientSock, clientaddr);
        SendPacket(clientSock, nullptr, PACKET_JOIN_ACK);
        SendPacket(clientSock, nullptr, PACKET_CHAT_NAME_SC_REQ);
    }
    return clientSock;
}
bool    TNetworkTCP::CreateServer(int iPort)
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

    if (m_pModel != nullptr)
    {
        m_pModel->Init(this);
    }
    return true;
}
bool    TNetworkTCP::Run()
{
    while (m_bRun)
    {
        if (m_pModel) m_pModel->Run();
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
bool    TNetworkTCP::RecvRun()
{
    for (auto iter = m_HostList.begin();
        iter != m_HostList.end();
        iter++)
    {
        THost& host = *iter;
        if (host.m_bConnect == false) continue;
        host.RunTCP(*this);
    }
    return true;
}
bool    TNetworkTCP::PostProcess()
{
    // 종료처리
    for (auto iter = m_HostList.begin();
        iter != m_HostList.end(); )
    {
        THost& host = *iter;
        if (host.m_bConnect == false)
        {
            USER_NAME Data;
            memcpy(Data.name, host.m_csName, sizeof(char) * 32);

            std::cout << "접속종료(IP):" <<
                inet_ntoa(host.addr.sin_addr)
                << "포트번호" << ntohs(host.addr.sin_port)
                << std::endl;
            closesocket(host.sock);

            iter = m_HostList.erase(iter);

            UPACKET sendpacket;
            ZeroMemory(&sendpacket, sizeof(sendpacket));
            sendpacket.ph.len = PACKET_HEADER_SIZE + sizeof(USER_NAME);
            sendpacket.ph.type = PACKET_DRUP_USER;
            memcpy(sendpacket.msg, (char*)&Data, sizeof(USER_NAME));
            m_RecvPool.emplace_back(sendpacket);
        }
        else
        {
            iter++;
        }
    }
    return true;
}
int     TNetworkTCP::Send(THost& host, UPACKET& packet)
{
    char* pMsg = (char*)&packet;
    int iSendSize = send(host.sock, pMsg, packet.ph.len, 0);
    Check(host, iSendSize);
    return iSendSize;
}
int  TNetworkTCP::SendPacket(SOCKADDR_IN addr,
    const char* msg,
    WORD type)
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

    int iSendbyte = sendto(m_Sock, pMsg, sendpacket.ph.len, 0,
        (SOCKADDR*)&addr, sizeof(addr));
    /*if (Check(iSendbyte) == TResult::TNet_FALSE)
    {
        return false;
    }*/
    return true;
}
int     TNetworkTCP::SendPacket(SOCKET sock,
    const char* msg,
    WORD type)
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

    int iSendbyte = send(sock, pMsg, sendpacket.ph.len, 0);
    /*if (Check(iSendbyte) == TResult::TNet_FALSE)
    {
        return false;
    }*/
    return true;
}
int     TNetworkTCP::SendPacket(THost* host,  const char* msg, WORD type)
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