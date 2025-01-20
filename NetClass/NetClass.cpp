#define _WINSOCK_DEPRECATED_NO_WARNINGS                 //이전 API함수에서 발생하는 컴파일 경고를 무시하기 위한 매크로

// 네트워크를 사용하기 위한 환경설정.
#include <winsock2.h>           //Windows 소켓관련 함수 및 구조체 정의
#include <ws2tcpip.h>           //최신 TCP/IP 관련 기능.
#include <mswsock.h>            //Microsoft 확장 소켓 기능.
#include <iostream>             
#include <string>               

#pragma comment(lib, "ws2_32.lib")          //네트워크 함수 사용을 위해 ws2_32.lib라이브러리 링크
#pragma comment(lib, "mswsock.lib")         //고급 소켓 기능을 위한 라이브러리 링크.


int main()
{
    // 윈속 초기화( 버전 선택) WSAStartup
    WSADATA wsa;                //윈속 정보를 담는 구조체
    int iRet = WSAStartup(MAKEWORD(2, 2), &wsa); // WinSock 2.2버젼 버젼 초기화
    if (iRet != 0) return 1;                    
    // 윈도우에서는 0값이 떨어지면 오류라고 보면된다.
    // 윈속을 만들면 이제 서버가 사용가능하고, 2.2버젼 위의 버전이 없다.
    // Windows 환겨엥서는 반드시 소켓 사용 전에 WSAStartup을 호출해야함.


    // 소켓생성 : 네크워크를 할때 연결을 하는 도구. 서버 <-> 클라
    SOCKET sock;            // 소켓을 저장할 변수 sock.
    // TCP & IP 프로토콜 
    sock = socket(AF_INET, SOCK_STREAM, 0);  //원래는 0자리에 IPPROTO_TCP를 넘겨주는게 FM이다. AF_INET이 IP이다.
                //AF_INET: Ipv4주소체계사용, SOCK_STREAM: TCP 연결 방식 사용  
    
                
    //전화번호 만들기(서버 주소 설정)
    SOCKADDR_IN sa; //연결해주는 전화번호 (구조체로 되어있음) << 서버 주소 정보를 저장할 구조체
    ZeroMemory(&sa, sizeof(sa));                              //메모리 초기화
    // 3가지를 설정해주면 해당 전화번호로 전화를 해준다.
    sa.sin_family = AF_INET;  //ip프로토콜
    sa.sin_addr.s_addr = inet_addr("192.168.0.44");  // 주소                    << 전화번호   inet_addr 비추천 대신에 inet_pton 권장.
    sa.sin_port = htons(10000);         //번호생성 완료. 응용프로그램 인덱스.    << 전화번호로 생각하면 받는사람
    // 포트를 네트워크 바이트 순서(빅 엔디안)로 변환.

    //접속 명령어( 서버 접속 )
    iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
    //전화를 안받으면 에러를 체크한다.
    //connect를 통해서 서버 접속을 시도해본다. sa라는 서버 주소관련 변수를 통해서 접근 시도.
    if (iRet == SOCKET_ERROR)       return 1;
    //char buffer[256] = "서준입니다.";//{ 0, };
    ////send가 얼마만큼 보내줬는지를 반환해준다.
    //int iSendSize = send(sock, buffer, sizeof(buffer), 0);
    
    
    std::string SendBuf;
    SendBuf.resize(256);
    std::string RecvBuf;

    while (1)
    {
        std::getline(std::cin, SendBuf);
        if (SendBuf.empty())
        {
            break;
        }
        int iSendSize = send(sock, &SendBuf[0], SendBuf.size(), 0);
        if (iSendSize == 0 || iSendSize == SOCKET_ERROR) {
            std::cout << "서버가 종료되었거나 문제가 발생" << std::endl;
            break;
        }
        RecvBuf.resize(256);
        int iRecvSize = recv(sock, &RecvBuf[0], RecvBuf.size(), 0);
        if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR) {
            std::cout << "서버가 종료되었거나 문제가 발생" << std::endl;
            break;
        }
        std::cout << RecvBuf << std::endl;
        RecvBuf.clear();
    }

    closesocket(sock); // 소켓을 닫아준다.

    // 윈속 소멸
    WSACleanup();
    // 생성과 초기화를 1번만 넣어주면 된다.
}

