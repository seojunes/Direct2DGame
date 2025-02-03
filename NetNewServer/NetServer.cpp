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
TResult CheckSock(int iCode)
{
    if (iCode == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            return TResult::TNet_FALSE;
        }
        return TResult::TNet_EWOULDBLOCK;
    }
    return TResult::TNet_TRUE;
}

struct THost
{
    SOCKET      sock;
    SOCKADDR_IN addr;
    char        m_csName[32];
    char        Recvbuffer[PACKET_MAX_PACKET_SIZE] = { 0, };
    int         m_iRecvBytes = 0;
    bool        bConnect;
    UPACKET     m_tPacket;
    bool        m_bRecvPacket = false;
    THost()
    {
        ZeroMemory(&m_tPacket, sizeof(m_tPacket));
    }
    bool        Run()
    {
        char* pRecvMsg = (char*)&m_tPacket;
        int iRecvByte = recv(sock, &pRecvMsg[m_iRecvBytes],
            PACKET_HEADER_SIZE - m_iRecvBytes, 0);
        TResult ret = CheckSock(iRecvByte);
        if (ret == TResult::TNet_FALSE)
        {
            return false;
        }
        if (ret == TResult::TNet_TRUE)
        {
            m_iRecvBytes += iRecvByte;
            if (m_iRecvBytes > PACKET_HEADER_SIZE)
            {
                return true;
            }
        }

        if (m_iRecvBytes == PACKET_HEADER_SIZE)
        {
            while (m_tPacket.ph.len > m_iRecvBytes)//m_iRecvbyte= 4
            {
                int iRecvByte = recv(sock,
                    &pRecvMsg[m_iRecvBytes],
                    m_tPacket.ph.len - m_iRecvBytes, 0);
                if (CheckSock(iRecvByte) == TResult::TNet_TRUE)
                {
                    m_iRecvBytes += iRecvByte;
                }
            }
            // 패킷 완성
            if (m_tPacket.ph.type == PACKET_CHAT_MSG)
            {
                m_bRecvPacket = true;
                std::cout << m_tPacket.msg << std::endl;
                //m_iRecvBytes = 0;                
                //ZeroMemory(&m_tPacket, sizeof(m_tPacket));
            }
            if (m_tPacket.ph.type == PACKET_GAME_START)
            {
                m_bRecvPacket = true;
                //m_iRecvBytes = 0;
                //ZeroMemory(&m_tPacket, sizeof(m_tPacket));
            }
            if (m_tPacket.ph.type == PACKET_GAME_END)
            {
                m_bRecvPacket = true;
                //m_iRecvBytes = 0;
                //ZeroMemory(&m_tPacket, sizeof(m_tPacket));
            }
            if (m_tPacket.ph.type == PACKET_CHAT_NAME_CS_ACK)
            {
                m_bRecvPacket = true;
            }
        }
        return true;
    }
};


bool Check(THost& host, int iCode)
{
    if (iCode == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            host.bConnect = false;
        }
    }
    else
    {
        return true;
    }
    return false;
}
bool Check(int iCode)
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
int  SendPacket(SOCKET sock, const char* msg, WORD type)
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

std::list<THost> userList;

void  Broadcasting(UPACKET& packet)
{
    for (auto sendHost = userList.begin(); sendHost != userList.end(); sendHost++)
    {
        THost& host = *sendHost;
        if (host.bConnect == false) continue;
        char* pMsg = (char*)&packet;
        int iSendSize = send(host.sock, pMsg,
            packet.ph.len, 0);
        Check(host, iSendSize);
    }
}
int main()
{
    WSADATA wsa;
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);// IPPROTO_TCP);
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// 전화번호
    sa.sin_port = htons(10000); // 받는 사람    
    iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return 1;
    iRet = listen(sock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return 1;

    // 넌블록킹 소켓전환
    u_long on = 1;
    ioctlsocket(sock, FIONBIO, &on);

    SOCKADDR_IN clientaddr;
    int addlen = sizeof(clientaddr);


    while (1)
    {
        SOCKET clientSock = accept(sock, (SOCKADDR*)&clientaddr,
            &addlen);
        if (Check(clientSock))
        {
            // 정상작업
            std::cout << "접속(IP):" << inet_ntoa(clientaddr.sin_addr)
                << "포트번호" << ntohs(clientaddr.sin_port)
                << std::endl;
            THost host;
            host.addr = clientaddr;
            host.sock = clientSock;
            userList.push_back(host);

            SendPacket(host.sock, nullptr, PACKET_CHAT_NAME_SC_REQ);
        }

        for (auto iter = userList.begin();
            iter != userList.end();
            iter++)
        {
            THost& host = *iter;
            if (host.bConnect == false) continue;
            host.Run();
        }

        for (auto recvHost = userList.begin();
            recvHost != userList.end();
            recvHost++)
        {
            THost& sendUser = *recvHost;
            if (sendUser.bConnect == false) continue;
            if (sendUser.m_bRecvPacket == false)
            {
                continue;
            }
            //서버작업
            if (sendUser.m_tPacket.ph.type == PACKET_CHAT_NAME_CS_ACK)
            {
                memcpy(sendUser.m_csName, sendUser.m_tPacket.msg,
                    sendUser.m_tPacket.ph.len - PACKET_HEADER_SIZE);

                /*UPACKET sendpacket;
                JOIN_USER join;
                join.id = 99;
                memcpy(join.name, sendUser.m_csName, sizeof(char)*32);
                ZeroMemory(&sendpacket, sizeof(sendpacket));
                sendpacket.ph.len = PACKET_HEADER_SIZE + sizeof(join);
                sendpacket.ph.type = PACKET_JOIN_USER;
                memcpy(sendpacket.msg, (char*)&join, sizeof(join));
                Broadcasting(sendpacket);*/
                sendUser.m_bRecvPacket = false;
                sendUser.m_iRecvBytes = 0;
                ZeroMemory(&sendUser.m_tPacket, sizeof(sendUser.m_tPacket));
                continue;
            }
            Broadcasting(sendUser.m_tPacket);

            sendUser.m_bRecvPacket = false;
            sendUser.m_iRecvBytes = 0;
            ZeroMemory(&sendUser.m_tPacket, sizeof(sendUser.m_tPacket));
        }

        // 종료처리
        for (auto iter = userList.begin();
            iter != userList.end(); )
        {
            THost& host = *iter;
            if (host.bConnect == false)
            {
                std::cout << "접속종료(IP):" <<
                    inet_ntoa(host.addr.sin_addr)
                    << "포트번호" << ntohs(host.addr.sin_port)
                    << std::endl;
                closesocket(host.sock);
                iter = userList.erase(iter);
            }
            else
            {

                ZeroMemory(host.Recvbuffer, sizeof(char) * 256);
                iter++;
            }
        }
    }

    Sleep(10000);

    closesocket(sock);
    WSACleanup();

}
