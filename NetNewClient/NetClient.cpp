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
DWORD WorkSendMacro(LPVOID lpThreadParameter)
{
    TNetwork* net = (TNetwork*)lpThreadParameter;
    std::string SendBuf = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";    
    
    int iCount = 0;
    std::string szCount = std::to_string(iCount);
    while (net->m_bRun)
    {
        szCount = std::to_string(iCount++);       
        szCount += SendBuf;
        net->SendWork(szCount);
        Sleep(50);
        szCount.c_str();
    }
    return 1;
}
int main()
{
    // ********
    // TCP의 특징
    // 단, 패킷의 데이터는 쪼개져서 연속적으로 들어온다.
    // 여기서 보내 패킷의 순서는 보장된다.
    //PACKET_HEADER1->MSG1->PACKET_HEADER2->MSG2->PACKET_HEADER3->MSG3 
    // ********

    TNetwork net;   
    net.Init();
    if (net.Connect("192.168.0.88", 10000))
    {       
        HANDLE hThread1 = CreateThread(NULL, 0, WorkSend, &net, 0, NULL);
        CloseHandle(hThread1);
        net.Run();
    }
    net.Release();
}
