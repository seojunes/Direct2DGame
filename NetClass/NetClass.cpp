#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>
#include <string>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

// 오류 처리 send()및 recv() 호출 결과를 확인.
bool Check(int iCode)
{
    if (iCode == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)           // WSAEVOULDBLOCK : 논 블로킹상태에서 데이터가 준비되지 않았음을 의미하므로 무시.
        {
            std::cout << "서버가 비정상종료,문제가 발생" << std::endl;               // 다른 오류가 발생시 프로그램 종료
            exit(1);
        }
        return false;
    }
    return true;
}
int main()
{
    // 윈속 초기화( 버전선택)
    WSADATA wsa;
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    // 소켓생성
    SOCKET sock;
    //  TCP & IP 프로토콜
    sock = socket(AF_INET, SOCK_STREAM, 0);// IPPROTO_TCP);

    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    //error C4996: 'inet_addr': Use inet_pton() or InetPton() 
    //instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = inet_addr("192.168.0.88");// 전화번호
    sa.sin_port = htons(10000); // 받는 사람
    iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) return 1;

    // 넌블록킹 소켓전환
    u_long on = 1;
    ioctlsocket(sock, FIONBIO, &on);        //ioctlsocket에서 FIONBIO를 통해서 논블록킹으로 전환

    std::string SendBuf;
    SendBuf.reserve(256);
    std::string RecvBuf;

    while (1)
    {
        if (_kbhit() == 1)                  // 키입력이 있는지 없는지를 확인
        {
            int iValue = _getche();         // 글자를 받아서 한글자씩 처리 (화면에 보여줌)(안보여주는 함수는 _getch)
            if (SendBuf.empty())
            {
                if (iValue == '\r')         //\r : 엔터키를 입력시 생성
                {
                    break;
                }
            }
            SendBuf.push_back(iValue);
            //std::getline(std::cin, SendBuf);
           /* if (SendBuf.empty())
            {
                break;
            }*/
            if (iValue == '\r')
            {
                int iSendSize = send(sock, &SendBuf[0], SendBuf.size() - 1, 0);             // 메세지 전송 후 오류 검사 수행
                if (!Check(iSendSize)) {
                    break;
                }
                SendBuf.clear();
            }
        }
        RecvBuf.resize(256);
        int iRecvSize = recv(sock, &RecvBuf[0], RecvBuf.size(), 0);                         //데이터 수신
        if (iRecvSize == 0)
        {
            std::cout << "서버가 정상종료" << std::endl;
            break;
        }
        if (Check(iRecvSize))
        {
            std::cout << std::endl;
            std::cout << RecvBuf << std::endl;
        }
        RecvBuf.clear();
    }
    closesocket(sock);
    // 윈속 소멸
    WSACleanup();
    std::cout << "Hello World!\n";
}
