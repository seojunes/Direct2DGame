#define _WINSOCK_DEPRECATED_NO_WARNINGS

// 네트워크를 사용하기 위한 환경설정.
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")


int main()
{
    // 윈속 초기화( 버전 선택)
    WSADATA wsa;
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa); // 2.2버젼
    if (iRet != 0) return 1;
    // 윈도우에서는 0값이 떨어지면 오류라고 보면된다.
    // 윈속을 만들면 이제 서버가 사용가능하고, 2.2버젼 위의 버전이 없다.

    // 소켓생성 : 네크워크를 할때 연결을 하는 도구. 서버 <-> 클라
    SOCKET sock;
    // TCP & IP 프로토콜 
    sock = socket(AF_INET, SOCK_STREAM, 0);  //원래는 0자리에 IPPROTO_TCP를 넘겨주는게 FM이다. AF_INET이 IP이다.
    //전화번호 만들기
    SOCKADDR_IN sa; //연결해주는 전화번호 (구조체로 되어있음)
    ZeroMemory(&sa, sizeof(sa));
    // 3가지를 설정해주면 해당 전화번호로 전화를 해준다.
    sa.sin_family = AF_INET;  //ip프로토콜
    sa.sin_addr.s_addr = inet_addr("192.168.0.88");  // 주소                    << 전화번호
    sa.sin_port = htons(10000);         //번호생성 완료. 응용프로그램 인덱스.    << 전화번호로 생각하면 받는사람

    //접속 명령어
    iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
    //전화를 안받으면 에러를 체크한다.
    if (iRet == SOCKET_ERROR)       return 1;
    char buffer[256] = "서준입니다.";//{ 0, };
    //send가 얼마만큼 보내줬는지를 반환해준다.
    int iSendSize = send(sock, buffer, sizeof(buffer), 0);

    closesocket(sock); // 소켓을 닫아준다.

    // 윈속 소멸
    WSACleanup();
    // 생성과 초기화를 1번만 넣어주면 된다.
}

