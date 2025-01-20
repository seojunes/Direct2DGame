#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
int main()
{
	//윈속 초기화
	WSADATA wsa;
	int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iRet != 0) return 1;

	//소켓생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);// IPPROTO_TCP);

	//서버 주소 설정 및 바인딩
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);// 전화번호
	sa.sin_port = htons(10000); // 받는 사람

	//소켓 바인딩(bind)
	iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));          // 소켓을 특정 IP및 포트와 연결
	if (iRet == SOCKET_ERROR) return 1;

	//연결 대기 설정(listen)
	iRet = listen(sock, SOMAXCONN);         //클라이언트를 받을 준비
	if (iRet == SOCKET_ERROR) return 1;

	//클라이언트 연결 수락 (accept) 
	SOCKADDR_IN clientaddr;
	int addlen = sizeof(clientaddr);

	//데이터 수신 및 전송 루프
	std::string SendBuf;
	while (1)
	{
		SOCKET clientSock = accept(sock, (SOCKADDR*)&clientaddr, &addlen);
		std::cout << "접속(IP):" << inet_ntoa(clientaddr.sin_addr)
			<< "포트번호" << ntohs(clientaddr.sin_port)
			<< std::endl;
		while (1)
		{
			char Recvbuffer[256] = { 0, };
			int iRecvSize = recv(clientSock, Recvbuffer, sizeof(Recvbuffer), 0);
			if (iRecvSize == 0)
			{
				std::cout << "접속종료(IP):" << inet_ntoa(clientaddr.sin_addr)
					<< "포트번호" << ntohs(clientaddr.sin_port)
					<< std::endl;
				closesocket(clientSock);
			}
			std::cout << Recvbuffer << std::endl;
			SendBuf = Recvbuffer;
			SendBuf += " 이렇게 말했음.";
			int iSendSize = send(clientSock,
				&SendBuf[0], SendBuf.size(), 0);
			if (iSendSize == SOCKET_ERROR)
			{
				std::cout << "접속종료(IP):" << inet_ntoa(clientaddr.sin_addr)
					<< "포트번호" << ntohs(clientaddr.sin_port)
					<< std::endl;
				closesocket(clientSock);
				std::cout << "서버에 문제가 있다." << std::endl;
				break;
			}
			SendBuf.clear();
		}
	}
	// Sleep(10000);
	// 서버 종료 처리
	closesocket(sock);
	WSACleanup();

}
