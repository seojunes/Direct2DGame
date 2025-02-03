#include "TNetwork.h"
//쓰레드의 시작함수 : 반환되면 쓰레드는 정상종료된다.
DWORD WorkSend(LPVOID lpThreadParameter)
{
    TNetwork* net = (TNetwork*)lpThreadParameter;
    std::string SendBuf;
    SendBuf.reserve(256);
    while (net->m_bRun)
    {
        std::getline(std::cin, SendBuf);
        //net->SendWork(SendBuf);
        net->SendPacket(net->m_Sock, // 목적지
            SendBuf.c_str(),
            PACKET_CHAT_MSG);
    }
    return 1;
}

int     TNetwork::SendPacket(SOCKET sock,
    const char* msg,
    WORD type)
{
    UINT iMsgSize = strlen(msg);
    UPACKET sendpacket;
    ZeroMemory(&sendpacket, sizeof(sendpacket));
    sendpacket.ph.len = PACKET_HEADER_SIZE + iMsgSize;
    sendpacket.ph.type = type;
    if (iMsgSize > 0)
    {
        memcpy(sendpacket.msg, msg, iMsgSize);
    }
    char* pMsg = (char*)&sendpacket;

    m_iSendBytes = send(m_Sock, pMsg, sendpacket.ph.len, 0);
    if (Check(m_iSendBytes) == TResult::TNet_FALSE)
    {
        return false;
    }
    return true;
}
void    TNetwork::Reset()
{
    m_iRecvBytes = 0;
}
bool    TNetwork::Init()
{
    // 윈속 초기화( 버전선택)
    WSADATA wsa;
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;

    m_bRun = false;
    m_szRecvData.resize(1024);
    return true;
}
bool    TNetwork::Release()
{
    // 윈속 소멸
    WSACleanup();
    return true;
}
bool	TNetwork::Connect(std::string ip, UINT iPort)
{
    //  TCP & IP 프로토콜
    m_Sock = socket(AF_INET, SOCK_STREAM, 0);// IPPROTO_TCP);

    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(10000); // 받는 사람
    //sa.sin_addr.s_addr = inet_addr("192.168.0.88");// 전화번호
    inet_pton(AF_INET, ip.c_str(), &sa.sin_addr);
    int iRet = connect(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return false;

    // 넌블록킹 소켓전환
    u_long on = 1;
    ioctlsocket(m_Sock, FIONBIO, &on);

    m_bRun = true;
    return true;
}
bool	TNetwork::DisConnect()
{
    closesocket(m_Sock);
    return true;
}
bool    TNetwork::Run()
{
    HANDLE hThread1 = CreateThread(NULL, 0,
        WorkSend, this, CREATE_SUSPENDED, NULL);

    UPACKET recvPacket;
    ZeroMemory(&recvPacket, sizeof(recvPacket));
    char* pRecvMsg = (char*)&recvPacket;
    m_iRecvBytes = 0;
    // [2],2
    while (m_bRun)
    {
        int iRecvByte = recv(m_Sock, &pRecvMsg[m_iRecvBytes],
            PACKET_HEADER_SIZE - m_iRecvBytes, 0);
        TResult ret = Check(iRecvByte);
        if (ret == TResult::TNet_FALSE)
        {
            exit(1);
        }
        if (ret == TResult::TNet_TRUE)
        {
            m_iRecvBytes += iRecvByte;
            if (m_iRecvBytes > PACKET_HEADER_SIZE)
            {
                continue;
            }
        }

        if (m_iRecvBytes == PACKET_HEADER_SIZE)
        {
            while (recvPacket.ph.len > m_iRecvBytes)//m_iRecvBytes= 4
            {
                int iRecvByte = recv(m_Sock,
                    &pRecvMsg[m_iRecvBytes],
                    recvPacket.ph.len - m_iRecvBytes, 0);
                if (Check(iRecvByte) == TResult::TNet_TRUE)
                {
                    m_iRecvBytes += iRecvByte;
                }
            }
            // 패킷 완성
            if (recvPacket.ph.type == PACKET_JOIN_USER)
            {
                USER_NAME* pData = (USER_NAME*)recvPacket.msg;
                std::cout << pData->name << "님이 입장하셨습니다." << std::endl;
            }
            if (recvPacket.ph.type == PACKET_CHAT_MSG)
            {
                recvPacket.msg[recvPacket.ph.len - PACKET_HEADER_SIZE] = 0;
                std::cout << recvPacket.msg << std::endl;

            }
            if (recvPacket.ph.type == PACKET_GAME_START)
            {

            }
            if (recvPacket.ph.type == PACKET_GAME_END)
            {

            }
            if (recvPacket.ph.type == PACKET_DRUP_USER)
            {
                USER_NAME* pData = (USER_NAME*)recvPacket.msg;
                std::cout << pData->name << "님이 퇴장하셨습니다." << std::endl;
            }
            if (recvPacket.ph.type == PACKET_CHAT_NAME_SC_REQ)
            {
                std::string SendBuf;
                SendBuf.reserve(256);
                std::getline(std::cin, SendBuf);

                SendPacket(m_Sock, // 목적지
                    SendBuf.c_str(),
                    PACKET_CHAT_NAME_CS_ACK);
                m_bThreadRun = true;

                ResumeThread(hThread1);
            }
            m_iRecvBytes = 0;
            pRecvMsg = (char*)&recvPacket;
        }
        //if (!RecvWork()) break;
    }

    CloseHandle(hThread1);
    return true;
}
bool TNetwork::SendWork(std::string SendBuf)
{
    if (SendBuf.empty())
    {
        return true;
    }
    m_iSendBytes = send(m_Sock, &SendBuf[0], SendBuf.size(), 0);
    if (Check(m_iSendBytes) == TResult::TNet_FALSE)
    {
        return false;
    }
    return true;
}
bool TNetwork::RecvWork()
{
    m_iRecvBytes = recv(m_Sock, &m_szRecvData[0], m_szRecvData.size(), 0);
    if (m_iRecvBytes == 0)
    {
        return false;
    }
    if (Check(m_iRecvBytes) == TResult::TNet_TRUE)
    {
        std::string msg;
        msg = m_szRecvData.substr(0, m_iRecvBytes);
        std::cout << msg.c_str() << std::endl;
    }
    return true;
}

TResult TNetwork::Check(int iCode)
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