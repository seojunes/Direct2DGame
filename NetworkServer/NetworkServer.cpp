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

    WSADATA wsa;
    int iRet =  WSAStartup(MAKEWORD(2,2), &wsa);
    if (iRet != 0) return 1;
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); 
    SOCKADDR_IN sa; 
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("192.168.0.110"); 
    sa.sin_port = htons(10000);        
    //iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
    iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));  // 집에 전화를 개통해놓음. 클라는 connect 서버는 bind 
    if (iRet == SOCKET_ERROR) return 1;
    listen(sock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) return 1;
    SOCKADDR_IN clientaddr;
    int addlen = sizeof(clientaddr);
    SOCKET clientSock = accept(sock, (SOCKADDR*)&clientaddr, &addlen);             //누가 전화를 했는지 return까지 해준다. 
    // 전화벨이 울리면, 누가 전화했는지 확인하는 것이 clientaddr 
    //accept가 반환이 됐다는 것은 이미 연결이 되어있다는 이야기.
    // 두개의 소켓이 open인 상태를 연결되었다고 한다.
    
    char buffer[256] = "0, ";
    int iRedvSize = recv(clientSock, buffer, sizeof(buffer), 0);
    std::cout << buffer << std::endl;
    closesocket(clientSock);
    closesocket(sock);
    WSACleanup();
}

