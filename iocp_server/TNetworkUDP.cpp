#include "TNetworkUDP.h"
bool  TNetworkUDP::CreateServer(int iPort)
{
    m_Sock = socket(AF_INET, SOCK_DGRAM, 0);// IPPROTO_TCP);
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// 전화번호
    sa.sin_port = htons(iPort); // 받는 사람    
    int iRet = bind(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
   
    // 넌블록킹 소켓전환
    u_long on = 1;
    ioctlsocket(m_Sock, FIONBIO, &on);

    m_bRun = true;
    return true;
}
int   TNetworkUDP::Send(THost& host, UPACKET& packet)
{
    char* pMsg = (char*)&packet;
    int iSendSize = sendto(m_Sock, pMsg,
        packet.ph.len, 0,
        (SOCKADDR*)&host.addr, sizeof(host.addr));
    Check(host, iSendSize);
    return iSendSize;
}
bool    TNetworkUDP::Run()
{
    while (m_bRun)
    {
        RecvRun();
        PacketProcess();
        PostProcess();
    }
    return true;
}
bool    TNetworkUDP::RecvRun()
{
    int iAddlen = sizeof(m_Address);
    char* pRecvMsg = (char*)&m_tPacket;
    int iRecvByte = recvfrom(m_Sock, pRecvMsg,
        PACKET_MAX_PACKET_SIZE, 0, 
        (SOCKADDR*)&m_Address, &iAddlen);

    if (iRecvByte == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            //return TResult::TNet_FALSE;
            return false;
        }
        //return TResult::TNet_EWOULDBLOCK;
    }
    else
    {
        if (m_tPacket.ph.type == PACKET_JOIN_REQ)
        {
            AddHost( 0, m_Address);
            SendPacket(m_Address, nullptr, PACKET_JOIN_ACK);
            SendPacket(m_Address, nullptr, PACKET_CHAT_NAME_SC_REQ);
        }

        if (m_tPacket.ph.type == PACKET_CHAT_NAME_CS_ACK)
        {
            USER_NAME* pData = (USER_NAME*)m_tPacket.msg;
            THost* pHost = FindHost(m_Address);
            if (pHost != nullptr)
            {
                memcpy(pHost->m_csName, pData->name, sizeof(char) * 32);
            }
            m_tPacket.ph.type = PACKET_JOIN_USER;
            m_RecvPool.emplace_back(m_tPacket);
        }
        else
        {
            m_RecvPool.emplace_back(m_tPacket);
        }
        if (m_tPacket.ph.type == PACKET_DRUP_USER)
        {
            THost* pHost = FindHost(m_Address);
            if (pHost != nullptr)
            {
                pHost->m_bConnect = false;
            }            
        }
    }
    //return TResult::TNet_TRUE;   
    return true;
}
int     TNetworkUDP::SendPacket(SOCKADDR_IN addr, const char* msg,WORD type)
{   
    return TNetwork::SendPacket(addr, msg, type);
}
bool    TNetworkUDP::PostProcess()
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