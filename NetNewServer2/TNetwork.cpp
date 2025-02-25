#include "TNetwork.h"


// WinSock 초기화 WSAStartup
bool    TNetwork::Init()
{
    // 윈속 초기화( 버전선택)
    WSADATA wsa;
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    return true;
}


// 메인루프 (Accept → RecvRun → PacketProcess → PostProcess)
bool    TNetwork::Run()
{
    while (m_bRun)
    {
        if (m_pModel)   m_pModel->Run();
        //Accept();
        RecvRun();
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

// 모든 클라이언트에게 패킷 전송
void  TNetwork::Broadcasting(UPACKET& packet)
{
    for (auto sendHost = m_HostList.begin(); sendHost != m_HostList.end(); sendHost++)
    {
        THost& host = *sendHost;
        if (host.m_bConnect == false) continue;
        char* pMsg = (char*)&packet;
        Send(host, packet);
    }
}

//수신된 패킷을 처리 후 브로드 캐스팅
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


// 소켓 오류 처리
bool TNetwork::Check(THost& host, int iCode)
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

THost* TNetwork::FindHost(SOCKET sock)
{
    return nullptr;
}
THost* TNetwork::FindHost(SOCKADDR_IN addr)
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

// 접속된 호스트의 IP 출력
bool    TNetwork::AddHost(SOCKET clientSock, SOCKADDR_IN clientaddr)
{
    std::cout << "접속(IP):" << inet_ntoa(clientaddr.sin_addr)
        << "포트번호" << ntohs(clientaddr.sin_port)
        << std::endl;
    THost host;
    host.addr = clientaddr;
    host.sock = clientSock;
    m_HostList.push_back(host);
    return true;
}

// 패킷 전송 함수
int     TNetwork::SendPacket(SOCKET sock, const char* msg, WORD type)
{
    return 0;
}// 패킷 전송 함수
int     TNetwork::SendPacket(SOCKADDR_IN addr, const char* msg, WORD type)
{
    return 0;
}

// 클라이언트 접속 처리 (accept)
SOCKET    TNetwork::Accept()
{
    return 0;
}




// 클라이언트 Accept 오류 처리
bool TNetwork::CheckAccept(int iCode)
{
    return false;
}

TNetwork::TNetwork(std::shared_ptr<TSelectModel> pModel)
{
    m_pModel = pModel;
}
