#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>
#include <list>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")


// 클라이언트 정보 저장
struct THost
{
    SOCKET      sock;                           // 클라이언트 소켓 핸들
    SOCKADDR_IN addr;                           // 클라이언트 주소 및 포트 정보
    char        Recvbuffer[256] = { 0, };       // 수신 데이터 저장 공간
    int         iRecvBytes;                     // 수신된 바이트 수 저장.
    bool        bConnect;                       // 연결여부 (종료 시 false 설정)
};
//에러처리
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
int main()
{
    WSADATA wsa;
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);                // 윈속 초기화
    if (iRet != 0) return 1;
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);// IPPROTO_TCP); TCP소켓 생성
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);// 전화번호
    sa.sin_port = htons(10000); // 받는 사람    
    iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));              // 서버 소켓을 포트 10000에 바인딩
    if (iRet == SOCKET_ERROR) return 1;
    iRet = listen(sock, SOMAXCONN);                             // 클라이언트 연결 대기 상태로 전환.
    if (iRet == SOCKET_ERROR) return 1;

    // 넌블록킹 소켓전환
    u_long on = 1;
    ioctlsocket(sock, FIONBIO, &on);

    SOCKADDR_IN clientaddr;                                     // 클라이언트 연결요청을 수락하고, THost 객체에 저장.
    int addlen = sizeof(clientaddr);

    std::list<THost> userList;
    while (userList.size() != 1)
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
            host.bConnect = true;
            userList.push_back(host);
        }
    }
    
    while (1)
    {
        //데이터 수신 및 처리
        for (auto iter = userList.begin();
            iter != userList.end();
            iter++)
        {
            THost& host = *iter;
            if (host.bConnect == false) continue;
            ZeroMemory(host.Recvbuffer, sizeof(char) * 256);
            host.iRecvBytes = 0;
            int iRecvSize = recv(host.sock,
                host.Recvbuffer,
                sizeof(host.Recvbuffer), 0);
            if (iRecvSize == 0)
            {
                host.bConnect = false;
            }
            if (Check(host, iRecvSize))
            {
                host.iRecvBytes = iRecvSize;
                std::cout << host.Recvbuffer << std::endl;
            }
        }

        //브로드캐스트 (데이터 전송)
        for (auto recvHost = userList.begin();
            recvHost != userList.end();
            recvHost++)
        {
            THost& sendData = *recvHost;
            if (sendData.bConnect == false) continue;
            if (sendData.iRecvBytes <= 0)
            {
                continue;
            }
            for (auto sendHost = userList.begin(); sendHost != userList.end(); sendHost++)
            {
                THost& host = *sendHost;
                if (host.bConnect == false) continue;
                int iSendSize = send(host.sock, sendData.Recvbuffer,
                    sendData.iRecvBytes, 0);
                Check(host, iSendSize);
            }
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
                iter++;
            }
        }
    }

    closesocket(sock);
    WSACleanup();

}
