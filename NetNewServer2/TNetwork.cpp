#include "TNetwork.h"


// WinSock �ʱ�ȭ WSAStartup
bool    TNetwork::Init()
{
    // ���� �ʱ�ȭ( ��������)
    WSADATA wsa;
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    return true;
}


// ���η��� (Accept �� RecvRun �� PacketProcess �� PostProcess)
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
    // ���� �Ҹ�
    WSACleanup();
    return true;
}

// ��� Ŭ���̾�Ʈ���� ��Ŷ ����
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

//���ŵ� ��Ŷ�� ó�� �� ��ε� ĳ����
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


// ���� ���� ó��
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

// ���ӵ� ȣ��Ʈ�� IP ���
bool    TNetwork::AddHost(SOCKET clientSock, SOCKADDR_IN clientaddr)
{
    std::cout << "����(IP):" << inet_ntoa(clientaddr.sin_addr)
        << "��Ʈ��ȣ" << ntohs(clientaddr.sin_port)
        << std::endl;
    THost host;
    host.addr = clientaddr;
    host.sock = clientSock;
    m_HostList.push_back(host);
    return true;
}

// ��Ŷ ���� �Լ�
int     TNetwork::SendPacket(SOCKET sock, const char* msg, WORD type)
{
    return 0;
}// ��Ŷ ���� �Լ�
int     TNetwork::SendPacket(SOCKADDR_IN addr, const char* msg, WORD type)
{
    return 0;
}

// Ŭ���̾�Ʈ ���� ó�� (accept)
SOCKET    TNetwork::Accept()
{
    return 0;
}




// Ŭ���̾�Ʈ Accept ���� ó��
bool TNetwork::CheckAccept(int iCode)
{
    return false;
}

TNetwork::TNetwork(std::shared_ptr<TSelectModel> pModel)
{
    m_pModel = pModel;
}
